#include <Weave/WeaveSerializer.hpp>
#include <Weave/WeaveGraphScene.hpp>

#include <QFile>
#include <QMap>
#include <cstring>

namespace NuEditor::Weave
{
    namespace
    {
        void WriteU8(QByteArray& buff, uint8_t value)
        {
            buff.append(static_cast<char>(value));
        }

        void WriteU16(QByteArray& buff, uint16_t value)
        {
            uint8_t b[2];
            b[0] = static_cast<uint8_t>(value & 0xFF);
            b[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
            buff.append(reinterpret_cast<const char*>(b), 2);
        }

        void WriteU32(QByteArray& buff, uint32_t value)
        {
            uint8_t b[4];
            for (int i = 0; i < 4; ++i)
            {
                b[i] = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
            }

            buff.append(reinterpret_cast<const char*>(b), 4);
        }

        void WriteI32(QByteArray& buff, int32_t value)
        {
            WriteU32(buff, static_cast<uint32_t>(value));
        }

        void WriteF32(QByteArray& buff, float value)
        {
            uint32_t bits;
            std::memcpy(&bits, &value, 4);
            WriteU32(buff, bits);
        }

        void WriteString(QByteArray& buff, const QString& str)
        {
            QByteArray utf8 = str.toUtf8();
            uint16_t len = static_cast<uint16_t>(qMin(utf8.size(), 65535));
            WriteU16(buff, len);
            buff.append(utf8.constData(), len);
        }

        bool ReadU8(const QByteArray& buff, int& pos, uint8_t& out)
        {
            if (pos + 1 > buff.size()) 
            {
                return false;
            }

            out = static_cast<uint8_t>(buff[pos]);
            ++pos;

            return true;
        }

        bool ReadU16(const QByteArray& buff, int& pos, uint16_t& out)
        {
            if (pos + 2 > buff.size()) 
            {
                return false;
            }

            out = static_cast<uint16_t>(static_cast<uint8_t>(buff[pos]) |
                 (static_cast<uint8_t>(buff[pos + 1]) << 8));
            pos += 2;

            return true;
        }

        bool ReadU32(const QByteArray& buff, int& pos, uint32_t& out)
        {
            if (pos + 4 > buff.size()) 
            {
                return false;
            }
            out = 0;

            for (int i = 0; i < 4; ++i)
            {
                out |= static_cast<uint32_t>(static_cast<uint8_t>(buff[pos + i])) << (i * 8);
            }

            pos += 4;
            return true;
        }

        bool ReadI32(const QByteArray& buff, int& pos, int32_t& out)
        {
            uint32_t u = 0;

            if (!ReadU32(buff, pos, u)) 
            {
                return false;
            }

            std::memcpy(&out, &u, 4);
            return true;
        }

        bool ReadF32(const QByteArray& buff, int& pos, float& out)
        {
            uint32_t u = 0;

            if (!ReadU32(buff, pos, u)) 
            {
                return false;
            }

            std::memcpy(&out, &u, 4);
            return true;
        }

        bool ReadString(const QByteArray& buff, int& pos, QString& out)
        {
            uint16_t len = 0;

            if (!ReadU16(buff, pos, len)) 
            {
                return false;
            }

            if (pos + len > buff.size()) 
            {
                return false;
            }

            out = QString::fromUtf8(buff.constData() + pos, len);
            pos += len;
            return true;
        }
    }

    WeaveSerializer::SaveResult WeaveSerializer::Save(const WeaveGraphScene& scene, const QString& path)
    {
        SaveResult result;
        QFile f(path);

        if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            result.Error = "Can't open file for writing: " + path;
            return result;
        }

        QByteArray buff;
        buff.reserve(1024);

        const auto& nodes = scene.GetNodes();
        const auto& connections = scene.GetConnections();

        WriteU32(buff, k_Magic);
        WriteU16(buff, k_Version);
        WriteU16(buff, 0);
        WriteU32(buff, static_cast<uint32_t>(nodes.size()));
        WriteU32(buff, static_cast<uint32_t>(connections.size()));

        for (const auto& node : nodes)
        {
            WriteI32(buff, node.Id);
            WriteU8(buff, static_cast<uint8_t>(node.Kind));
            WriteF32(buff, static_cast<float>(node.Position.x()));
            WriteF32(buff, static_cast<float>(node.Position.y()));
            WriteString(buff, node.Title);

            uint8_t dvCount = static_cast<uint8_t>(qMin(node.PinDefaultValues.size(), 255));
            WriteU8(buff, dvCount);

            for (int i = 0; i < dvCount; ++i)
            {
                WriteString(buff, node.PinDefaultValues[i]);
            }
        }

        for (const auto& conn : connections)
        {
            WriteI32(buff, conn.FromNodeId);
            WriteU8(buff, static_cast<uint8_t>(conn.FromPinIdx));
            WriteI32(buff, conn.ToNodeId);
            WriteU8(buff, static_cast<uint8_t>(conn.ToPinIdx));
        }

        f.write(buff);
        f.close();

        if (f.error() != QFileDevice::NoError)
        {
            result.Error = "Write error: " + f.errorString();
            return result;
        }

        result.Success = true;
        return result;
    }

    WeaveSerializer::LoadResult WeaveSerializer::Load(WeaveGraphScene& scene, const QString& path)
    {
        LoadResult result;
        QFile f(path);

        if (!f.open(QIODevice::ReadOnly))
        {
            result.Error = "Can't open file for reading: " + path;
            return result;
        }

        QByteArray buff = f.readAll();
        f.close();

        int pos = 0;
        uint32_t magic = 0;
        uint16_t version = 0, flags = 0;
        uint32_t nodeCount = 0, connCount = 0;

        if (!ReadU32(buff, pos, magic) || magic != k_Magic)
        {
            result.Error = "Invalid .wgraph file (bad magic).";
            return result;
        }

        if (!ReadU16(buff, pos, version) || version != k_Version)
        {
            result.Error = "Unsupported .wgraph version: " + QString::number(version);
            return result;
        }

        ReadU16(buff, pos, flags);

        if (!ReadU32(buff, pos, nodeCount) || !ReadU32(buff, pos, connCount))
        {
            result.Error = "Truncated header.";
            return result;
        }

        QVector<int> ids;
        for (const auto& node : scene.GetNodes())
        {
            ids.append(node.Id);
        }

        for (int id : ids)
        {
            scene.RemoveNode(id);
        }

        QMap<int32_t, int> idRemap;

        for (uint32_t ni = 0; ni < nodeCount; ++ni)
        {
            int32_t oldId = 0;
            uint8_t kindU8 = 0;
            float x = 0, y = 0;
            QString title;
            uint8_t dvCount = 0;

            if (!ReadI32(buff, pos, oldId) ||
                !ReadU8(buff, pos, kindU8) ||
                !ReadF32(buff, pos, x)     ||
                !ReadF32(buff, pos, y)     ||
                !ReadString(buff, pos, title))
            {
                result.Error = "Truncated node data at index " + QString::number(ni);
                return result;
            }

            if (!ReadU8(buff, pos, dvCount))
            {
                result.Error = "Truncated node defaultValues count.";
                return result;
            }

            QVector<QString> dvs;

            for (int di = 0; di < dvCount; ++di)
            {
                QString dv;
                if (!ReadString(buff, pos, dv))
                {
                    result.Error = "Truncated defaultValue string.";
                    return result;
                }
                dvs.append(dv);
            }

            auto kind = static_cast<WeaveNodeKind>(kindU8);
            int newId = scene.SpawnNode(kind, QPointF(x, y));

            if (newId == -1)
            {
                result.Error += "Warning: unknown NodeKind " + QString::number(kindU8) + " skipped.\n";
                continue;
            }

            idRemap[oldId] = newId;

            WeaveNode* node = scene.FindNode(newId);

            if (node)
            {
                if (!title.isEmpty())
                {
                    node->Title = title;
                }

                node->PinDefaultValues.resize(node->Pins.size());
                for (int i = 0; i < node->PinDefaultValues.size() && i < dvs.size(); ++i)
                {
                    node->PinDefaultValues[i] = dvs[i];
                }
            }

            ++result.NodesLoaded;
        }

        for (uint32_t ci = 0; ci < connCount; ++ci)
        {
            int32_t fromNode = 0, toNode = 0;
            uint8_t fromPin = 0, toPin = 0;

            if (!ReadI32(buff, pos, fromNode) ||
                !ReadU8(buff, pos, fromPin)   ||
                !ReadI32(buff, pos, toNode)   ||
                !ReadU8(buff, pos, toPin))
            {
                result.Error = "Truncated connection data at index " + QString::number(ci);
                return result;
            }

            if (!idRemap.contains(fromNode) || !idRemap.contains(toNode))
            {
                continue;
            }

            WeaveConnection conn;
            conn.FromNodeId = idRemap[fromNode];
            conn.FromPinIdx = static_cast<int>(fromPin);
            conn.ToNodeId = idRemap[toNode];
            conn.ToPinIdx = static_cast<int>(toPin);

            bool ok = scene.AddConnection(conn);

            if (ok)
            {
                ++result.ConnectionsLoaded;
            }
            else
            {
                result.Error += "Warning: connection skipped (cycle or duplicate).\n";
            }
        }

        result.Success = true;
        return result;
    }
}
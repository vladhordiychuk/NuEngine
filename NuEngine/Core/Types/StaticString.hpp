// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <algorithm>
#include <cassert>
#include <compare>
#include <iterator>
#include <string_view>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <type_traits>
#include <utility>

namespace NuEngine::Core
{
	/**
	 * @brief Fixed-capacity, null-terminated string with an STL-like interface.
	 *
	 * StaticString stores up to N-1 characters inline (no heap allocation) and
	 * is usable in constexpr contexts wherever possible.
	 *
	 * @tparam N Total buffer size in bytes, including the null terminator.
	 */
	template <size_t N>
	class StaticString
	{
	public:
		// ==========================
		// Construction / Assignment
		// ==========================

		/**
		 * @brief Constructs an empty string.
		*/
		constexpr StaticString() = default;

		/**
		 * @brief Constructs a string from a string view.
		 *
		 * The input is truncated if it exceeds the available capacity.
		 *
		 * @param sv Source string.
		 */
		constexpr StaticString(std::string_view sv) noexcept
		{
			Assign(sv);
		}

		/**
		 * @brief Constructs a string from a string literal.
		 *
		 * @tparam L Length of the string literal including the null terminator.
		 * @param lit Source string literal.
		 */
		template <size_t L>
		constexpr StaticString(const char(&lit)[L]) noexcept
			: StaticString(std::string_view(lit, L - 1))
		{
		}

		/**
		 * @brief Constructs from a StaticString of a different capacity.
		 *
		 * If the source string is longer than this string's capacity,
		 * it is truncated the same way as any other over-long assignment.
		 *
		 * @tparam M Capacity of the source string.
		 * @param other Source string.
		 */
		template <size_t M>
		constexpr StaticString(const StaticString<M>& other) noexcept
		{
			Assign(other.View());
		}

		/**
		 * @brief Constructs a string from a string literal.
		 *
		 * @tparam L Length of the string literal including the null terminator.
		 * @param lit Source string literal.
		 */
		constexpr StaticString& operator=(std::string_view sv) noexcept
		{
			Assign(sv);
			return *this;
		}

		/**
		 * @brief Assigns from a string literal.
		 *
		 * Without this overload, `s = "text";` is ambiguous between converting
		 * the literal to std::string_view and converting it to StaticString via
		 * the array constructor. This exact-match overload for char arrays wins
		 * over both, resolving the ambiguity.
		 *
		 * @tparam L Length of the string literal including the null terminator.
		 * @param lit Source string literal.
		 */
		template <size_t L>
		constexpr StaticString& operator=(const char(&lit)[L]) noexcept
		{
			Assign(std::string_view(lit, L - 1));
			return *this;
		}

		// ==========================
		// Views / Conversions
		// ==========================

		/**
		 * @brief Returns a view of the stored string.
		 *
		 * @return std::string_view over the internal buffer.
		 */
		[[nodiscard]] constexpr std::string_view View() const noexcept
		{
			return { m_data, m_size };
		}

		/**
		 * @brief Returns a null-terminated C string.
		 *
		 * @return Pointer to the internal buffer.
		 */
		[[nodiscard]] constexpr const char* CStr() const noexcept
		{
			return m_data;
		}

		/**
		 * @brief Returns a pointer to the underlying character buffer.
		 *
		 * @return Pointer to the internal storage.
		 */
		[[nodiscard]] constexpr char* Data() noexcept
		{
			return m_data;
		}

		/**
		 * @brief Returns a pointer to the underlying character buffer.
		 *
		 * @return Pointer to the internal storage.
		 */
		[[nodiscard]] constexpr const char* Data() const noexcept
		{
			return m_data;
		}

		/**
		 * @brief Implicit conversion to std::string_view.
		 */
		[[nodiscard]] constexpr operator std::string_view() const noexcept
		{
			return View();
		}

		// ==========================
		// Size / Capacity
		// ==========================

		/**
		 * @brief Returns the current string length.
		 *
		 * @return Number of stored characters.
		 */
		[[nodiscard]] constexpr uint16_t Size() const noexcept
		{
			return m_size;
		}

		/**
		 * @brief Checks whether the string is empty.
		 *
		 * @return True if the string contains no characters.
		 */
		[[nodiscard]] constexpr bool Empty() const noexcept
		{
			return m_size == 0;
		}

		/**
		 * @brief Returns the maximum string capacity.
		 *
		 * @return Maximum number of characters excluding the null terminator.
		 */
		[[nodiscard]] constexpr size_t Capacity() const noexcept
		{
			return CapacityValue;
		}

		/**
		 * @brief Returns the maximum number of characters the string can hold.
		 *
		 * Equivalent to Capacity(); provided for STL-container familiarity.
		 *
		 * @return Maximum string length.
		 */
		[[nodiscard]] constexpr size_t MaxSize() const noexcept
		{
			return CapacityValue;
		}

		// ==========================
		// Element Access
		// ==========================

		/**
		 * @brief Returns a reference to the character at the specified index.
		 *
		 * @param index Character index.
		 * @return Reference to the character.
		 */
		[[nodiscard]] constexpr char& operator[](size_t index) noexcept
		{
			assert(index < m_size);
			return m_data[index];
		}

		/**
		 * @brief Returns the character at the specified index.
		 *
		 * @param index Character index.
		 * @return Character value.
		 */
		[[nodiscard]] constexpr char operator[](size_t index) const noexcept
		{
			assert(index < m_size);
			return m_data[index];
		}

		/**
		 * @brief Returns the character at the specified index, with bounds checking in debug builds.
		 *
		 * @param index Character index.
		 * @return Character value.
		 */
		[[nodiscard]] constexpr char At(size_t index) const noexcept
		{
			assert(index < m_size);
			return m_data[index];
		}

		/**
		 * @brief Returns a reference to the first character.
		 *
		 * @return Reference to the first character.
		 */
		[[nodiscard]] constexpr char& Front() noexcept
		{
			assert(!Empty());
			return m_data[0];
		}

		/**
		 * @brief Returns the first character.
		 *
		 * @return Value of the first character.
		 */
		[[nodiscard]] constexpr const char& Front() const noexcept
		{
			assert(!Empty());
			return m_data[0];
		}

		/**
		 * @brief Returns a reference to the last character.
		 *
		 * @return Reference to the last character.
		 */
		[[nodiscard]] constexpr char& Back() noexcept
		{
			assert(!Empty());
			return m_data[m_size - 1];
		}

		/**
		 * @brief Returns the last character.
		 *
		 * @return Value of the last character.
		 */
		[[nodiscard]] constexpr const char& Back() const noexcept
		{
			assert(!Empty());
			return m_data[m_size - 1];
		}

		// ==========================
		// Comparison
		// ==========================

		/**
		 * @brief Compares the string with a string view.
		 *
		 * @param rhs String to compare against.
		 * @return True if both strings are equal.
		 */
		[[nodiscard]] constexpr bool operator==(std::string_view rhs) const noexcept
		{
			return View() == rhs;
		}

		/**
		 * @brief Three-way comparison against a string_view.
		 *
		 * The compiler synthesizes <, <=, >, >= from this operator, so no
		 * separate overloads for those are needed.
		 *
		 * @param rhs String to compare against.
		 * @return Ordering result of the comparison.
		 */
		[[nodiscard]] constexpr auto operator<=>(std::string_view rhs) const noexcept
		{
			return View() <=> rhs;
		}

		/**
		 * @brief Lexicographically compares against another string, STL-style.
		 *
		 * @param rhs String to compare against.
		 * @return Negative value if this string is less than rhs, zero if equal,
		 *         positive value if this string is greater than rhs.
		 */
		[[nodiscard]] constexpr int Compare(std::string_view rhs) const noexcept
		{
			return View().compare(rhs);
		}

		// ==========================
		// Search
		// ==========================

		/**
		 * @brief Returns a substring view.
		 *
		 * @param pos Starting position.
		 * @param count Number of characters.
		 * @return Substring view.
		 */
		[[nodiscard]] constexpr std::string_view Substr(size_t pos, size_t count = std::string_view::npos) const noexcept
		{
			return View().substr(pos, count);
		}

		/**
		 * @brief Finds the first occurrence of a substring.
		 *
		 * @param str Substring to search for.
		 * @param pos Position to start searching from.
		 * @return Index of the match or std::string_view::npos.
		 */
		[[nodiscard]] constexpr size_t Find(std::string_view str, size_t pos = 0) const noexcept
		{
			return View().find(str, pos);
		}

		/**
		 * @brief Finds the first occurrence of a character starting at a given position.
		 *
		 * @param c Character to search for.
		 * @param pos Position to start searching from.
		 * @return Index of the first match, or std::string_view::npos if not found.
		 */
		[[nodiscard]] constexpr size_t Find(char c, size_t pos = 0) const noexcept
		{
			return View().find(c, pos);
		}

		/**
		 * @brief Finds the first character that matches any character in the given set.
		 *
		 * @param chars Set of characters to search for.
		 * @param pos Position to start searching from.
		 * @return Index of the first match, or std::string_view::npos if not found.
		 */
		[[nodiscard]] constexpr size_t FindFirstOf(std::string_view chars, size_t pos = 0) const noexcept
		{
			return View().find_first_of(chars, pos);
		}

		/**
		 * @brief Finds the last character that matches any character in the given set.
		 *
		 * @param chars Set of characters to search for.
		 * @param pos Position to search backwards from.
		 * @return Index of the last match, or std::string_view::npos if not found.
		 */
		[[nodiscard]] constexpr size_t FindLastOf(std::string_view chars, size_t pos = std::string_view::npos) const noexcept
		{
			return View().find_last_of(chars, pos);
		}

		/**
		 * @brief Finds the first character that does not match any character in the given set.
		 *
		 * @param chars Set of characters to exclude.
		 * @param pos Position to start searching from.
		 * @return Index of the first non-match, or std::string_view::npos if not found.
		 */
		[[nodiscard]] constexpr size_t FindFirstNotOf(std::string_view chars, size_t pos = 0) const noexcept
		{
			return View().find_first_not_of(chars, pos);
		}

		/**
		 * @brief Finds the last character that does not match any character in the given set.
		 *
		 * @param chars Set of characters to exclude.
		 * @param pos Position to search backwards from.
		 * @return Index of the last non-match, or std::string_view::npos if not found.
		 */
		[[nodiscard]] constexpr size_t FindLastNotOf(std::string_view chars, size_t pos = std::string_view::npos) const noexcept
		{
			return View().find_last_not_of(chars, pos);
		}

		/**
		 * @brief Checks whether the string starts with a substring.
		 *
		 * @param sv Prefix to check.
		 * @return True if the prefix matches.
		 */
		[[nodiscard]] constexpr bool StartsWith(std::string_view sv) const noexcept
		{
			return View().starts_with(sv);
		}

		/**
		 * @brief Checks whether the string begins with the given character.
		 *
		 * @param c Character to check for.
		 * @return True if the string is non-empty and its first character is c.
		 */
		[[nodiscard]] constexpr bool StartsWith(char c) const noexcept
		{
			return !Empty() && m_data[0] == c;
		}

		/**
		 * @brief Checks whether the string ends with a substring.
		 *
		 * @param sv Suffix to check.
		 * @return True if the suffix matches.
		 */
		[[nodiscard]] constexpr bool EndsWith(std::string_view sv) const noexcept
		{
			return View().ends_with(sv);
		}

		/**
		 * @brief Checks whether the string ends with the given character.
		 *
		 * @param c Character to check for.
		 * @return True if the string is non-empty and its last character is c.
		 */
		[[nodiscard]] constexpr bool EndsWith(char c) const noexcept
		{
			return !Empty() && m_data[m_size - 1] == c;
		}

		/**
		 * @brief Checks whether the string contains a substring.
		 *
		 * @param sv Substring to search for.
		 * @return True if the substring is found.
		 */
		[[nodiscard]] constexpr bool Contains(std::string_view sv) const noexcept
		{
			return Find(sv) != std::string_view::npos;
		}

		/**
		 * @brief Checks whether the string contains the given character.
		 *
		 * @param c Character to search for.
		 * @return True if c occurs at least once.
		 */
		[[nodiscard]] constexpr bool Contains(char c) const noexcept
		{
			return Find(c) != std::string_view::npos;
		}

		/**
		 * @brief Case-insensitive equality comparison (ASCII only).
		 *
		 * @param rhs String to compare against.
		 * @return True if both strings have equal length and match ignoring ASCII case.
		 */
		[[nodiscard]] constexpr bool EqualsIgnoreCase(std::string_view rhs) const noexcept
		{
			if (m_size != rhs.size())
			{
				return false;
			}

			auto toLower = [](char c) constexpr noexcept
				{
					return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + ('a' - 'A')) : c;
				};

			for (size_t i = 0; i < m_size; ++i)
			{
				if (toLower(m_data[i]) != toLower(rhs[i]))
				{
					return false;
				}
			}

			return true;
		}

		// ==========================
		// Modifiers
		// ==========================

		/**
		 * @brief Appends characters to the end of the string.
		 *
		 * If the appended string exceeds the remaining capacity,
		 * only the portion that fits is copied.
		 *
		 * @param sv String to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(std::string_view sv) noexcept
		{
			const size_t free = CapacityValue - m_size;
			const size_t copy = std::min(sv.size(), free);

			if (copy == 0)
			{
				return *this;
			}

			if (std::is_constant_evaluated())
			{
				for (size_t i = 0; i < copy; ++i)
				{
					m_data[m_size + i] = sv[i];
				}
			}
			else
			{
				std::memcpy(m_data + m_size, sv.data(), copy);
			}

			m_size += static_cast<uint16_t>(copy);
			m_data[m_size] = '\0';

			return *this;
		}

		/**
		 * @brief Appends a single character.
		 *
		 * Does nothing if the string is already full.
		 *
		 * @param c Character to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(char c) noexcept
		{
			return PushBack(c);
		}

		/**
		 * @brief Appends the textual representation of a boolean value ("true"/"false").
		 *
		 * @param value Value to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(bool value) noexcept
		{
			return Append(value ? std::string_view("true") : std::string_view("false"));
		}

		/**
		 * @brief Appends the decimal textual representation of a signed 32-bit integer.
		 *
		 * @param value Value to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(int32_t value) noexcept
		{
			return AppendInteger(value);
		}

		/**
		 * @brief Appends the decimal textual representation of an unsigned 32-bit integer.
		 *
		 * @param value Value to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(uint32_t value) noexcept
		{
			return AppendInteger(value);
		}

		/**
		 * @brief Appends the decimal textual representation of a signed 64-bit integer.
		 *
		 * @param value Value to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(int64_t value) noexcept
		{
			return AppendInteger(value);
		}

		/**
		 * @brief Appends the decimal textual representation of an unsigned 64-bit integer.
		 *
		 * @param value Value to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Append(uint64_t value) noexcept
		{
			return AppendInteger(value);
		}

		/**
		 * @brief Appends a fixed-point textual representation of a floating point value.
		 *
		 * Not usable in constexpr context.
		 *
		 * @param value Value to append.
		 * @param precision Number of digits after the decimal point.
		 *
		 * @return Reference to this string.
		 */
		StaticString& Append(float value, int precision = 3) noexcept
		{
			char buffer[64];
			const int written = std::snprintf(buffer, sizeof(buffer), "%.*f", precision, static_cast<double>(value));

			if (written > 0)
			{
				Append(std::string_view(buffer, static_cast<size_t>(written)));
			}

			return *this;
		}

		/**
		 * @brief Appends a string.
		 *
		 * @param sv String to append.
		 * @return Reference to this string.
		 */
		constexpr StaticString& operator+=(std::string_view sv) noexcept
		{
			return Append(sv);
		}

		/**
		 * @brief Appends a character.
		 *
		 * @param c Character to append.
		 * @return Reference to this string.
		 */
		constexpr StaticString& operator+=(char c) noexcept
		{
			return PushBack(c);
		}

		/**
		 * @brief Concatenates a StaticString with a string_view.
		 *
		 * @param lhs Left-hand side string (taken by value, reused as the result).
		 * @param rhs Right-hand side string to append.
		 *
		 * @return New StaticString containing the concatenation.
		 */
		friend constexpr StaticString operator+(StaticString lhs, std::string_view rhs) noexcept
		{
			lhs.Append(rhs);
			return lhs;
		}

		/**
		 * @brief Concatenates a string_view with a StaticString.
		 *
		 * @param lhs Left-hand side string to prepend.
		 * @param rhs Right-hand side string (taken by value, reused as the result).
		 *
		 * @return New StaticString containing the concatenation.
		 */
		friend constexpr StaticString operator+(std::string_view lhs, StaticString rhs) noexcept
		{
			StaticString result(lhs);
			result.Append(rhs.View());
			return result;
		}

		/**
		 * @brief Appends a single character.
		 *
		 * Does nothing if the string is already full.
		 *
		 * @param c Character to append.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& PushBack(char c) noexcept
		{
			if (m_size >= CapacityValue)
			{
				return *this;
			}

			m_data[m_size++] = c;
			m_data[m_size] = '\0';

			return *this;
		}

		/**
		 * @brief Removes the last character.
		 *
		 * Does nothing if the string is empty.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& PopBack() noexcept
		{
			if (m_size == 0)
			{
				return *this;
			}

			--m_size;
			m_data[m_size] = '\0';

			return *this;
		}

		/**
		 * @brief Inserts a character at the specified position.
		 *
		 * If the position is invalid or the string is full,
		 * the operation has no effect.
		 *
		 * @param pos Insertion position.
		 * @param c Character to insert.
		 * @return Reference to this string.
		 */
		constexpr StaticString& Insert(size_t pos, char c) noexcept
		{
			if (pos > m_size || m_size >= CapacityValue)
			{
				return *this;
			}

			for (size_t i = m_size; i > pos; --i)
			{
				m_data[i] = m_data[i - 1];
			}

			m_data[pos] = c;
			++m_size;
			m_data[m_size] = '\0';

			return *this;
		}

		/**
		 * @brief Removes one or more characters.
		 *
		 * If the position is out of range, the operation has no effect.
		 *
		 * @param pos Starting position.
		 * @param count Number of characters to erase.
		 * @return Reference to this string.
		 */
		constexpr StaticString& Erase(size_t pos, size_t count = 1) noexcept
		{
			if (pos >= m_size)
			{
				return *this;
			}

			count = std::min(count, static_cast<size_t>(m_size - pos));

			for (size_t i = pos; i + count <= m_size; ++i)
			{
				m_data[i] = m_data[i + count];
			}

			m_size -= static_cast<uint16_t>(count);

			return *this;
		}

		/**
		 * @brief Replaces all occurrences of one character with another.
		 *
		 * @param oldChar Character to replace.
		 * @param newChar Replacement character.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Replace(char oldChar, char newChar) noexcept
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				if (m_data[i] == oldChar)
				{
					m_data[i] = newChar;
				}
			}

			return *this;
		}

		/**
		 * @brief Removes leading characters that are present in the given set.
		 *
		 * @param chars Set of characters to strip. Defaults to ASCII whitespace.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& TrimLeft(std::string_view chars = " \t\n\r\f\v") noexcept
		{
			const size_t pos = FindFirstNotOf(chars);

			if (pos == std::string_view::npos)
			{
				Clear();
			}
			else if (pos > 0)
			{
				Erase(0, pos);
			}

			return *this;
		}

		/**
		 * @brief Removes trailing characters that are present in the given set.
		 *
		 * @param chars Set of characters to strip. Defaults to ASCII whitespace.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& TrimRight(std::string_view chars = " \t\n\r\f\v") noexcept
		{
			const size_t pos = FindLastNotOf(chars);

			if (pos == std::string_view::npos)
			{
				Clear();
			}
			else
			{
				Shrink(pos + 1);
			}

			return *this;
		}

		/**
		 * @brief Removes leading and trailing characters that are present in the given set.
		 *
		 * @param chars Set of characters to strip. Defaults to ASCII whitespace.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Trim(std::string_view chars = " \t\n\r\f\v") noexcept
		{
			TrimRight(chars);
			TrimLeft(chars);
			return *this;
		}

		/**
		 * @brief Copies characters into an external buffer, STL std::string::copy style.
		 *
		 * Does not null-terminate the destination.
		 *
		 * @param dst Destination buffer.
		 * @param count Maximum number of characters to copy.
		 * @param pos Position in this string to start copying from.
		 *
		 * @return Number of characters actually copied.
		 */
		constexpr size_t Copy(char* dst, size_t count, size_t pos = 0) const noexcept
		{
			if (pos > m_size)
			{
				return 0;
			}

			const size_t copyCount = std::min(count, static_cast<size_t>(m_size - pos));

			if (std::is_constant_evaluated())
			{
				for (size_t i = 0; i < copyCount; ++i)
				{
					dst[i] = m_data[pos + i];
				}
			}
			else
			{
				std::memcpy(dst, m_data + pos, copyCount);
			}

			return copyCount;
		}

		/**
		 * @brief Removes the specified prefix if present.
		 *
		 * @param prefix Prefix to remove.
		 * @return True if the prefix was removed.
		 */
		[[nodiscard]] constexpr bool RemovePrefix(std::string_view prefix) noexcept
		{
			if (!StartsWith(prefix))
			{
				return false;
			}

			Erase(0, prefix.size());

			return true;
		}

		/**
		 * @brief Removes the specified suffix if present.
		 *
		 * @param suffix Suffix to remove.
		 * @return True if the suffix was removed.
		 */
		[[nodiscard]] constexpr bool RemoveSuffix(std::string_view suffix) noexcept
		{
			if (!EndsWith(suffix))
			{
				return false;
			}

			Shrink(m_size - suffix.size());

			return true;
		}

		/**
		 * @brief Changes the length of the string.
		 *
		 * If the new size is larger than the current size,
		 * new characters are initialized with the specified value.
		 *
		 * @param newSize New string length.
		 * @param fill Character used to initialize new elements.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& Resize(size_t newSize, char fill = '\0') noexcept
		{
			newSize = std::min(newSize, CapacityValue);

			if (newSize > m_size)
			{
				for (size_t i = m_size; i < newSize; ++i)
				{
					m_data[i] = fill;
				}
			}

			m_size = static_cast<uint16_t>(newSize);
			m_data[m_size] = '\0';

			return *this;
		}

		/**
		 * @brief Reduces the string length.
		 *
		 * Does nothing if the requested size is greater than or equal
		 * to the current length.
		 *
		 * @param size New string length.
		 */
		constexpr void Shrink(size_t size) noexcept
		{
			if (size >= m_size)
			{
				return;
			}

			m_size = static_cast<uint16_t>(size);
			m_data[m_size] = '\0';
		}

		/**
		 * @brief Removes all characters from the string.
		 */
		constexpr void Clear() noexcept
		{
			m_data[0] = '\0';
			m_size = 0;
		}

		/**
		 * @brief Exchanges the contents with another StaticString.
		 *
		 * m_data is a raw char array, so std::swap performs an element-wise
		 * swap here (the same behavior std::array::swap would give).
		 *
		 * @param other String to swap with.
		 */
		constexpr void Swap(StaticString& other) noexcept
		{
			if (this == &other)
			{
				return;
			}

			std::swap(m_data, other.m_data);
			std::swap(m_size, other.m_size);
		}

		// ==========================
		// Case Conversion
		// ==========================

		/**
		 * @brief Converts ASCII letters to lowercase.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& ToLower() noexcept
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				if (m_data[i] >= 'A' && m_data[i] <= 'Z')
				{
					m_data[i] += 'a' - 'A';
				}
			}

			return *this;
		}

		/**
		 * @brief Converts ASCII letters to uppercase.
		 *
		 * @return Reference to this string.
		 */
		constexpr StaticString& ToUpper() noexcept
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				if (m_data[i] >= 'a' && m_data[i] <= 'z')
				{
					m_data[i] -= 'a' - 'A';
				}
			}

			return *this;
		}

		/**
		 * @brief Returns an iterator to the first character.
		 */
		[[nodiscard]] constexpr char* begin() noexcept
		{
			return m_data;
		}

		/**
		 * @brief Returns an iterator to the first character.
		 */
		[[nodiscard]] constexpr const char* begin() const noexcept
		{
			return m_data;
		}

		/**
		 * @brief Returns an iterator one past the last character.
		 */
		[[nodiscard]] constexpr char* end() noexcept
		{
			return m_data + m_size;
		}

		/**
		 * @brief Returns an iterator one past the last character.
		 */
		[[nodiscard]] constexpr const char* end() const noexcept
		{
			return m_data + m_size;
		}

		/**
		 * @brief Returns a const iterator to the first character.
		 */
		[[nodiscard]] constexpr const char* cbegin() const noexcept
		{
			return m_data;
		}

		/**
		 * @brief Returns a const iterator one past the last character.
		 */
		[[nodiscard]] constexpr const char* cend() const noexcept
		{
			return m_data + m_size;
		}

		/// Reverse iterator type.
		using ReverseIterator = std::reverse_iterator<char*>;

		/// Constant reverse iterator type.
		using ConstReverseIterator = std::reverse_iterator<const char*>;

		/**
		 * @brief Returns a reverse iterator to the last character.
		 */
		[[nodiscard]] constexpr ReverseIterator rbegin() noexcept
		{
			return ReverseIterator(end());
		}

		/**
		 * @brief Returns a reverse iterator to the last character.
		 */
		[[nodiscard]] constexpr ConstReverseIterator rbegin() const noexcept
		{
			return ConstReverseIterator(end());
		}

		/**
		 * @brief Returns a reverse iterator one before the first character.
		 */
		[[nodiscard]] constexpr ReverseIterator rend() noexcept
		{
			return ReverseIterator(begin());
		}

		/**
		 * @brief Returns a reverse iterator one before the first character.
		 */
		[[nodiscard]] constexpr ConstReverseIterator rend() const noexcept
		{
			return ConstReverseIterator(begin());
		}

		/**
		 * @brief Returns a const reverse iterator to the last character.
		 */
		[[nodiscard]] constexpr ConstReverseIterator crbegin() const noexcept
		{
			return ConstReverseIterator(cend());
		}

		/**
		 * @brief Returns a const reverse iterator one before the first character.
		 */
		[[nodiscard]] constexpr ConstReverseIterator crend() const noexcept
		{
			return ConstReverseIterator(cbegin());
		}

		/**
		 * @brief Computes a compile-time-friendly FNV-1a hash of the string's contents.
		 *
		 * Unlike std::hash, this is usable directly in constexpr contexts
		 * (e.g. static_assert, compile-time lookup tables).
		 *
		 * @return 64-bit FNV-1a hash of the current contents.
		 */
		[[nodiscard]] constexpr uint64_t HashCode() const noexcept
		{
			uint64_t hash = 1469598103934665603ull; // FNV offset basis

			for (size_t i = 0; i < m_size; ++i)
			{
				hash ^= static_cast<unsigned char>(m_data[i]);
				hash *= 1099511628211ull; // FNV prime
			}

			return hash;
		}

		/// Maximum number of characters excluding the null terminator.
		static constexpr size_t CapacityValue = N - 1;

	private:
		static_assert(N > 0 && N <= 4096, "StaticString size must be 1-4096");

		/**
		 * @brief Assigns the contents of a string view.
		 *
		 * If the source string exceeds the available capacity,
		 * it is truncated and optionally marked with an ellipsis.
		 *
		 * @param sv Source string.
		 */
		constexpr void Assign(std::string_view sv) noexcept
		{
			const size_t len = std::min(sv.size(), CapacityValue);
			const bool truncated = sv.size() > CapacityValue;

			if (std::is_constant_evaluated())
			{
				for (size_t i = 0; i < len; ++i)
				{
					m_data[i] = sv[i];
				}
			}
			else
			{
				if (len > 0)
				{
					std::memcpy(m_data, sv.data(), len);
				}
			}

			if (truncated)
			{
				if (N >= 4)
				{
					m_data[len - 3] = '.';
					m_data[len - 2] = '.';
					m_data[len - 1] = '.';
				}
				else if (N >= 2)
				{
					m_data[len - 1] = '~';
				}
			}

			m_data[len] = '\0';
			m_size = static_cast<uint16_t>(len);
		}

		/**
		 * @brief Appends the decimal textual representation of an integral value.
		 *
		 * @tparam T Integral type.
		 * @param value Value to append.
		 *
		 * @return Reference to this string.
		 */
		template <typename T>
		constexpr StaticString& AppendInteger(T value) noexcept
		{
			static_assert(std::is_integral_v<T>, "AppendInteger requires an integral type");

			char buffer[24];
			size_t pos = sizeof(buffer);

			bool negative = false;
			using UnsignedT = std::make_unsigned_t<T>;
			UnsignedT uvalue;

			if constexpr (std::is_signed_v<T>)
			{
				negative = value < 0;
				uvalue = negative ? static_cast<UnsignedT>(-(value + 1)) + 1 : static_cast<UnsignedT>(value);
			}
			else
			{
				uvalue = value;
			}

			if (uvalue == 0)
			{
				buffer[--pos] = '0';
			}
			else
			{
				while (uvalue > 0)
				{
					buffer[--pos] = static_cast<char>('0' + (uvalue % 10));
					uvalue /= 10;
				}
			}

			if (negative)
			{
				buffer[--pos] = '-';
			}

			return Append(std::string_view(buffer + pos, sizeof(buffer) - pos));
		}

		/// Character storage including the null terminator.
		char m_data[N]{};

		/// Current string length.
		uint16_t m_size = 0;
	};

	/// Default storage type for error messages.
	using MessageString = StaticString<128>;

	/// Default storage type for file system paths.
	using PathString = StaticString<256>;
} // namespace NuEngine::Core

namespace std
{
	/**
	 * @brief Hash specialization for StaticString.
	 *
	 * Allows StaticString to be used as a key in unordered containers.
	 * Delegates to StaticString::HashCode(), an FNV-1a hash that is also
	 * usable directly in constexpr contexts.
	 */
	template <size_t N>
	struct hash<NuEngine::Core::StaticString<N>>
	{
		/**
		 * @brief Computes a hash value for a StaticString.
		 *
		 * @param s String to hash.
		 * @return Hash value.
		 */
		constexpr size_t operator()(const NuEngine::Core::StaticString<N>& s) const noexcept
		{
			return static_cast<size_t>(s.HashCode());
		}
	};
} // namespace std
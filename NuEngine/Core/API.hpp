#pragma once

#ifdef _WIN32
	#ifdef NUENGINE_BUILD_DLL
		#define NU_API __declspec(dllexport)
	#else
		#define NU_API __declspec(dllimport)
	#endif
#else
	#ifdef NUENGINE_BUILD_DLL
		#define NU_API __attribute__((visibility("default")))
	#else
		#define NU_API
	#endif
#endif
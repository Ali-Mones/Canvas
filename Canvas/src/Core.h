#pragma once

#ifdef BUILD_DLL
	#define CANVAS_API __declspec(dllexport)
#else
	#define CANVAS_API __declspec(dllimport)
#endif

#pragma once

#if defined(WIN32)
#ifdef EXPORT_VIS
#define VIS_API __declspec(dllexport)
#else
#define VIS_API __declspec(dllimport)
#endif
#else
#define VIS_API
#endif
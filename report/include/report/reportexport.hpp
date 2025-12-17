#pragma once

#if defined(WIN32)
#ifdef EXPORT_REPORT
#define REPORT_API __declspec(dllexport)
#else
#define REPORT_API __declspec(dllimport)
#endif
#else
#define REPORT_API
#endif
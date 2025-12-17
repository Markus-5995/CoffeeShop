#pragma once

#if defined(WIN32)
#ifdef EXPORT_MSGQ
#define MSGQ_API __declspec(dllexport)
#else
#define MSGQ_API __declspec(dllimport)
#endif
#else
#define MSGQ_API
#endif
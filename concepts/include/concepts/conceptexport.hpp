#pragma once

#if defined(WIN32)
#ifdef EXPORT_CONCEPTS
#define CONCEPTS_API __declspec(dllexport)
#else
#define CONCEPTS_API __declspec(dllimport)
#endif
#else
#define CONCEPTS_API
#endif
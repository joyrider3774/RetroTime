#include <limits.h>

#ifdef WIN32
#include <windows.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#endif
#ifndef PLATFORM_H
#define PLATFORM_H

// Define path separator by OS
#ifdef _WIN32
#define PATH_SEPARATOR   "\\"
#else
#define PATH_SEPARATOR   "/"
#endif

#endif

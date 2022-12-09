#ifndef __APPUTILS_HPP__
#define __APPUTILS_HPP__

typedef int (*APP_FUNC)(int argc, char *argv[]);

int runApplication(int argc, char *argv[], APP_FUNC appFunc);

#endif
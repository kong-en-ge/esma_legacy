#ifndef __APPUTILS_HPP__
#define __APPUTILS_HPP__

typedef int (*APP_FUNC)(int argc, char *argv[]);

int runApplication(int argc, char *argv[], APP_FUNC appFunc);

void setWarningMessages(char const *texts[], int count);
void setErrorMessages(char const *texts[], int count);

void displayError(int code);
void displayWarning(int code);

#endif
#ifndef __APPUTILS_HPP__
#define __APPUTILS_HPP__

#include "coretype.hpp"

void setWarningMessages(char const *texts[], int count);
void setErrorMessages(char const *texts[], int count);

void displayError(int code, char const *text = NULL);
void displayWarning(int code, char const *text = NULL);

bool displaySplashBox(const char * version);
bool confirmExit(void);

#endif
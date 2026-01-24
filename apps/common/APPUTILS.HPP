#ifndef __APPUTILS_HPP__
#define __APPUTILS_HPP__

#include "coretype.hpp"

void setWarningMessages(PCSTR texts[], int count);
void setErrorMessages(PCSTR texts[], int count);

void displayError(int code, PCSTR text = NULL);
void displayWarning(int code, PCSTR text = NULL);

bool displaySplashBox(PCSTR version);
bool confirmExit(void);

#endif
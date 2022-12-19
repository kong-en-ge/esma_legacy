#ifndef __APPUTILS_HPP__
#define __APPUTILS_HPP__

void setWarningMessages(char const *texts[], int count);
void setErrorMessages(char const *texts[], int count);

void displayError(int code);
void displayWarning(int code);

int displaySplashBox(const char * version);
int confirmExit(void);

#endif
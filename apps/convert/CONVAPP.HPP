#ifndef __CONVAPP_HPP__
#define __CONVAPP_HPP__

#include "graphapp.hpp"
#include "errhand.hpp"

class CConverterApplication : public CGraphicApplication
{
private:
  CErrorHandler* onError;

  void convertMapData(void);
  CErrorHandler* getErrorHandler(void) const;
protected:
  virtual char const * getVersion(void) const;
public:
  CConverterApplication(int argc, char *argv[]);
  ~CConverterApplication(void);

  virtual int runUI(void);
};

class CConverterConsoleApplication : public CApplication
{
private:
  int _argc;
  char **_argv;

  int loadClassifier(void);
  void convertClassifier(void);
protected:
  int findCommandLineParam(char const* param, char* value = NULL) const;
public:
  CConverterConsoleApplication(int argc, char *argv[]);

  virtual int run(void);
};

inline CErrorHandler* CConverterApplication::getErrorHandler(void) const
{
  return onError;
}

#endif

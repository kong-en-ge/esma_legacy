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

inline CErrorHandler* CConverterApplication::getErrorHandler(void) const
{
  return onError;
}

#endif

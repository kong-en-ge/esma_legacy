#ifndef __CONVAPP_HPP__
#define __CONVAPP_HPP__

#include "graphapp.hpp"

class CConverterApplication : public CGraphicApplication
{
private:
  void convertMapData(void);
protected:
  virtual char const * getVersion(void) const;
public:
  CConverterApplication(int argc, char *argv[]);

  virtual int runUI(void);
};

#endif

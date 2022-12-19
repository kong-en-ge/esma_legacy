#ifndef __RBMAPAPP_HPP__
#define __RBMAPAPP_HPP__

#include "graphapp.hpp"

class CRbMapApplication : public CGraphicApplication
{
private:
  int initializeApplication(void);
  int loadIndexMap(void);
  int openMapData(void);
protected:
  virtual void executeApp(void);
  virtual int runUI(void);
public:
  CRbMapApplication(int argc, char *argv[]);
};

#endif

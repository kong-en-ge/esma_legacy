#ifndef __INMAPAPP_HPP__
#define __INMAPAPP_HPP__

#include "graphapp.hpp"

#define STATE_SUCCESS 0
#define STATE_LOW_MEMORY 1
#define STATE_NO_MAP 2
#define STATE_FAILED 3

class CInfoMapApplication : public CGraphicApplication
{
private:
  int loadIndexMap(void);
  void loadLibDefs(void);
  int openMapData(void);
protected:
  void displayHelp(void) const;
  virtual char const * getIndexMapFilename(void) const = 0;
  virtual char const * getHelpFilename(void) const = 0;
  virtual char const * getMainMapFilename(void) const = 0;
  virtual int initializeApplication(void);
  virtual void runMainMenu(void) = 0;
public:
  CInfoMapApplication(int argc, char *argv[]);
};

int state2error(int state);

#endif
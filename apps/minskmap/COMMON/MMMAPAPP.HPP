#ifndef __MMMAPAPP_HPP__
#define __MMMAPAPP_HPP__

#include "inmapapp.hpp"
#include "mmmaphn.hpp"

class CMinskMapApplication : public CInfoMapApplication
{
private:
  CMinskMapDataHandler* pMapDataHandler;
protected:
  int argc;
  char ** argv;
  char const* parst;
  int parbl;
  static char const *ston;
  static char const *stoff;

  virtual CMapDataHandler* createMapDataHandler(void);
  virtual void executeApp(void);
  virtual char const * getIndexMapFilename(void) const;
  virtual char const * getHelpFilename(void) const;
  virtual char const * getMainMapFilename(void) const;
  virtual void handleCommand(int command);
  virtual int suppressStartSplash(void) const;
public:
  CMinskMapApplication(int argc, char *argv[]);
};

#endif

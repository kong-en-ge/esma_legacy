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
  PSTR *argv;
  PCSTR parst;
  int parbl;
  static PCSTR ston;
  static PCSTR stoff;

  virtual CMapDataHandler* createMapDataHandler(void);
  virtual void executeApp(void);
  virtual PCSTR getIndexMapFilename(void) const;
  virtual PCSTR getHelpFilename(void) const;
  virtual PCSTR getMainMapFilename(void) const;
  virtual void handleCommand(int command);
  virtual bool suppressStartSplash(void) const;
public:
  CMinskMapApplication(int argc, PSTR argv[]);
};

#endif

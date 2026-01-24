#ifndef __RBMAPAPP_HPP__
#define __RBMAPAPP_HPP__

#include "inmapapp.hpp"

class CRbMapApplication : public CInfoMapApplication
{
protected:
  virtual CMapDataHandler* createMapDataHandler(void);
  virtual void executeApp(void);
  virtual PCSTR getIndexMapFilename(void) const;
  virtual PCSTR getMainMapFilename(void) const;
  virtual PCSTR getHelpFilename(void) const;
  virtual PCSTR getVersion(void) const;
  virtual void handleCommand(int command);
  virtual void runMainMenu(void);
public:
  CRbMapApplication(int argc, PSTR argv[]);
};

#endif

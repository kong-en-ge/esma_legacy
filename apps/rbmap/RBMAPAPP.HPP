#ifndef __RBMAPAPP_HPP__
#define __RBMAPAPP_HPP__

#include "inmapapp.hpp"

class CRbMapApplication : public CInfoMapApplication
{
protected:
  virtual void executeApp(void);
  virtual char const * getIndexMapFilename(void) const;
  virtual char const * getMainMapFilename(void) const;
  virtual char const * getHelpFilename(void) const;
  virtual char const * getVersion(void) const;
  virtual void runMainMenu(void);
public:
  CRbMapApplication(int argc, char *argv[]);
};

#endif

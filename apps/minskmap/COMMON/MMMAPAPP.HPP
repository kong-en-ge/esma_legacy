#ifndef __MMMAPAPP_HPP__
#define __MMMAPAPP_HPP__

#include "inmapapp.hpp"

class CMinskMapApplication : public CInfoMapApplication
{
protected:
  int argc;
  char ** argv;
  char const* parst;
  int parbl;

  virtual void executeApp(void);
  virtual char const * getIndexMapFilename(void) const;
  virtual char const * getHelpFilename(void) const;
  virtual char const * getMainMapFilename(void) const;
public:
  CMinskMapApplication(int argc, char *argv[]);
};

#endif

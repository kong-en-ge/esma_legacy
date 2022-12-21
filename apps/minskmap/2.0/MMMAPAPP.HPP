#ifndef __MMMAPAPP_HPP__
#define __MMMAPAPP_HPP__

#include "inmapapp.hpp"

class CMinskMapApplication : public CInfoMapApplication
{
private:
  int argc;
  char ** argv;
protected:
  virtual void executeApp(void);
  virtual char const * getIndexMapFilename(void) const;
  virtual char const * getMainMapFilename(void) const;
  virtual char const * getVersion(void) const;
  virtual int runUI(void);
public:
  CMinskMapApplication(int argc, char *argv[]);
};

#endif

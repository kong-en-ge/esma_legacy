#ifndef __MMAPP20_HPP__
#define __MMAPP20_HPP__

#include "mmmapapp.hpp"

class CMinskMap20Application : public CMinskMapApplication
{
protected:
  virtual char const * getVersion(void) const;
  virtual void runMainMenu(void);
  virtual int runUI(void);
public:
  CMinskMap20Application(int argc, char *argv[]);
};

#endif

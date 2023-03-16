#ifndef __MMAPP10_HPP__
#define __MMAPP10_HPP__

#include "mmmapapp.hpp"

class CMinskMap10Application : public CMinskMapApplication
{
protected:
  virtual char const * getVersion(void) const;
  virtual void runMainMenu(void);
public:
  CMinskMap10Application(int argc, char *argv[]);
};

#endif

#include "mmapp20.hpp"
#include "softasrt.hpp"

void __localTrap(PCSTR msg, PCSTR cond, PCSTR file, int line)
{
  __assertfail((PSTR)msg, (PSTR)cond, (PSTR)file, line);
  return;
}

int main(int argc, char *argv[])
{
  TRAP_FUNC saveTrap = setAssertTrap(__localTrap);
  
  CApplication* app = new CMinskMap20Application(argc, argv);
  int retValue = app->run();
  delete app;
  
  setAssertTrap(saveTrap);

  return retValue;
}


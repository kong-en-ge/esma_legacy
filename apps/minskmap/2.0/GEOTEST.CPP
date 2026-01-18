#include "mmapp20.hpp"
#include "softasrt.hpp"

void __localTrap(char const *msg, char const *cond, char const *file, int line)
{
  __assertfail((char*)msg, (char*)cond, (char*)file, line);
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


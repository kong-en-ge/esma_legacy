#include "mmapp20.hpp"

extern unsigned _stklen = 10240;

int main(int argc, char *argv[])
{
  CApplication* app = new CMinskMap20Application(argc, argv);
  int retValue = app->run();
  delete app;

  return retValue;
}


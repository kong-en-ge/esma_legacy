#include "mmapp10.hpp"

extern unsigned _stklen = 10240;

int main(int argc, char *argv[])
{
  CApplication* app = new CMinskMap10Application(argc, argv);
  int retValue = app->run();
  delete app;

  return retValue;
}


#include "mmmapapp.hpp"

extern unsigned _stklen = 10240;

int main(int argc, char *argv[])
{
  CApplication* app = new CMinskMapApplication(argc, argv);
  int retValue = app->run();
  delete app;

  return retValue;
}


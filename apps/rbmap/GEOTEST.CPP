#include "rbmapapp.hpp"

int main(int argc, char *argv[])
{
  CApplication* app = new CRbMapApplication(argc, argv);
  int retValue = app->run();
  delete app;

  return retValue;
}

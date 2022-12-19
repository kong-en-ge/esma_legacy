#ifndef __GRAPHAPP_HPP__
#define __GRAPHAPP_HPP__

#include "graphics.h"
#include "app.hpp"

class CGraphicApplication : public CApplication
{
private:
  int _graphicMode;

  int enterGraphicMode(void);
  void leaveGraphicMode(void);
  void setupPalette(void);
protected:
  virtual int runUI(void) = 0;
public:
  CGraphicApplication(int argc, char *argv[]);
  virtual int run(void);

  int isModeEGA(void) const;
};

inline int CGraphicApplication::isModeEGA(void) const
{
  return _graphicMode == EGA;
}


#endif

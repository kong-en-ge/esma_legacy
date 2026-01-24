#ifndef __GRAPHAPP_HPP__
#define __GRAPHAPP_HPP__

#include "coretype.hpp"
#include "graphics.h"
#include "grwnd.hpp"
#include "app.hpp"

class CGraphicApplication : public CApplication
{
private:
  static CGraphicApplication* _instance;
  int _graphicMode;

  bool enterGraphicMode(void);
  void leaveGraphicMode(void);
  void setupPalette(void);
protected:
  GRwnd* createTitleWindow(void);
  virtual PCSTR getVersion(void) const = 0;
  virtual int runUI(void) = 0;
public:
  CGraphicApplication(int argc, PSTR argv[]);
  virtual ~CGraphicApplication(void);

  virtual int run(void);

  bool isModeEGA(void) const;

  static CGraphicApplication* getInstance(void);
};

inline static CGraphicApplication* CGraphicApplication::getInstance(void)
{
  return _instance;
}

inline bool CGraphicApplication::isModeEGA(void) const
{
  return _graphicMode == EGA;
}

#endif

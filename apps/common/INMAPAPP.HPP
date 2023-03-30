#ifndef __INMAPAPP_HPP__
#define __INMAPAPP_HPP__

#include "graphapp.hpp"
#include "button.hpp"
#include "viewctx.hpp"
#include "maphand.hpp"

#define STATE_SUCCESS 0
#define STATE_LOW_MEMORY 1
#define STATE_NO_MAP 2
#define STATE_FAILED 3

class CInfoMapApplication : public CGraphicApplication
{
private:
  Menu_but* activeMenu;
  CViewContext viewContext;

  int loadIndexMap(void);
  void loadLibDefs(void);
  int openMapData(void);
protected:
  button* btnOnOff;

  virtual CMapDataHandler* createMapDataHandler(void) = 0;
  void displayHelp(void) const;
  virtual void executeApp(void) = 0;
  Menu_but* getActiveMenu(void) const;
  virtual char const ** getErrorMessages(void) const;
  virtual int getErrorMessageCount(void) const;
  virtual char const * getIndexMapFilename(void) const = 0;
  virtual char const * getHelpFilename(void) const = 0;
  virtual char const * getMainMapFilename(void) const = 0;
  CViewContext const& getViewContext(void) const;
  virtual char const ** getWarningMessages(void) const;
  virtual int getWarningMessageCount(void) const;
  virtual void handleCommand(int command);
  virtual int initializeApplication(void);
  virtual void runMainMenu(void) = 0;
  void runMenu(Menu_but* menu);
  virtual int runUI(void);
  virtual bool suppressStartSplash(void) const;
public:
  CInfoMapApplication(int argc, char *argv[]);
  virtual ~CInfoMapApplication(void);
};

int state2error(int state);

inline Menu_but* CInfoMapApplication::getActiveMenu(void) const
{
  return activeMenu;
}

inline CViewContext const& CInfoMapApplication::getViewContext(void) const
{
  return viewContext;
}

#endif
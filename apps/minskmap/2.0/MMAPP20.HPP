#ifndef __MMAPP20_HPP__
#define __MMAPP20_HPP__

#include "mmmapapp.hpp"
#include "transp.hpp"

class CMinskMap20Application : public CMinskMapApplication
{
private:
  CTransportModule transportModule;
protected:
  virtual void drawIndexMapOverlay(void) const;
  virtual char const * getVersion(void) const;
  virtual char const ** getWarningMessages(void) const;
  virtual int getWarningMessageCount(void) const;
  virtual void handleCommand(int command);
  virtual int initializeApplication(void);
  virtual void runMainMenu(void);
public:
  CMinskMap20Application(int argc, char *argv[]);
};

#endif

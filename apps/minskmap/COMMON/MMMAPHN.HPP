#ifndef __MMMAPHN_HPP__
#define __MMMAPHN_HPP__

#include "maphand.hpp"

class CMinskMapDataHandler : public CMapDataHandler
{
private:
  int displayNumbers;
public:
  CMinskMapDataHandler(void);

  virtual int draw_one_obj(CViewContext const& viewContext, unsigned obj,
    int color, int drawDisabled);
  int getDisplayNumbers(void) const;
  void setDisplayNumbers(int value);
};

inline int CMinskMapDataHandler::getDisplayNumbers(void) const
{
  return displayNumbers;
}

inline void CMinskMapDataHandler::setDisplayNumbers(int value)
{
  displayNumbers = value;
}

#endif
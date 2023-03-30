#ifndef __MMMAPHN_HPP__
#define __MMMAPHN_HPP__

#include "maphand.hpp"

class CMinskMapDataHandler : public CMapDataHandler
{
private:
  bool displayNumbers;
public:
  CMinskMapDataHandler(void);

  virtual bool draw_one_obj(CViewContext const& viewContext, unsigned obj,
    int color, int drawDisabled);
  bool getDisplayNumbers(void) const;
  void setDisplayNumbers(bool value);
};

inline bool CMinskMapDataHandler::getDisplayNumbers(void) const
{
  return displayNumbers;
}

inline void CMinskMapDataHandler::setDisplayNumbers(bool value)
{
  displayNumbers = value;
}

#endif
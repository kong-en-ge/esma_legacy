#ifndef __FINDOBJ_HPP__
#define __FINDOBJ_HPP__

#include "geometry.hpp"
#include "flibrt.hpp"
#include "viewctx.hpp"

typedef int (*FIND_OBJECT_CLASS_FILTER)(PCSTR code, char loc);

class CCustomFindObjectQuery
{
protected:
  virtual int addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints) = 0;
  void updateThreshold(double maxDistance, int* deltaCommon, int* deltaPoints);
public:
  CViewContext const* viewContext;
  double threshold;
  CPointS pos;
  int ignoreScaleFilter;
  int checkEnabled;
  FIND_OBJECT_CLASS_FILTER filter_proc;

  CCustomFindObjectQuery(void);
  virtual ~CCustomFindObjectQuery(void);

  friend find_objects(CCustomFindObjectQuery&);
};

bool fillQueryPosition(CPointS& pos);

/*
-1: Cancelled
>0: Found object count
*/
int find_objects(CCustomFindObjectQuery& query);

#endif

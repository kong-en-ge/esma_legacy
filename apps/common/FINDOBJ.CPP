#include "files.hpp"
#include "utils.hpp"
#include "globals.hpp"
#include "findobj.hpp"
#include "funcs.hpp"
#include "apputils.hpp"

int find_objects(CCustomFindObjectQuery& query)
{
  int nobj, k;
  F_SPR spr;
  int x1, y1;
  int ret = 0;
  nobj = 0;

  while ((ret = next_model(ret, query.ignoreScaleFilter)) > 0)
  {
    FLIB const* fl = flibrt + models[ret - 1].numkl;
    if (query.filter_proc && !query.filter_proc(fl->clcod, fl->loc))
      continue;

    double D;
    int deltaCommon = DXEKRDIS(query.threshold);
    int deltaPoints = deltaCommon, l;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      if (model.readspr(&spr, i) != 1)
        continue;

      if (!xydregion(query.pos.x, query.pos.y, fl->loc == 1 ? deltaPoints : deltaCommon, (REGION *)&spr.xmin))
        continue;
      if (query.checkEnabled && !obj_enabled(*query.viewContext, &spr))
        continue;
      if (fl->loc == 1)
      {

        if ((D = length(query.pos.x - spr.xmin, query.pos.y - spr.ymin)) < deltaPoints)
          l = D / 4;
        else
          continue;
      }
      else
      {
        int *metr;
        if ((model.readmets((int **)&metr, &spr)) == -1)
          continue;
        if (!findDistanceToPolyline(metr, query.pos.x, query.pos.y, deltaPoints, l))
          continue;
      }
      double dp = DXDISEKR(l);
      nobj = query.addObject(i, dp, fl, &deltaCommon, &deltaPoints);
    } // for
  } // while

  return nobj;
}

/* CCustomFindObjectQuery */

CCustomFindObjectQuery::CCustomFindObjectQuery(void)
{
}

CCustomFindObjectQuery::~CCustomFindObjectQuery(void)
{
}

void CCustomFindObjectQuery::updateThreshold(double maxDistance, int* deltaCommon, int* deltaPoints)
{
  *deltaCommon = DXEKRDIS(maxDistance);
  *deltaPoints = maxDistance < 2.5 ? 4 * *deltaCommon : DXEKRDIS(10);
}

bool fillQueryPosition(CPointS& pos)
{
  double xPos, yPos;
  if (selectObjectPoint(&xPos, &yPos))
  {
    pos.x = XRAZDIS(yPos);
    pos.y = YRAZDIS(xPos);
    return true;
  }
  else
    return false;
}

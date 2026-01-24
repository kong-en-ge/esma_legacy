#include <string.h>
#include <stdio.h>
#include <math.h>

#include "funcs.hpp"
#include "transp.hpp"
#include "utils.hpp"

int twoperes(CTransferStopArray const &transferStops,
  CStopArray const& srcStops, CRouteDefArray &routeDefs,
  CStopArray const& dstStops)
{
  CPointS fromPos, toPos;
  PCSTR mruz1, mruz2;

  ROUTE_DEF routeDef;
  memset(&routeDef, 0, sizeof(routeDef));
  routeDef.distance = 1e20;
  routeDef.stageCount = 4;

  for (int iv = 0; iv < transferStops.getCount(); iv++)
  { //
    if (!calcCommonRoutes(transferStops[iv], srcStops, routeDef.stages[0], fromPos))
      continue;

    float distanceFrom =  hypot(transferStops[iv].pos, fromPos);

    for (int ivk = 0; ivk < transferStops.getCount(); ivk++)
    { // two
     if (!calcCommonRoutes(transferStops[ivk], dstStops, routeDef.stages[3], toPos))
       continue;

      float distanceTo = hypot(transferStops[ivk].pos, toPos);
      routeDef.stages[1].routes[0] = 0;
      for (int kk = 1; kk <= 4; kk++)
      { // for4
        mruz1 = routesByType(transferStops[iv], kk);
        mruz2 = routesByType(transferStops[ivk], kk);
        if (kk == 1)
        {
          if (*mruz1 && *mruz2)
            strcat(routeDef.stages[1].routes, "*");
        }
        if (kk != 1)
          composeCommonRoutes(kk, mruz1, mruz2, routeDef.stages[1].routes, NULL, NULL);
      } // for4

      if (!routeDef.stages[1].routes[0])
        continue;

      routeDef.distance = distanceFrom + distanceTo +
        hypot(transferStops[iv].pos, transferStops[ivk].pos);
      strcpy(routeDef.stages[1].stopTitle, transferStops[iv].title);
      strcpy(routeDef.stages[2].stopTitle, transferStops[ivk].title);
      routeDefs.insertOrdered(routeDef, MAX_ROUTE_DEFS);
    }
  }
  return !routeDefs.isEmpty();
}
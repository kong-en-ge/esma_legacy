#include <string.h>
#include <values.h>
#include <stdio.h>
#include <math.h>

#include "apputils.hpp"
#include "funcs.hpp"
#include "transp.hpp"
#include "utils.hpp"

bool three(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CRouteDefArray &routeDefs,
  CStopArray const& dstStops)
{
  CPointS fromPos, toPos;
  PCSTR mruz1, mruz2, mruz3;

  ROUTE_DEF routeDef;
  memset(&routeDef, 0, sizeof(routeDef));
  routeDef.distance = MAXFLOAT;
  routeDef.stageCount = 5;

  for (int iv = 0; iv < transferStops.getCount(); iv++)
  { //
    if (!calcCommonRoutes(transferStops[iv], srcStops, routeDef.stages[0], fromPos))
      continue;

    for (int ivk = 0; ivk < transferStops.getCount(); ivk++)
    { // two
     if (!calcCommonRoutes(transferStops[ivk], dstStops, routeDef.stages[4], toPos))
       continue;

      float distanceFrom = hypot(transferStops[iv].pos, fromPos);

      PCSTR srcRoutes, srcRoutes2;
      for (int ivs = 0; ivs < transferStops.getCount(); ivs++)
      { // for5
        float distanceTo = hypot(transferStops[ivk].pos, toPos);

        routeDef.stages[1].routes[0] = 0;
        routeDef.stages[3].routes[0] = 0;
        for (int kk = 1; kk <= 4; kk++)
        { // for4
          mruz1 = routesByType(transferStops[iv], kk);
          mruz2 = routesByType(transferStops[ivk], kk);
          mruz3 = routesByType(transferStops[ivs], kk);
          if (kk == 1)
          {
            if (*mruz1 && *mruz3)
              strcat(routeDef.stages[1].routes, "*");
            if (*mruz2 && *mruz3)
              strcat(routeDef.stages[3].routes, "*");
          }
          else
          { // 1
            composeCommonRoutes(kk, mruz1, mruz3,
              routeDef.stages[1].routes, NULL, NULL);
            composeCommonRoutes(kk, mruz2, mruz3,
              routeDef.stages[3].routes, NULL, NULL);
          }       // 1
          if (!routeDef.stages[1].routes[0] && !routeDef.stages[3].routes[0])
            continue;

          routeDef.distance = distanceFrom + distanceTo +
            hypot(transferStops[iv].pos, transferStops[ivs].pos) +
            hypot(transferStops[ivs].pos, transferStops[ivk].pos);

          strcpy(routeDef.stages[1].stopTitle, transferStops[iv].title);
          strcpy(routeDef.stages[2].stopTitle, transferStops[ivs].title);
          strcpy(routeDef.stages[3].stopTitle, transferStops[ivk].title);
          routeDefs.insertOrdered(routeDef, MAX_ROUTE_DEFS);
        }
      }
    }
  }
  return !routeDefs.isEmpty();
}
#include <math.h>
#include <string.h>

#include "coreutls.hpp"
#include "apputils.hpp"
#include "funcs.hpp"
#include "transp.hpp"

bool calcSubwayTransfer(CViewContext const& viewContext,
  CRouteDefArray &routeDefs, CStopArray const& dstStops, int subwayObj,
  CSubwayStationArray const& subwayStations, bool fromSubway)
{
  ROUTE_DEF routeDef;

  CStopArray stops, transferStops, targetStops;
  CPointS dstStopPos;
  int num, stopIndex;
  char srcRoutes[55], dstRoutes[55];

  int subwayIndex = -1;

  for (int i = 0; i < subwayStations.getCount(); i++)
    if (subwayStations[i].obj == subwayObj)
    {
      subwayIndex = i;
      break;
    }

  if (subwayIndex < 0)
  {
    displayError(1);
    return false;
  }
  CPointS const &pos = subwayStations[subwayIndex].pos;

  memset(&routeDef, 0, sizeof(routeDef));
  routeDef.stageCount = 4;
  strcpy(routeDef.stages[0].stopTitle, subwayStations[subwayIndex].title);

  getNearestStopsByType(targetStops, dstStops);

  for (int iv = 0; iv < subwayStations.getCount(); iv++)
  { //
    find_transport_stops_at_pos(viewContext, subwayStations[iv].pos, stops);

    for (int i = 0; i < stops.getCount(); i++)
      if (stops[i].vt != 1) // subway2subway case already covered earlier
        transferStops.add(stops[i]);

    for (int iSrc = 0; iSrc < transferStops.getCount(); iSrc++)
    {
      int type = transferStops[iSrc].vt;

      if (!fetchStopData(type, transferStops[iSrc].nobj, routeDef.stages[2].stopTitle, srcRoutes))
        continue;

      if (!routeDef.stages[2].stopTitle)
        continue;

      for (int iDst = 0; iDst < targetStops.getCount(); iDst++)
      {
        if (targetStops[iDst].vt != type)
          continue;

        if (!fetchStopData(type, targetStops[iDst].nobj, routeDef.stages[3].stopTitle,
          dstRoutes, &dstStopPos))
          continue;

        routeDef.stages[3].routes[0] = 0;
        if (!composeCommonRoutes(type, srcRoutes, dstRoutes, routeDef.stages[3].routes, NULL, NULL))
          continue;

        routeDef.distance = hypot(subwayStations[iv].pos, dstStopPos) +
          hypot(subwayStations[iv].pos, pos);

        int js;
        for (js = 0; js < routeDefs.getCount(); js++)
        {
          ROUTE_DEF const &item = routeDefs[js];

          if (!strcmp(item.stages[3].stopTitle, routeDef.stages[3].stopTitle) &&
              !strcmp(item.stages[3].routes, routeDef.stages[3].routes) &&
              routeDef.distance > item.distance)
            break;
        }

        if (js < routeDefs.getCount())
          continue;

        strcpy(routeDef.stages[1].stopTitle, subwayStations[iv].title);
        routeDef.type = type;

        if (!fromSubway)
          reverseArray(routeDef.stages, sizeof(ROUTE_STAGE), 4);

        routeDefs.insertOrdered(routeDef, MAX_ROUTE_DEFS);
      }
    }
  }

  return true;
}
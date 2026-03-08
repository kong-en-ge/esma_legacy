#include <math.h>
#include <string.h>

#include "coreutls.hpp"
#include "transp.hpp"

#define TRANSFER_SCALE 2

bool calcSubwayTransfer(CViewContext const& viewContext,
  CRouteDefArray &routeDefs, CStopArray const& dstStops, int subwayObj,
  CSubwayStationArray const& subwayStations, bool fromSubway)
{
  ROUTE_DEF routeDef(4);

  CStopArray stops, transferStops, targetStops;
  CPointS srcStopPos, dstStopPos;
  int num, stopIndex;
  char subwayTitle[40], srcRoutes[55], dstRoutes[55];

  if (!fetchStopData(1, subwayObj, subwayTitle, NULL, NULL))
     return false;

  int subwayIndex = -1;

  for (int i = 0; i < subwayStations.getCount(); i++)
    if (!strcmp(subwayStations[i].title, subwayTitle))
    {
      subwayIndex = i;
      break;
    }

  if (subwayIndex < 0)
    return false;

  CPointS const &pos = subwayStations[subwayIndex].pos;

  strcpy(routeDef.stages[0].stopTitle, subwayStations[subwayIndex].title);

  getNearestStopsByType(targetStops, dstStops);

  for (int iv = 0; iv < subwayStations.getCount(); iv++)
  { //
    find_transport_stops_at_pos(viewContext, subwayStations[iv].pos, stops);

    for (int i = 0; i < stops.getCount(); i++)
      if (stops[i].type != 1) // subway2subway case already covered earlier
        transferStops.add(stops[i]);

    for (int iSrc = 0; iSrc < transferStops.getCount(); iSrc++)
    {
      int type = transferStops[iSrc].type;

      if (!fetchStopData(type, transferStops[iSrc].obj, routeDef.stages[2].stopTitle,
        srcRoutes, &srcStopPos))
        continue;

      if (!routeDef.stages[2].stopTitle)
        continue;

      for (int iDst = 0; iDst < targetStops.getCount(); iDst++)
      {
        if (targetStops[iDst].type != type)
          continue;

        if (!fetchStopData(type, targetStops[iDst].obj, routeDef.stages[3].stopTitle,
          dstRoutes, &dstStopPos))
          continue;

        routeDef.stages[3].routes[0] = 0;
        if (!composeCommonRoutes(type, srcRoutes, dstRoutes, routeDef.stages[3].routes, NULL, NULL))
          continue;

        routeDef.distance = hypot(pos, subwayStations[iv].pos) +
          hypot(subwayStations[iv].pos, srcStopPos) * TRANSFER_SCALE +
          hypot(srcStopPos, dstStopPos);

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
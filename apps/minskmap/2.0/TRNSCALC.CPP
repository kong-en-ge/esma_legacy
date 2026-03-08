#include <string.h>
#include <math.h>

#include "transp.hpp"

bool findCommonRoutes(TRANSFER_STOP const & transferStop,
  CStopArray const &srcStops, char *title, char *routes, CPointS &pos)
{
  char stopTitle[40], stopRoutes[55];
  CPointS stopPos;

  *routes = 0;
  pos.x = UNDEFINED;
  pos.y = UNDEFINED;

  for (int type = 2/*1*/; type <= 4; type++)
  {
    CStopArray nearestStops;
    getNearestStopsByType(nearestStops, srcStops);

    PCSTR transerStopRoutes = routesByType(transferStop, type);

    for (int i = 0; i < nearestStops.getCount(); i++)
    {
      OBSTOP const &stop = nearestStops[i];
      if (stop.type != type)
        continue;

      if (!fetchStopData(type, stop.obj, stopTitle, stopRoutes, &stopPos))
        continue;

      if (composeCommonRoutes(type, stopRoutes, transerStopRoutes,
        routes, &pos, &stopPos))
        strcpy(title, stopTitle);
    }
  }

  return *routes;
}

bool calcCommonTransfer(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CStopArray const& dstStops,
  CRouteDefArray &routeDefs)
{
  CPointS fromPos, toPos;
  ROUTE_DEF routeDef(3);

  for (int i = 0; i < transferStops.getCount(); i++)
  {
    TRANSFER_STOP const& stop = transferStops[i];
    if (!findCommonRoutes(stop, srcStops, routeDef.stages[0].stopTitle,
      routeDef.stages[0].routes, fromPos))
      continue;

    if (!findCommonRoutes(stop, dstStops, routeDef.stages[2].stopTitle,
      routeDef.stages[2].routes, toPos))
      continue;


    float distance = hypot(stop.pos, fromPos) + hypot(stop.pos, toPos);
    for (int j = 0; j < routeDefs.getCount(); j++)
      if (distance < routeDefs[j].distance)
        break;

    strcpy(routeDef.stages[1].stopTitle, stop.title);
    routeDef.distance = distance;

    routeDefs.insert(j, routeDef);
  }

  return !routeDefs.isEmpty();
}

bool calcTwoTransfers(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CStopArray const& dstStops, 
  CRouteDefArray &routeDefs)
{
  CPointS fromPos, toPos;
  ROUTE_DEF routeDef(4);

  for (int t1 = 0; t1 < transferStops.getCount(); t1++)
  {
    TRANSFER_STOP const &transfer1 = transferStops[t1];

    if (!calcCommonRoutes(transfer1, srcStops, routeDef.stages[0], fromPos))
      continue;

    for (int t2 = 0; t2 < transferStops.getCount(); t2++)
    {
      TRANSFER_STOP const &transfer2 = transferStops[t2];

     if (!calcCommonRoutes(transfer2, dstStops, routeDef.stages[3], toPos))
       continue;

      if (!composeTransferStage(transfer1, transfer2, routeDef.stages[1]))
        continue;

      routeDef.distance =
        hypot(fromPos, transfer1.pos) +
        hypot(transfer1.pos, transfer2.pos) +
        hypot(transfer2.pos, toPos);
      strcpy(routeDef.stages[1].stopTitle, transfer1.title);
      strcpy(routeDef.stages[2].stopTitle, transfer2.title);
      routeDefs.insertOrdered(routeDef, MAX_ROUTE_DEFS);
    }
  }
  return !routeDefs.isEmpty();
}

bool calcThreeTransfers(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CStopArray const& dstStops, 
  CRouteDefArray &routeDefs)
{
  CPointS fromPos, toPos;
  ROUTE_DEF routeDef(5);

  for (int t1 = 0; t1 < transferStops.getCount(); t1++)
  { //
    TRANSFER_STOP const &transfer1 = transferStops[t1];
    if (!calcCommonRoutes(transfer1, srcStops, routeDef.stages[0], fromPos))
      continue;

    for (int t3 = 0; t3 < transferStops.getCount(); t3++)
    {
      TRANSFER_STOP const &transfer3 = transferStops[t3];
      if (!calcCommonRoutes(transfer3, dstStops, routeDef.stages[4], toPos))
        continue;

      for (int t2 = 0; t2 < transferStops.getCount(); t2++)
      {
        TRANSFER_STOP const &transfer2 = transferStops[t2];

        if (!composeTransferStage(transfer1, transfer2, routeDef.stages[1]))
          continue;

        if (!composeTransferStage(transfer2, transfer3, routeDef.stages[3]))
          continue;

        routeDef.distance =
          hypot(fromPos, transfer1.pos) +
          hypot(transfer1.pos, transfer2.pos) +
          hypot(transfer2.pos, transfer3.pos);
          hypot(transfer3.pos, toPos);

        strcpy(routeDef.stages[1].stopTitle, transfer1.title);
        strcpy(routeDef.stages[2].stopTitle, transfer2.title);
        strcpy(routeDef.stages[3].stopTitle, transfer3.title);
        routeDefs.insertOrdered(routeDef, MAX_ROUTE_DEFS);
      }
    }
  }
  return !routeDefs.isEmpty();
}

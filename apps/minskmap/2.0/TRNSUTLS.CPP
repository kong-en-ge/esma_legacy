#include <stdio.h>
#include <string.h>

#include "funcs.hpp"
#include "transp.hpp"

void filterRouteEndingStops(CStopArray const& input, CStopArray& output)
{
  int subwayIndex = -1;
  float minDistance = 1000000.0;
  for(int i = 0; i < input.getCount(); i++)
  {
    if (input[i].vt == 1)
    {
      if (input[i].d < minDistance)
      {
        subwayIndex = i;
        minDistance = input[i].d;
      }
    }
    else
      output.add(input[i]);
  }

  if (subwayIndex >= 0)
    output.insert(0, input[subwayIndex]);
}

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

    char const *transerStopRoutes = routesByType(transferStop, type);

    for (int i = 0; i < nearestStops.getCount(); i++)
    {
      OBSTOP const &stop = nearestStops[i];
      if (stop.vt != type)
        continue;

      if (!fetchStopData(type, stop.nobj, stopTitle, stopRoutes, &stopPos))
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
  int i, j;
  ROUTE_DEF routeDef;
  CPointS fromPos, toPos;

  memset(&routeDef, 0, sizeof(routeDef));
  routeDef.stageCount = 3;

  for (i = 0; i < transferStops.getCount(); i++)
  {
    TRANSFER_STOP const& stop = transferStops[i];
    if (!findCommonRoutes(stop, srcStops, routeDef.stages[0].stopTitle,
      routeDef.stages[0].routes, fromPos))
      continue;

    if (!findCommonRoutes(stop, dstStops, routeDef.stages[2].stopTitle,
      routeDef.stages[2].routes, toPos))
      continue;


    float distance = length(stop.pos, fromPos) + length(stop.pos, toPos);
    for (j = 0; j < routeDefs.getCount(); j++)
      if (distance < routeDefs[j].distance)
        break;

    strcpy(routeDef.stages[1].stopTitle, stop.title);
    routeDef.distance = distance;

    routeDefs.insert(j, routeDef);
  }

  return !routeDefs.isEmpty();
}

typedef struct
{
  bool subway;
  char bus[55];
  char trolleybus[55];
  char tram[55];
} ROUTES;


void parseRouteData(char const* src, ROUTES& routes)
{
  char* dst;

  memset(&routes, 0, sizeof(ROUTES));
  if (*src == '*')
  {
    routes.subway = true;
    src++;
  }

  while (*src)
  {
    switch(*src++)
    {
      case '%':
        dst = routes.bus;
        break;
      case '@':
        dst = routes.trolleybus;
        break;
      case '&':
        dst = routes.tram;
        break;

      default: ;
    }

    while (*src && *src != '%' && *src != '@' && *src != '&')
      *dst++ = *src++;

    *dst = 0;
  }
}

void putRouteData(TEXT_PACK_MENUS *tp, ROUTES const& routes)
{
  char str[100];

  if (routes.subway)
  {
    tp->add("На метро ");
    if (routes.bus[0] || routes.trolleybus[0] || routes.tram[0])
      tp->add("или");
  }
  if (routes.bus[0])
  {
    sprintf(str, "Автобусами N %s", routes.bus);
    tp->add(str);
  }
  if (routes.trolleybus[0])
  {
    sprintf(str, "Троллейбусами N %s", routes.trolleybus);
    tp->add(str);
  }
  if (routes.tram[0])
  {
    sprintf(str, "Трамваями N %s", routes.tram);
    tp->add(str);
  }
}

void processRouteData(TEXT_PACK_MENUS *tp, char const* src)
{
  ROUTES routes;
  parseRouteData(src, routes);
  putRouteData(tp, routes);
}

void copyAttributeValue(mk_atrf& attributes, const char* attrName, char* dst)
{
  if (checkAttributeDef(attributes.natr, attrName, 'T'))
    strcpy(dst, attributes.zns);
}

bool fetchStopData(int type, int obj, char *title, char *routes,
  CPointS *pos)
{
  F_SPR spr;
  mk_atrf attributes;
  int status;

  if (title)
    *title = 0;
  if (routes)
    *routes = 0;

  if (!loadAttributes(obj, attributes, &spr))
    return false;

  if (pos)
  {
    pos->x = spr.xmin;
    pos->y = spr.ymin;
  }

  do
  {
    status = make_atr(&attributes);
    if (type != 1 && routes != NULL)
      copyAttributeValue(attributes, "NM", routes);
    copyAttributeValue(attributes, "09", title);
  } while (!status);

  return true;
}

int findNearestStop(CStopArray const& stops, int type)
{
  int index = -1;

  for(int i = 0; i < stops.getCount(); i++)
    if (stops[i].vt == type &&
      (index < 0 || stops[i].d < stops[index].d))
      index = i;

  return index;
}

void getNearestStopsByType(CStopArray &dstStops, CStopArray const& srcStops)
{
  int index;
  for (int i = 1; i <= 4; i++)
  {
    index = findNearestStop(srcStops, i);
    if (index < 0)
      continue;

    dstStops.add(srcStops[index]);
  }
}

const char* copyRouteStr(char* dst, char const* src)
{
  while (*src && *src != ',' && *src != '.')
    *dst++ = *src++;
  *dst = 0;

  if (*src == ',' || *src == '.')
    src++;

  return src;
}

bool getCommonRoutes(char* dst, char const* src1, char const* src2)
{
  char const* src2Data;
  char item1[10], item2[10];

  *dst = 0;

  while (*src1)
  {
    src1 = copyRouteStr(item1, src1);

    src2Data = src2;
    while (*src2Data)
    {
      src2Data = copyRouteStr(item2, src2Data);
      if ((strcmp(item1, item2)) == 0)
      {
        if (*dst)
          strcat(dst, ",");
        strcat(dst, item1);
      }
    }
  }

  return *dst;
}

bool composeCommonRoutes(int type, char const *src1, char const *src2,
  char *dst, CPointS *dstPos, CPointS const *srcPos)
{
  if (!(*src1 && *src2))
    return false;

  bool isFirst = true;
  char str1[5], str2[5];
  char const *routes1, *routes2;

  routes1 = src1;
  while (*routes1)
  {
    routes1 = copyRouteStr(str1, routes1);
    routes2 = src2;
    while (*routes2)
    {
      routes2 = copyRouteStr(str2, routes2);

      if (strcmp(str1, str2))
        continue;

      if (isFirst)
      {
        if (type == 2)
          strcat(dst, "%");
        else if (type == 3)
          strcat(dst, "@");
        else if (type == 4)
          strcat(dst, "&");

        isFirst = false;
      }
      else
        strcat(dst, ",");

      strcat(dst, str1);

      if (dstPos && srcPos)
       if (dstPos->x == UNDEFINED && dstPos->y == UNDEFINED)
       {
         *dstPos = *srcPos;
       }
       else
       {
         dstPos->x = dstPos->x / 2 + srcPos->x / 2;
         dstPos->y = dstPos->y / 2 + srcPos->y / 2;
       }
    }
  }

  return *dst;
}

bool calcCommonRoutes(TRANSFER_STOP const& transferStop,
  CStopArray const &stops, ROUTE_STAGE &routeStage, CPointS &pos)
{
  char const *transferRoutes;
  char stopRoutes[55];
  char stopTitle[55];
  CPointS stopPos;

  routeStage.stopTitle[0] = 0;
  routeStage.routes[0] = 0;

  pos.x = UNDEFINED;
  pos.y = UNDEFINED;

  for (int i = 0; i < stops.getCount(); i++)
  {
    transferRoutes = routesByType(transferStop, stops[i].vt);

    if (!fetchStopData(stops[i].vt, stops[i].nobj, stopTitle, stopRoutes, &stopPos))
      continue;

    if (composeCommonRoutes(stops[i].vt, transferRoutes, stopRoutes,
      routeStage.routes, &pos, &stopPos) && !routeStage.stopTitle[0])
      strcpy(routeStage.stopTitle, stopTitle);
  }

  return routeStage.routes[0];
}

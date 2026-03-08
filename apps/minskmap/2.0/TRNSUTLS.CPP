#include <stdio.h>
#include <string.h>

#include "funcs.hpp"
#include "transp.hpp"

bool composeTransferStage(TRANSFER_STOP const &stop1,
  TRANSFER_STOP const &stop2, ROUTE_STAGE &stage)
{
  stage.routes[0] = 0;
  for (int type = 1; type <= 4; type++)
  {
    PCSTR routes1 = routesByType(stop1, type);
    PCSTR routes2 = routesByType(stop2, type);
    if (type == 1)
    {
      if (*routes1 && *routes2)
        strcat(stage.routes, "*");
    }
    else
      composeCommonRoutes(type, routes1, routes2, stage.routes, NULL, NULL);
  }

  return stage.routes[0];
}

typedef struct
{
  bool subway;
  char bus[55];
  char trolleybus[55];
  char tram[55];
} ROUTES;

void parseRouteData(PCSTR src, ROUTES& routes)
{
  PSTR dst;

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

void processRouteData(TEXT_PACK_MENUS *tp, PCSTR src)
{
  ROUTES routes;
  parseRouteData(src, routes);
  putRouteData(tp, routes);
}

void copyAttributeValue(mk_atrf& attributes, PCSTR attrName, PSTR dst)
{
  if (checkAttributeDef(attributes.natr, attrName, 'T'))
    strcpy(dst, attributes.zns);
}

bool fetchStopData(int type, int obj, PSTR title, PSTR routes,
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
    if (stops[i].type == type &&
      (index < 0 || stops[i].distance < stops[index].distance))
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

PCSTR copyRouteStr(PSTR dst, PCSTR src)
{
  while (*src && *src != ',' && *src != '.')
    *dst++ = *src++;
  *dst = 0;

  if (*src == ',' || *src == '.')
    src++;

  return src;
}

bool getCommonRoutes(PSTR dst, PCSTR src1, PCSTR src2)
{
  PCSTR src2Data;
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

bool composeCommonRoutes(int type, PCSTR src1, PCSTR src2,
  PSTR dst, CPointS *dstPos, CPointS const *srcPos)
{
  if (!(*src1 && *src2))
    return false;

  bool isFirst = true;
  char str1[5], str2[5];
  PCSTR routes1, routes2;

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
  PCSTR transferRoutes;
  char stopRoutes[55];
  char stopTitle[55];
  CPointS stopPos;

  routeStage.stopTitle[0] = 0;
  routeStage.routes[0] = 0;

  pos.x = UNDEFINED;
  pos.y = UNDEFINED;

  for (int i = 0; i < stops.getCount(); i++)
  {
    transferRoutes = routesByType(transferStop, stops[i].type);

    if (!fetchStopData(stops[i].type, stops[i].obj, stopTitle, stopRoutes, &stopPos))
      continue;

    if (composeCommonRoutes(stops[i].type, transferRoutes, stopRoutes,
      routeStage.routes, &pos, &stopPos) && !routeStage.stopTitle[0])
      strcpy(routeStage.stopTitle, stopTitle);
  }

  return routeStage.routes[0];
}

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "coretype.hpp"
#include "utils.hpp"
#include "funcs.hpp"
#include "transp.hpp"
#include "findobj.hpp"
#include "apputils.hpp"
#include "globals.hpp"
#include "inmapapp.hpp"
#include "extmath.hpp"
#include "transp.hpp"

/*const*/ char szWalkRoute[] = "     ПЕШКОМ!   ";

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

  if (subwayIndex >= 0) // put subway first
    output.insert(0, input[subwayIndex]);
}

bool queryPointAndStops(CViewContext const &viewContext, CPointS &pos, CStopArray &stops)
{
  CStopArray tempStops;

  stops.clear();
  bool retValue = find_transport_stops(viewContext, tempStops, pos);

  if (retValue)
    filterRouteEndingStops(tempStops, stops);

  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);

  return retValue;
}

/* CRouteDefArray */

bool CRouteDefArray::insertOrdered(ROUTE_DEF const &routeDef,
  unsigned maxCount)
{
  int index;
  for (index = 0; index < getCount(); index++)
    if ((*this)[index].distance > routeDef.distance)
      break;

  if (index < maxCount || getCount() < maxCount)
  {
    insert(index, routeDef);

    while (getCount() > maxCount)
      removeLast();

    return true;
  }
  else
    return false;
}

class CWalkRouteDescription : public CCustomRouteDescription
{
public:
  virtual void fillResultWindow(TEXT_PACK_MENUS *tp) const;
};

class CDirectRouteDescription : public CCustomRouteDescription
{
private:
  CRouteDefArray &items;
public:
  CDirectRouteDescription(CRouteDefArray &items);
  virtual ~CDirectRouteDescription(void);

  virtual void fillResultWindow(TEXT_PACK_MENUS *tp) const;
};

class CRouteDescription : public CCustomRouteDescription
{
private:
  CRouteDefArray const &routeDefs;
public:
  CRouteDescription(CRouteDefArray const &routeDefs);
  virtual ~CRouteDescription(void);

  virtual void fillResultWindow(TEXT_PACK_MENUS *tp) const;
};

class CSingleTransferRouteDescription : public CCustomRouteDescription
{
public:
  virtual void fillResultWindow(TEXT_PACK_MENUS *tp) const;
};

/* CCustomRouteDescription */

CCustomRouteDescription::~CCustomRouteDescription(void)
{
}

/* CWalkRouteDescription */

void CWalkRouteDescription::fillResultWindow(TEXT_PACK_MENUS *tp) const
{
  tp->add(szWalkRoute);
}

/* CDirectRouteDescription */

CDirectRouteDescription::CDirectRouteDescription(CRouteDefArray &items) :
  items(items)
{
}

CDirectRouteDescription::~CDirectRouteDescription()
{
  delete &items;
}

void CDirectRouteDescription::fillResultWindow(TEXT_PACK_MENUS *tp) const
{
  static PCSTR typeTitles[] =
  {
    "Метро",
    "Автобусами",
    "Троллейбусами",
    "Трамваями"
  };

  char text[80];
  bool isFirstLine;
  if (!items.isEmpty())
  { // yes
    for(int k = 1; k <= 4; k++)
    {
      PCSTR stopType = k == 1 ? "ст." : "ост.";
      isFirstLine = true;
      for (int i = 0; i < items.getCount(); i++)
      {
        ROUTE_DEF const& item = items[i];
        if (item.type != k)
          continue;

        if (isFirstLine)
        {
          sprintf(text, "   %s:", typeTitles[k - 1]);
          tp->add(text);
          isFirstLine = false;
        }
        sprintf(text, "От %s %s", stopType, item.stages[0].stopTitle);
        tp->add(text);
        if (k != 1)
        {
          sprintf(text, "N %s", item.stages[0].routes);
          tp->add(text);
        }
        sprintf(text, "до %s %s", stopType, item.stages[1].stopTitle);
        tp->add(text);
      }
    }
  }
  else
    tp->add(szWalkRoute);
}

/* CSingleTransferRouteDescription */

void CSingleTransferRouteDescription::fillResultWindow(TEXT_PACK_MENUS *tp) const
{
/*
  if ((j2 != 0) || (j3 != 0))
  { // 1 peresad
    if (j2 != 0)
    {
      if (hasFromSubway)
        ATRMT(&tp, ob);
      else if (hasToSubway)
        ATRTM(*this, &tp, ob);
      viewContext.updateViewPort();
    }
    if (j3 != 0)
    {
      if (j2 != 0)
        tp.add("         ИЛИ        ");
      ATRPER(&tp, ob3);
      delete[] psu;
      viewContext.updateViewPort();
    }
  }
*/
}

/* CRouteDescription */

CRouteDescription::CRouteDescription(CRouteDefArray const &routeDefs) :
  routeDefs(routeDefs)
{
}

CRouteDescription::~CRouteDescription()
{
//  delete &routeDefs;
}

void CRouteDescription::fillResultWindow(TEXT_PACK_MENUS *tp) const
{
  static PCSTR typeTitles[] =
  {
    "Метро",
    "Автобусами",
    "Троллейбусами",
    "Трамваями"
  };

  for (int i = 0; i < routeDefs.getCount(); i++)
  {
    ROUTE_DEF const& item = routeDefs[i];
    for (int j = 0; j < 5; j++)
    {
      if (!item.stages[j].stopTitle[0])
        break;

      tp->add((char*)item.stages[j].stopTitle);
      if (item.stages[j].routes[0])
      {
        if (item.type >= 2 && item.type <=4)
          tp->add((char*)typeTitles[item.type - 1]);
        tp->add((char*)item.stages[j].routes);
      }
//      else
//        tp->add("SUBWAY");
    }
    tp->add("***");
  }
/*

  char text[80];
  bool isFirstLine;
  if (!items.isEmpty())
  { // yes
    for(int k = 1; k <= 4; k++)
    {
      PCSTR stopType = k == 1 ? "ст." : "ост.";
      isFirstLine = true;
      for (int i = 0; i < items.getCount(); i++)
      {
        ROUTE_DEF const& item = items[i];
        if (item.type != k)
          continue;

        if (isFirstLine)
        {
          sprintf(text, "   %s:", typeTitles[k - 1]);
          tp->add(text);
          isFirstLine = false;
        }
        sprintf(text, "От %s %s", stopType, item.stages[0].stopTitle);
        tp->add(text);
        if (k != 1)
        {
          sprintf(text, "N %s", item.stages[0].routes);
          tp->add(text);
        }
        sprintf(text, "до %s %s", stopType, item.stages[1].stopTitle);
        tp->add(text);
      }
    }
  }
  else
    tp->add(szWalkRoute);
*/
}

/* CTransportModule */

CTransportModule::CTransportModule(void) :
  hasSourcePos(false)
{
}

int compareDirectRoutes(ROUTE_DEF const &item1, ROUTE_DEF const &item2)
{
  int ret = item1.type - item2.type;
  if (ret)
    return ret;

  return sign(item1.distance - item2.distance);
}

bool CTransportModule::detectDirectRoute(CStopArray const &targetStops,
  CRouteDefArray &routeDefs)
{
  int i;
  char sourceRoutes[100], targetRoutes[100];
  ROUTE_DEF routeDef;

  memset(&routeDef, 0, sizeof(routeDef));
  routeDef.stageCount = 2;

  for (i = 0; i < sourceStops.getCount(); i++)
  {
    routeDef.type = sourceStops[i].vt;
    if (!fetchStopData(routeDef.type, sourceStops[i].nobj, routeDef.stages[0].stopTitle,
      sourceRoutes))
      continue;

    if (!routeDef.stages[0].stopTitle[0])
      continue;

    if (routeDef.type != 1 && !sourceRoutes[0])
      continue;

    for (int j = 0; j < targetStops.getCount(); j++)
    {
      if (targetStops[j].vt != routeDef.type)
        continue;

      if (!fetchStopData(routeDef.type, targetStops[j].nobj,
        routeDef.stages[1].stopTitle, targetRoutes))
        continue;

      if (!routeDef.stages[1].stopTitle[0])
        continue;

      if (!strcmp(routeDef.stages[0].stopTitle, routeDef.stages[1].stopTitle))
        continue;

      routeDef.distance = sourceStops[i].d + targetStops[j].d;

      if (routeDef.type == 1 ||
          getCommonRoutes(routeDef.stages[0].routes, sourceRoutes, targetRoutes))
        routeDefs.add(routeDef);
    }
  }

  routeDefs.qsort(&compareDirectRoutes);

  for (i = routeDefs.getCount() - 1; i > 0; i--)
    if (routeDefs[i - 1].type == routeDefs[i].type)
      routeDefs.remove(i);

  return (!routeDefs.isEmpty());
}

bool CTransportModule::detectMultiTransferRoute(CStopArray const &targetStops,
  CRouteDefArray &routeDefs)
{
  CStopArray fromStops, toStops;
  CTransferStopArray transferStops;

  getNearestStopsByType(fromStops, sourceStops);
  getNearestStopsByType(toStops, targetStops);

  findTransferStops(transferStops, 50);
  return twoperes(transferStops, fromStops, routeDefs, toStops) ||
    three(transferStops, fromStops, routeDefs, toStops);

}

bool CTransportModule::detectSingleTransferRoute(
  CViewContext const& viewContext, CStopArray const &targetStops,
  CRouteDefArray &routeDefs)
{
  int subwayStatus = 0;
  char targetSubway[40] = "";

  if (sourceStops[0].vt == 1)
  {
    if (calcSubwayTransfer(viewContext, routeDefs, targetStops, sourceStops[0].nobj, subwayStations, true /*fromSubway*/))
      subwayStatus = 1;
  }
  else if (targetStops[0].vt == 1)
  {
    if (calcSubwayTransfer(viewContext, routeDefs, sourceStops, targetStops[0].nobj, subwayStations, false /*fromSubway*/))
    {
      subwayStatus = 2;
      if (!fetchStopData(1, targetStops[0].nobj, targetSubway))
        return NULL;
    }
  }

  CTransferStopArray transferStops;
  findTransferStops(transferStops, 40);

  if (!stopNodes.isEmpty())
    calcCommonTransfer(transferStops, sourceStops, targetStops, routeDefs);

  return !routeDefs.isEmpty();
}

void CTransportModule::findStopsAtPos(CViewContext const& viewContext) const
{
  CStopArray srcStops, filteredStops;
  CPointS pos;
  if (find_transport_stops(viewContext, srcStops, pos))
  {
    getNearestStopsByType(filteredStops, srcStops);
    TEXT_PACK_MENUS tp(450, 185, 632, 11, "Транспорт", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      displayError(0);
    else
    {
      viewContext.updateViewPort();
      for (int i = 0; i < filteredStops.getCount(); i++)
        gMapDataHandler->draw_one_obj(viewContext, filteredStops[i].nobj, 12, 1);
      settwink(12, 15);
      ATRTR(&tp, filteredStops);
      tp.draw();
      tp.textxy(1, 13, "Esc,Enter - выход");
      tp.menu_key2();
      tp.restore();
      tp.del();
      nottwink();
      for (int ii = 0; ii < filteredStops.getCount(); ii++)
        gMapDataHandler->draw_one_obj(viewContext, filteredStops[ii].nobj, -1, 1);
      viewContext.updateViewPort();
    }
  }
}

void CTransportModule::initialize(void)
{
  loadTransferStops();
  loadSubwayStations();
}

void CTransportModule::loadTransferStops(void)
{
  F_SPR spr;
  STOP_NODE node;
  int ret = 0;
  while ((ret = next_model(ret, 1)) > 0)
  { // who
    int nkl = models[ret - 1].numkl;
    if (flibrt[nkl].loc != 1)
      continue;
    if (strStartCmp(flibrt[nkl].clcod, "655") != 0)
      continue;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      if (model.readspr(&spr, i) != 1)
        continue;
      if (!spr.lnh) // skip empty attributes
        continue;
      node.pos.x = spr.xmin;
      node.pos.y = spr.ymin;
      node.n = i;
      stopNodes.add(node);
    }
  }
}

void CTransportModule::loadSubwayStations(void)
{
  CPointS pos;
  mk_atrf maot;
  int ret = 0;
  char title[40];
  SUBWAY_STATION *station, newStation;

  while ((ret = next_model(ret)) > 0)
  {
    int nkl = models[ret - 1].numkl;
    if (flibrt[nkl].loc != 1)
      continue;
    if (strStartCmp(flibrt[nkl].clcod, "62132") != 0)
      continue;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      if (!fetchStopData(1, i, title, NULL, &pos))
        continue;

      if (!title[0])
        continue;

      bool stationExists = false;
      for (int j = 0; j < subwayStations.getCount(); j++)
      {
        station = &subwayStations[j];
        if ((strcmp(title, station->title)) == 0)
        {
          station->pos.x = station->pos.x / 2 + pos.x / 2;
          station->pos.y = station->pos.y / 2 + pos.y / 2;
          stationExists = true;
          break;
        }
      }

      if (!stationExists)
      {
        newStation.pos.x = pos.x;
        newStation.pos.y = pos.y;
        newStation.obj = i;
        strcpy(newStation.title, title);
        subwayStations.add(newStation);
      }
    }
  }
}

void CTransportModule::putSourcePos(CViewContext const& viewContext)
{
  bool hasPoint = queryPointAndStops(viewContext, sourcePos, sourceStops);
  setHasSourcePos(hasPoint);
}

void CTransportModule::putTargetPos(CViewContext const& viewContext)
{
  if (sourceStops.isEmpty())
    return;

  CStopArray dstStops;

  if (!queryPointAndStops(viewContext, targetPos, dstStops))
    return;

  F_SPR spr;
  CRouteDefArray routeDefs;

  CCustomRouteDescription *routeDescription = NULL;

  if (hypot(sourcePos, targetPos) <= 600)
    routeDescription = new CWalkRouteDescription();
  else
  {

    if (!detectDirectRoute(dstStops, routeDefs) &&
      !detectSingleTransferRoute(viewContext, dstStops, routeDefs) &&
      !detectMultiTransferRoute(dstStops, routeDefs))
      {
        displayWarning(4);
        return;
      }

    if (!routeDefs.isEmpty())
      routeDescription = new CRouteDescription(routeDefs);
//    else
//      delete routeDefs;
  }

  if ( routeDescription)
  { // вывод
    TEXT_PACK_MENUS tp(400, 185, 632, 10, "Как доехать", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      goto ex;
    }

    if (routeDescription)
      routeDescription->fillResultWindow(&tp);
/*    else
    if ((j4 != 0) || (j5 != 0))
    { // с п
      if (j4 != 0)
      { // 2 peresad
        ATR2PER(&tp, ob4);
        delete[] pstwo;
        viewContext.updateViewPort();
      } // 2 peresad
      else if (j5 != 0)
      { // 3 peresad
        char ssa[90];
        sprintf(ssa, "От ост. %s", ost);
        tp.add(ssa);

        processRouteData(&tp, nmot1);

        sprintf(ssa, "До ост.%s", pstwo[0].ot);
        tp.add(ssa);
        tp.add("  ДАЛЕЕ  ");
        ATR2PER(&tp, ob4);
        delete[] pstwo;
        viewContext.updateViewPort();
      } // 3 peresad
    }   // с п
*/
    tp.draw();
    tp.textxy(1, 12, "Esc,Enter - выход");
    tp.menu_key2();
    tp.restore();
    tp.del();
    viewContext.updateViewPort();
  } // вывод
ex:
  if (routeDescription)
    delete routeDescription;
  setHasSourcePos(false);
}

void CTransportModule::setHasSourcePos(bool value)
{
  if (hasSourcePos != value)
  {
    hasSourcePos = value;
    CInfoMapApplication::getInstance()->drawIndexMap();
  }
}

bool fillTransferStop(TRANSFER_STOP& stop, mk_atrf attributes)
{
  memset(&stop, 0, sizeof(TRANSFER_STOP));
  bool isEmpty;
  do
  {
    isEmpty = make_atr(&attributes);
    copyAttributeValue(attributes, "09", stop.title);
    copyAttributeValue(attributes, "M0", stop.nmu0);
    copyAttributeValue(attributes, "M1", stop.nmu1);
    copyAttributeValue(attributes, "M2", stop.nmu2);
    copyAttributeValue(attributes, "M3", stop.nmu3);
  } while (!isEmpty);

  return (stop.title[0] || stop.nmu0[0]);
}

PCSTR routesByType(TRANSFER_STOP const& stop, int type)
{
  switch(type)
  {
   case 1:
     return stop.nmu0;
   case 2:
     return stop.nmu1;
   case 3:
     return stop.nmu2;
   case 4:
     return stop.nmu3;
   default:
     return NULL;
  }
}

void CTransportModule::findTransferStops(CTransferStopArray &stops, int depth)
{
  mk_atrf maot;
  STOP_NODE *node;
  TRANSFER_STOP transferStop;
  stops.clear();
  int j;

  for (int i = 0; i < stopNodes.getCount(); i++)
  {
    node = &stopNodes[i];
    float distance = hypot(node->pos, sourcePos) +
      hypot(node->pos, targetPos);


    for (j = 0; j < stops.getCount() && distance > stops[j].distance; j++);

    if (j >= depth)
      continue;

    if (!loadAttributes(node->n, maot))
      continue;

    if (fillTransferStop(transferStop, maot))
    {
      transferStop.pos = node->pos;
      transferStop.obj = node->n;
      transferStop.distance = distance;

      if (stops.getCount() >= depth)
        stops.removeLast();
      stops.insert(j, transferStop);
    }
  }
}

const struct
{
  int type;
  PCSTR code;
} transportCodes[] =
{
  {1, "62132"},
  {2, "651"},
  {3, "652"},
  {4, "653"}
};

int code2transportType(PCSTR code)
{
  for (int i = 0; i < sizeof(transportCodes) / sizeof(transportCodes[0]); i++)
    if (!strStartCmp(code, transportCodes[i].code))
      return transportCodes[i].type;

  return -1;
}

class CTransportFindObjectQuery: public CCustomFindObjectQuery
{
private:
  CStopArray& stops;
protected:
  virtual int addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints);
public:
  CTransportFindObjectQuery(CStopArray& _stops);
};

CTransportFindObjectQuery::CTransportFindObjectQuery(CStopArray& _stops) :
  stops(_stops)
{
  stops.clear();
}

int CTransportFindObjectQuery::addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints)
{
  for (int i = 0; i < stops.getCount(); i++)
  {
    if (distance < stops[i].d)
      break;
  }
  if (i < MAX_TRANSPORT_STOPS)
  {
    OBSTOP stop;
    stop.d = distance;
    stop.nobj = index;
    stop.vt = code2transportType(fl->clcod);
    while (stops.getCount() >= MAX_TRANSPORT_STOPS)
      stops.removeLast();
    stops.insert(i, stop);
    if (stops.getCount() >= MAX_TRANSPORT_STOPS)
      updateThreshold(stops.getLast().d, deltaCommon, deltaPoints);
  }

  return stops.getCount();
}

int transport_filter(PCSTR code, char loc)
{
  return loc < 4 && code2transportType(code) > 0;
}

bool find_transport_stops(CViewContext const& viewContext, CStopArray& stops, CPointS& pos)
{
  if (!fillQueryPosition(pos))
    return false;

  CTransportFindObjectQuery query(stops);
  query.viewContext = &viewContext;
  query.ignoreScaleFilter = true;
  query.checkEnabled = false;
  query.filter_proc = transport_filter;
  query.threshold = DXDISEKR(1000);
  query.pos = pos;

  int findObjectResult = find_objects(query);

  if (!findObjectResult)
    displayWarning(0);

  return findObjectResult > 0;
}

int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos, CStopArray& stops)
{
  CTransportFindObjectQuery query(stops);
  query.viewContext = &viewContext;
  query.ignoreScaleFilter = true;
  query.checkEnabled = false;
  query.filter_proc = transport_filter;
  query.threshold = DXDISEKR(500);
  query.pos = pos;

  int findObjectResult = find_objects(query);

  return (findObjectResult > 0) ? findObjectResult : 0;
}

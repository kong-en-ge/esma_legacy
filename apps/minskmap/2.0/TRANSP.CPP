#include "coretype.hpp"
#include "utils.hpp"
#include "funcs.hpp"
#include "transp.hpp"
#include "findobj.hpp"
#include "apputils.hpp"
#include "globalda.hpp"

bool transportFromMarkerVisible = false;

const struct
{
  int type;
  const char* code;
} transportCodes[] =
{
  {1, "62132"},
  {2, "651"},
  {3, "652"},
  {4, "653"}
};

int code2transportType(const char* code)
{
  for (int i = 0; i < sizeof(transportCodes) / sizeof(transportCodes[0]); i++)
    if (!strStartCmp(code, transportCodes[i].code))
      return transportCodes[i].type;

  return -1;
}

class CTransportFindObjectQuery: public CCustomFindObjectQuery
{
private:
  int _count;
protected:
  virtual int addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints);
public:
  CTransportFindObjectQuery();
};

CTransportFindObjectQuery::CTransportFindObjectQuery()
{
  _count = 0;
}

int CTransportFindObjectQuery::addObject(int index, double distance, FLIB const* fl, int* deltaCommon, int* deltaPoints)
{
  for (int i = 0; i < _count; i++)
  {
    if (distance < obstop[i].d)
      break;
  }
  if (i < MAX_TRANSPORT_STOPS)
  {
    memmove(obstop + i + 1, obstop + i, (_count - i) * sizeof(OBSTOP));
    obstop[i].d = distance;
    obstop[i].nobj = index;
    obstop[i].vt = code2transportType(fl->clcod);
    if (_count < MAX_TRANSPORT_STOPS)
      _count++;
    if (_count == MAX_TRANSPORT_STOPS)
      updateThreshold(obstop[MAX_TRANSPORT_STOPS - 1].d, deltaCommon, deltaPoints);
  }

  return _count;
}

int transport_filter(const char* code, char loc)
{
  return loc < 4 && code2transportType(code) > 0;
}

int find_transport_stops(CViewContext const& viewContext, CPointS& pos)
{
  if (!fillQueryPosition(pos))
    return -1;

  CTransportFindObjectQuery query;
  query.viewContext = &viewContext;
  query.ignoreScaleFilter = 1;
  query.checkEnabled = 0;
  query.filter_proc = transport_filter;
  query.threshold = DXDISEKR(1000);
  query.pos = pos;

  int findObjectResult = find_objects(query);

  if (!findObjectResult)
    displayWarning(0);

  return findObjectResult > 0 ? findObjectResult : 0;
}

int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos)
{
  CTransportFindObjectQuery query;
  query.viewContext = &viewContext;
  query.ignoreScaleFilter = 1;
  query.checkEnabled = 0;
  query.filter_proc = transport_filter;
  query.threshold = DXDISEKR(500);
  query.pos = pos;

  int findObjectResult = find_objects(query);

  return (findObjectResult > 0) ? findObjectResult : 0;
}


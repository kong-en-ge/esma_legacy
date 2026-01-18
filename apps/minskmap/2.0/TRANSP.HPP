#ifndef __TRANSP_HPP__
#define __TRANSP_HPP__

#include "custarry.hpp"
#include "geometry.hpp"
#include "viewctx.hpp"
#include "txtpacks.hpp"
#include "fatr.hpp"

#define MAX_TRANSPORT_STOPS 20
#define MAX_ROUTE_DEFS 5

typedef struct
{
  char stopTitle[40];
  char routes[90];
} ROUTE_STAGE;

typedef struct
{
  unsigned stageCount;
  float distance;
  int type;
  ROUTE_STAGE stages[5];
} ROUTE_DEF;

class CRouteDefArray : public CCustomArray<ROUTE_DEF>
{
public:
  bool insertOrdered(ROUTE_DEF const &routeDef, unsigned maxCount);
};

/*
typedef struct
{
  char const *subwayTitle;
  char stopTitle[40];
  char routes[55];
  char type;
  float distance;
} SUBWAY_TRANSFER;
typedef CCustomArray<SUBWAY_TRANSFER> CSubwayTransferArray;

typedef struct
{
  ROUTE_STAGE src;
  ROUTE_STAGE dst;
  char transferStop[40];
  float distance;
} COMMON_TRANSFER;
typedef CCustomArray<COMMON_TRANSFER> CCommonTransferArray;

struct TWOPERES
{
  ROUTE_STAGE src;
  ROUTE_STAGE dst;

  char stperes1[40], stperes2[40];
  char srcTitle[40];
  char dstTitle[40];
  char srcRoutes[90];
  char dstRoutes[90];
  char routesBetween[90];
  float distance;
};
*/
typedef struct
{
  char title[40];
  CPointS pos;
  int obj;
} SUBWAY_STATION;
typedef CCustomArray<SUBWAY_STATION> CSubwayStationArray;

typedef struct
{
  char title[40], nmu0[25], nmu1[55], nmu2[55], nmu3[55];
  CPointS pos;
  int obj;
  float distance;
} TRANSFER_STOP;
typedef CCustomArray<TRANSFER_STOP> CTransferStopArray;

typedef struct
{
  CPointS pos;
  int n;
} STOP_NODE;

typedef struct
{
  int nobj;
  float d;
  char vt;
} OBSTOP;
typedef CCustomArray<OBSTOP> CStopArray;

bool calcSubwayTransfer(CViewContext const& viewContext,
  CRouteDefArray &routeDefs, CStopArray const& dstStops, int subwayObj,
  CSubwayStationArray const& subwayStations, bool fromSubway);

bool calcCommonTransfer(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CStopArray const& dstStops,
  CRouteDefArray &routeDefs);

int twoperes(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CRouteDefArray &routeDefs,
  CStopArray const& dstStops);

bool three(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CRouteDefArray &routeDefs,
  CStopArray const& dstStops);

int comuzel();
int findNearestStop(CStopArray const& stops, int type);

bool find_transport_stops(CViewContext const& viewContext, CStopArray& stops, CPointS& pos);
int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos, CStopArray& stops);

class CCustomRouteDescription
{
public:
  virtual ~CCustomRouteDescription(void);
  virtual void fillResultWindow(TEXT_PACK_MENUS *tp) const = 0;
};

class CTransportModule {
private:
  CStopArray sourceStops;
  CPointS sourcePos;
  CPointS targetPos;
  bool hasSourcePos;

  CCustomArray<STOP_NODE> stopNodes;
  CSubwayStationArray subwayStations;

  bool detectDirectRoute(CStopArray const &targetStops,
    CRouteDefArray &routeDefs);
  bool detectSingleTransferRoute(
    CViewContext const& viewContext, CStopArray const &targetStops,
    CRouteDefArray &routeDefs);
  bool detectMultiTransferRoute(CStopArray const &targetStops,
    CRouteDefArray &routeDefs);
  void loadTransferStops(void);
  void loadSubwayStations(void);
  void setHasSourcePos(bool value);
  void findTransferStops(CTransferStopArray &transferStops, int depth);
public:
  CTransportModule();

  CPointS const& getSourcePos(void) const;
  CPointS const& getTargetPos(void) const;
  bool getHasSourcePos(void) const;
  void initialize(void);

  void findStopsAtPos(CViewContext const& viewContext) const;
  void putSourcePos(CViewContext const& viewContext);
  void putTargetPos(CViewContext const& viewContext);
};

inline bool CTransportModule::getHasSourcePos(void) const
{
  return hasSourcePos;
}

inline CPointS const& CTransportModule::getSourcePos(void) const
{
  return sourcePos;
}

inline CPointS const& CTransportModule::getTargetPos(void) const
{
  return targetPos;
}

//void ATRMT(TEXT_PACK_MENUS *tp,CSubwayTransferArray const &transfers);
//void ATRTM(CTransportModule const& transportModule, TEXT_PACK_MENUS *tp, int ob);
//void ATRPER(TEXT_PACK_MENUS *tp, int ob);
//void ATR2PER(TEXT_PACK_MENUS *tp, int ob);
void ATRTR(TEXT_PACK_MENUS *tp, CStopArray const &stops);

#define UNDEFINED -32768

void processRouteData(TEXT_PACK_MENUS *tp, char const* src);
//void extractStopAttributes(int type, mk_atrf& attributes,
//  char* title, char* routes);
bool fetchStopData(int type, int obj, char *title, char *routes = NULL,
  CPointS *pos = NULL);
const char* copyRouteStr(char* dst, char const* src);
bool getCommonRoutes(char* dst, char const* src1, char const* src2);
bool composeCommonRoutes(int type, char const *src1, char const *src2,
  char *dst, CPointS *dstPos, CPointS const *srcPos);

char const * routesByType(TRANSFER_STOP const& stop, int type);
void getNearestStopsByType(CStopArray &dstStops, CStopArray const& srcStops);

bool calcCommonRoutes(TRANSFER_STOP const& transferStop,
  CStopArray const &stops, ROUTE_STAGE &routeStage, CPointS &pos);
//int find_nearest_stops(CStopArray const& stops, OBSTOP* dst);
void copyAttributeValue(mk_atrf& attributes, const char* attrName, char* dst);
void filterRouteEndingStops(CStopArray const& input, CStopArray& output);

#endif
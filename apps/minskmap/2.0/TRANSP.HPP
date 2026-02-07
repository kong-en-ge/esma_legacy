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

struct ROUTE_DEF
{
private:
  void init(void);
  unsigned stageCount;
public:
  float distance;
  int type;
  ROUTE_STAGE stages[5];

  ROUTE_DEF(void);
  ROUTE_DEF(unsigned _stageCount);
  unsigned getStageCount(void) const
  {
    return stageCount;
  }
};

class CRouteDefArray : public CCustomArray<ROUTE_DEF>
{
public:
  bool insertOrdered(ROUTE_DEF const &routeDef, unsigned maxCount);
};

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

bool calcTwoTransfers(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CStopArray const& dstStops, 
  CRouteDefArray &routeDefs);

bool calcThreeTransfers(CTransferStopArray const& transferStops,
  CStopArray const& srcStops, CStopArray const& dstStops, 
  CRouteDefArray &routeDefs);

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

void processRouteData(TEXT_PACK_MENUS *tp, PCSTR src);
//void extractStopAttributes(int type, mk_atrf& attributes,
//  char* title, char* routes);
bool fetchStopData(int type, int obj, PSTR title, PSTR routes = NULL,
  CPointS *pos = NULL);
PCSTR copyRouteStr(PSTR dst, PCSTR src);
bool getCommonRoutes(PSTR dst, PCSTR src1, PCSTR src2);
bool composeCommonRoutes(int type, PCSTR src1, PCSTR src2,
  char *dst, CPointS *dstPos, CPointS const *srcPos);
bool composeTransferStage(TRANSFER_STOP const &stop1,
  TRANSFER_STOP const &stop2, ROUTE_STAGE &stage);

PCSTR routesByType(TRANSFER_STOP const& stop, int type);
void getNearestStopsByType(CStopArray &dstStops, CStopArray const& srcStops);

bool calcCommonRoutes(TRANSFER_STOP const& transferStop,
  CStopArray const &stops, ROUTE_STAGE &routeStage, CPointS &pos);
//int find_nearest_stops(CStopArray const& stops, OBSTOP* dst);
void copyAttributeValue(mk_atrf& attributes, PCSTR attrName, PSTR dst);

#endif
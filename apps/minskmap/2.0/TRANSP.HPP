#ifndef __TRANSP_HPP__
#define __TRANSP_HPP__

#include "custarry.hpp"
#include "geometry.hpp"
#include "viewctx.hpp"
#include "txtpacks.hpp"

#define MAX_TRANSPORT_STOPS 20

struct PERES
{
  char stperes[40];
  char ot_kd[40];
  char nm[55];
  char vt;
  float d;
};
//            int nperes;
//            int not_kd;     };
struct PERESU
{
  char stperes[40];
  char kd[40];
  char ot[40];
  char nmot[90];
  char nmkd[90];
  float d;
};
/*             char vtot,vtkd;
         float d;
         int nperes,pcol;
         int xot,yot;
         int xkd,ykd;     };   */
struct TWOPERES
{
  char stperes1[40], stperes2[40];
  char kd[40];
  char ot[40];
  char nmot[90];
  char nmkd[90];
  char nmuz[90];
  float d;
};
struct METRO
{
  char smetro[40];
  int x, y, n;
};
extern METRO *metro;
struct UZEL
{
  char suzel[40], nmu0[25], nmu1[55], nmu2[55], nmu3[55];
  int x, y, n;
  float d;
};
extern UZEL *uzel;
struct UZLY
{
  int x, y, n;
};
extern UZLY *uzly;
extern CPointS transportFrom, transportTo;
extern int kolmet, koluz, comuz, prmt, uzl;
extern char stt[40], nmot1[90], ost[40];

typedef struct
{
  int nobj;
  float d;
  char vt;
} OBSTOP;

struct OTK_KUD
{
  char otk[40];
  char kud[40];
  char nm[55];
  char vt;
  float dot, dkd;
  int notk;
  int nkud;
};

typedef CCustomArray<OBSTOP> CStopArray;

int otk_met_kuda(CViewContext const& viewContext,
  PERES *pss, CStopArray const& stops, int nn);
int peresad(CStopArray const& srcStops, PERESU *ps, CStopArray const& dstStops);
int twoperes(CStopArray const& srcStops, TWOPERES *ps, CStopArray const& dstStops);
int three(CStopArray const& srcStops, TWOPERES *ps, CStopArray const& dstStops);
int stmetro();
int comuzel();
int findNearestStop(CStopArray const& stops, int type);

extern PERES *ps;
extern TWOPERES *pstwo;
extern PERESU *psu;
extern OTK_KUD otk_kud[], o_k[];


int ATRFR(TEXT_PACK_MENUS *tp, int j1);
void ATRMT(TEXT_PACK_MENUS *tp, int ob);
void ATRTM(TEXT_PACK_MENUS *tp, int ob);
void ATRPER(TEXT_PACK_MENUS *tp, int ob);
void ATR2PER(TEXT_PACK_MENUS *tp, int ob);

int find_transport_stops(CViewContext const& viewContext, CStopArray& stops, CPointS& pos);
int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos, CStopArray& stops);

class CTransportModule {
private:
  CStopArray sourceStops;
  CPointS sourcePos;
  CPointS targetPos;
  bool hasSourcePos;

  void setHasSourcePos(bool value);
public:
  CTransportModule();

  CPointS const& getSourcePos(void) const;
//  CPointS const& getTargetPos(void) const;
  bool getHasSourcePos(void) const;

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

//inline CPointS const& CTransportModule::getTargetPos(void) const
//{
//  return targetPos;
//}

#endif
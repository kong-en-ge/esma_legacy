#ifndef __TRANSP_HPP__
#define __TRANSP_HPP__

#include "custarry.hpp"
#include "viewctx.hpp"

#define MAX_TRANSPORT_STOPS 20

typedef struct
{
  int nobj;
  float d;
  char vt;
} OBSTOP;

typedef CCustomArray<OBSTOP> CStopArray;

extern bool transportFromMarkerVisible;

int find_transport_stops(CViewContext const& viewContext, CStopArray& stops, CPointS& pos);
int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos, CStopArray& stops);

#endif
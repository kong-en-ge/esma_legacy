#ifndef __TRANSP_HPP__
#define __TRANSP_HPP__


#include "viewctx.hpp"

#define MAX_TRANSPORT_STOPS 20

extern bool transportFromMarkerVisible;

int find_transport_stops(CViewContext const& viewContext, CPointS& pos);
int find_transport_stops_at_pos(CViewContext const& viewContext, CPointS const& pos);

#endif
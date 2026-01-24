#ifndef __MMFUNCS_HPP__
#define __MMFUNCS_HPP__

#include "coretype.hpp"
#include "viewctx.hpp"

void find_rg(CViewContext const& viewContext);
void find_st(CViewContext const& viewContext);
bool find_bl(CViewContext const& viewContext, PCSTR st, int bl);

#endif
#ifndef menuall2
#define menuall2

#include "viewctx.hpp"

void find_rg(CViewContext const& viewContext);
void find_st(CViewContext const& viewContext);
int find_bl(CViewContext const& viewContext,
  char const *parst = NULL, int parbl = 0);

#endif
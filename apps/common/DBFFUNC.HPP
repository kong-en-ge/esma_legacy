#ifndef __DBFFUNC_HPP__
#define __DBFFUNC_HPP__

#include "viewctx.hpp"

void onoff(CViewContext const& viewContext);
void chmark(CViewContext const& viewContext);
void finddbf(CViewContext const& viewContext);
void new_dbf();
void del_dbf(CViewContext const& viewContext);
void act_dbf(void);
void pnt_dbf(void);
void red_rec(void);
void whatisitd(CViewContext const& viewContext, int mode);
void draw_fld(void);

#endif
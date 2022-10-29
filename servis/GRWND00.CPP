#include "mouse.hpp"
#include "grwnd.hpp"

void pascal savesettings(SVS *svs) {
  getviewsettings(&svs->view);
  getfillsettings(&svs->fill);
  svs->color=getcolor();
  setviewport(0,0,getmaxx(),getmaxy(),1);
  mouse_show(OFF);
}
void pascal restoresettings(SVS *svs) {
  setviewport(svs->view.left,svs->view.top,svs->view.right,svs->view.bottom,
		    svs->view.clip);
  setfillstyle(svs->fill.pattern,svs->fill.color);
  setcolor(svs->color);
  mouse_show(ON);
}

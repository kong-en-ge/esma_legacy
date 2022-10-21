#include "globalda.hpp"
#include "dbf.hpp"

#define COORD

void crd_dbf() {
#ifdef COORD
 FIELD *field=(FIELD*)new char[4*sizeof(FIELD)];
 {FIELD fld("XMIN",'N',11,2);field[0]=fld;}
 {FIELD fld("YMIN",'N',11,2);field[1]=fld;}
 {FIELD fld("XMAX",'N',11,2);field[2]=fld;}
 {FIELD fld("YMAX",'N',11,2);field[3]=fld;}
 DBF db;
 int sp=db.Creat("COORD.DBF",4,field,0);	//0 or 1-переключатель?
 delete (field);
 if (sp==-1) return;
 db.R_blank();
 dtoa(XMINV,db.F_ptr(1),11,2);
 dtoa(YMINV,db.F_ptr(2),11,2);
 dtoa(XMAXV,db.F_ptr(3),11,2);
 dtoa(YMAXV,db.F_ptr(4),11,2);
 db.R_write();
 db.Close();
#endif
}

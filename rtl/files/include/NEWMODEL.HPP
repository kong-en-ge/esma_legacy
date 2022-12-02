#ifndef newmodelhpp
  #define newmodelhpp
#include "geomodel.hpp"

struct METHAR {
  int hmt;
  int *mtbuf;
  unsigned mtsize,nmtr,nmtd;
};

int pascal write_metharnew(const void *metr,unsigned *stb,
					  unsigned *enb,METHAR *mt);
int pascal flush_methar(METHAR *mt);
int pascal copymetrtobuf(void *buff,void *metr);

class  NEW_MODEL {
public:
  int hsp;//handles
  SPR *spbuf;
  unsigned spsize,nspr,nspd;
  METHAR m,h;

  pascal NEW_MODEL();
  pascal ~NEW_MODEL();
  int pascal open(char *path,char *mod,int spsize=640,
			   int mtsize=80,int harsize=20);
  int pascal close();
  int pascal writeform(const void *form);
  int pascal writespr (const SPR  *spr);
  int pascal writenullspr(void);
  int pascal writemets(const void *metr,SPR *spr);
  int pascal writehars(const void *hars,SPR *spr);
//с построением габаритной рамки
  int pascal writemetsgb(const void *metr,SPR *spr);
};

#endif
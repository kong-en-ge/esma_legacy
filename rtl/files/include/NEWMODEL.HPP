#ifndef newmodelhpp
#define newmodelhpp
#include "geomodel.hpp"

struct METHAR
{
  int hmt;
  int *mtbuf;
  unsigned mtsize, nmtr, nmtd;
};

int write_metharnew(const void *metr, unsigned *stb,
                    unsigned *enb, METHAR *mt);
int flush_methar(METHAR *mt);
int copymetrtobuf(void *buff, void *metr);

class NEW_MODEL
{
public:
  int hsp; // handles
  SPR *spbuf;
  unsigned spsize, nspr, nspd;
  METHAR m, h;

  NEW_MODEL();
  ~NEW_MODEL();
  int open(char *path, char *mod, int spsize = 640,
           int mtsize = 80, int harsize = 20);
  int close();
  int writeform(const void *form);
  int writespr(const SPR *spr);
  int writenullspr(void);
  int writemets(const void *metr, SPR *spr);
  int writehars(const void *hars, SPR *spr);
  //с построением габаритной рамки
  int writemetsgb(const void *metr, SPR *spr);
};

#endif
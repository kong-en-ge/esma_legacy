#ifndef CONVERTHPP
#define CONVERTHPP

#include <alloc.h>
#include <bios.h>
#include <dir.h>
#include <dos.h>
#include <fstream.h>
#include <math.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigpath.hpp"
#include "gdata.hpp"
#include "geomodel.hpp"
#include "flibrt.hpp"
#include "mouse.hpp"
#include "set.hpp"
#include "textpack.hpp"
#include "textfile.hpp"
#include "fastmod.hpp"
#include "atribut.hpp"
#include "errhand.hpp"

typedef struct
{
  char mapSrc[MAXPATH];
  char mapDst[MAXPATH];
  char mapSrcExt[3];
  char mapDstExt[3];
  char objectDirectory[MAXPATH];
} MapConverterData;

int encodeMapData(MapConverterData const* mapConverterData,
  CErrorHandler* onError);

#endif

#include <io.h>

#include "convert.hpp"
#include "errhand.hpp"

struct esort
{
  unsigned nkl, Num;
};

int esortcmp(const void *c1, const void *c2)
{
  //	esort *u1=(esort *)c1,*u2=(esort *)c2;
  //	int	pr1=flibrt[u1->nkl].pr,pr2=flibrt[u2->nkl].pr;
  //	return (pr1>pr2) ? 1 : (pr1<pr2) ? -1 :
  //	 (u1->nkl>u2->nkl ) ? 1 : (u1->nkl <u2->nkl ) ? -1 :
  //	 (u1->num>u2->num) ? 1 :  (u1->num<u2->num) ? -1 : 0;
  asm {
	mov		cx,5
	les		bx,c1
	mov		dx,es:[bx].nkl
	mov		si,dx
	shl		si,cl
	les		bx,c2
	mov		ax,es:[bx].nkl
	mov		di,ax
	shl		di,cl
	mov		cx,ax
	les		bx,flibrt
	mov		al,es:[si+bx].pr
	sub		al,es:[di+bx].pr
	cbw
	jnz		ret
	mov		ax,dx
	sub		ax,cx
	jnz		ret
	les		bx,c1
	mov		ax,es:[bx].Num
	les		bx,c2
	sub		ax,es:[bx].Num
  }
ret:
  return _AX;
}

SPR *_spr;
int _last_find;
int FLIBcmp(const void *i1, const void *i2)
{
  char *c1 = *(int *)i1 > FLIBsize ? _spr->clcod : flibrt[*(int *)i1].clcod;
  char *c2 = *(int *)i2 > FLIBsize ? _spr->clcod : flibrt[*(int *)i2].clcod;
  return strncmp(c1, c2, 9);
}

int findobjn(SPR *spr)
{
  _spr = spr;
  int n = FLIBsize + 2;
  int *f = (int *)bsearch(&n, flibsort, FLIBsize, 2, FLIBcmp);
  return (f == NULL) ? -1 : (_last_find = *f);
}

struct METHARFAST
{
  int hmt;
  int *mtbuf;
  unsigned mtsize, nmtr, nmtd;
  unsigned stm, len;
  unsigned char sts;
};

int write_metharfast(int *metr, METHARFAST *m, CErrorHandler* onError)
{
  metr += 2;
  metr[0] = 0xc018;
  unsigned len = size_methar(metr);
  m->len = len;
  m->stm = m->nmtd + m->nmtr / 256;
  m->sts = m->nmtr;
  if (len + m->nmtr > m->mtsize * 256)
  {
    if (write(m->hmt, m->mtbuf, (m->nmtr & 0xff00) * 2) == -1)
    {
      handleError(onError, 1);
      return -1;
    }

    m->nmtd += m->nmtr / 256;
    memmove(m->mtbuf, m->mtbuf + (m->nmtr & 0xff00), (m->nmtr & 0xff) * 2);
    m->nmtr &= 0xff;
    memset(m->mtbuf + m->nmtr, 0, (25600u - m->nmtr) * 2);
  }
  memmove(m->mtbuf + m->nmtr, metr, len * 2);
  m->nmtr += len - 1;
  metr[0] = 0;
  return 0;
}

void make_fast_atrib(F_SPR *fs, unsigned n, char *atrib, METHARFAST &mhf,
  CErrorHandler* onError)
{
  char buff[512], str[40];
  mk_atr mk;
  mk.har = atrib + 8;
  sprintf(str, "Объект %u", *(unsigned *)(atrib + 6));
  memset(buff, 0, 512);
  *((unsigned *)(buff + 6)) = n;
  int ret;
  int s = 8, ls, len, j, l;
  float z;
  char *ch;
  do
  {
    ret = make_atr(&mk);
    if (ret >= 0)
    {
      if (flibrt[fs->numincl].loc == 4 && !strcmp("09", mk.atr))
        continue;
      len = 0;
      for (int i = 0; i < ATRsize; i++)
        if (!strcmp(mk.atr, ATRN[i].atr))
        {
          ls = s;
          switch (ATRN[i].type)
          {
          case 'T':
            len = strlen(mk.zna) + 1;
            memmove(buff + s + 2, mk.zna, len);
            break;
          case 'N':
            z = strtod(mk.zna, &ch);
            if (*ch != 0)
              handleError(onError, 10, str);
            len = 4;
            memmove(buff + s + 2, &z, 4);
            break;
          case 'K':
            ch = ATCD + ATRN[i].start;
            for (j = 0; j < ATRN[i].ncod; j++)
            {
              l = strlen(ch) + 1;
              if (!strcmp(mk.zna, ch))
              {
                ch += l;
                len = 2;
                memmove(buff + s + 2, &j, 2);
                break;
              }
              ch += l;
              ch += strlen(ch) + 1;
            }
            if (len == 0)
              handleError(onError, 8, str);
            break;
          }
          buff[s] = i;
          buff[s + 1] = len;
          s += len + 2;
          break;
        }
      if (i == ATRsize)
        handleError(onError, 7, str);
    }
  } while (ret == 0);
  if (s > 8)
  {
    if (s & 1)
      buff[ls + 1]++; // если байт нечетный увеличить длину
    s = (s + 1) & 0xfe;
    *(int *)(buff + s) = 0xC018;
    buff[ls + 1] |= 0x80;
    if (write_metharfast((int *)buff, &mhf, onError) != 0)
    {
      handleError(onError, 2);
      return;
    }

    fs->sbh = mhf.stm;
    fs->lnh = mhf.len;
    fs->ssh = mhf.sts;
  }
}

int coder(char const *path, char const *mod, CErrorHandler* onError)
{
  char pth[80];
  int _st, _nk;
  GEO_MODEL model;
  F_FORM fr;
  int har = 0;
  FORM orgForm;
  METHARFAST mhf;
  SPR spr;
  int *metr;
  esort *esbase;
  esort huge *es;
  unsigned o;
  char patht[80];
  if (model.openexist(path, mod) != 0)
  {
    handleError(onError, 0);
    return -1;
  }
  sprintf(pth, "%s.ASM", path);
  ofstream of(pth);
  of << "\t.MODEL LARGE text\n\MD struc\n  _start\tdw 0\n"
     << "  _end\tdw 0\n  _nkls\tdw 0\nMD ends\n\t.DATA\n"
     << "_models\tlabel byte\n";
  if (model.readform(&orgForm) != 0)
  {
    handleError(onError, 1);
    return -1;
  }
  unsigned maxn = model.maxn, n = 0;
  fr.xminm = orgForm.xuzp;
  fr.yminm = orgForm.yuzp;
  fr.xmaxm = orgForm.xsvp;
  fr.ymaxm = orgForm.ysvp;
  esbase = (esort *)farmalloc((unsigned long)maxn * sizeof(esort));
  if (esbase == NULL)
  {
    handleError(onError, 4);
    return -1;
  }
  es = esbase;
  for (unsigned i = 1; i < maxn; i++)
  {
    if (model.readspr(&spr, i) != 0)
      continue;
    if (spr.stblm == 0)
      continue;
    unsigned int k = findobjn(&spr);
    if (k == 0xffff)
      continue;
    es[0].Num = i;
    es[0].nkl = k;
    es++;
    n++;
  } // for
  qsort(esbase, n, sizeof(esort), esortcmp);
  F_SPR *fsbase, *fs;
  int hsp, hmt;
  int *mtbase, *mt, j;
  fsbase = new F_SPR[400];
  if (fsbase == NULL)
  {
    handleError(onError, 4);
    return -1;
  }

  memset(fsbase, 0, 400 * sizeof(F_SPR));
  sprintf(patht, "%s.c%s", path, mod);
  if ((hsp = _creat(patht, FA_ARCH)) == -1)
  {
    handleError(onError, 2);
    return -1;
  }

  if (write(hsp, fsbase, sizeof(F_FORM)) == -1)
  {
    handleError(onError, 2);
    return -1;
  }

  mhf.mtbuf = new int[100 * 256];
  if (mhf.mtbuf == NULL)
  {
    handleError(onError, 4);
    return -1;
  }

  sprintf(patht, "%s.d%s", path, mod);
  if ((mhf.hmt = _creat(patht, FA_ARCH)) == -1)
  {
    handleError(onError, 2);
    return -1;
  }

  es = esbase;
  fs = fsbase;
  mhf.mtsize = 100;
  mhf.nmtr = mhf.nmtd = 0;
  _nk = -1;
  for (i = 0; i < n; i++, es++)
  {
    if (model.readspr(&spr, es->Num) != 0)
    {
      handleError(onError, 1);
      return -1;
    }

    fs->numincl = es->nkl;
    if (_nk != es->nkl || i == n - 1)
    {
      if (_nk != -1 && _nk != es->nkl)
      {
        of << "  MD\t<" << _st << "\t," << i - 1 << "\t," << _nk << ">\n";
      }
      if (_nk != es->nkl)
      {
        _st = i;
        _nk = es->nkl;
      }
      if (i == n - 1)
      {
        of << "  MD\t<" << _st << "\t," << i << "\t," << _nk << ">\n";
      }
    }
    if (model.readmets((void **)&metr, &spr) != 0)
    {
      handleError(onError, 1);
      return -1;
    }

    metr[3] = i;
    if (write_metharfast(metr, &mhf, onError) != 0)
    {
      handleError(onError, 2);
      return -1;
    }

    metr[3] = spr.nom;
    memmove(&fs->xmin, &spr.xmin, 4 * sizeof(int));
    fs->sbm = mhf.stm;
    fs->lnm = mhf.len;
    fs->ssm = mhf.sts;
    if (spr.loc == 4)
    {
      char str[80];
      memset(str, 0, 80);
      metr = (int *)str;
      if (model.readhars(&spr, "09", str + 8, 78) == 0)
      {
        metr[3] = i;
        int k = ((strlen(str + 8) + 4) & 0xfffc) / 2;
        metr[k + 4] = 0xc018;
        if (write_metharfast(metr, &mhf, onError) != 0)
        {
          handleError(onError, 2);
          return -1;
        }

        fs->lnm += mhf.len;
      }
    }
    if ((i + 1) % 400 == 0)
    {
      if (write(hsp, fsbase, 400 * sizeof(F_SPR)) == -1)
      {
        handleError(onError, 1);
        return -1;
      }
      fs = fsbase;
      memset(fsbase, 0, 400 * sizeof(F_SPR));
    }
    else
      fs++;
  }
  if (write(hsp, fsbase, (n % 400) * sizeof(F_SPR)) == -1)
  {
    handleError(onError, 2);
    return -1;
  }

  if (lseek(hsp, sizeof(F_FORM), SEEK_SET) == -1)
  {
    handleError(onError, 2);
    return -1;
  }

  es = esbase;
  of << "_nmodels\tdw\t(_nmodels-_models)/size MD\n"
     << "\tpublic\t_models\n\tpublic\t_nmodels\nEND";
  for (i = 0; i < n; i++, es++, fs++)
  {
    if (i % 400 == 0)
    {
      if (read(hsp, fsbase, 400 * sizeof(F_SPR)) == -1)
      {
        handleError(onError, 2);
        return -1;
      }

      fs = fsbase;
      if (lseek(hsp, (unsigned long)(i) * sizeof(F_SPR) + sizeof(F_FORM),
                SEEK_SET) == -1)
      {
        handleError(onError, 2);
        return -1;
      }
    }
    if (model.readspr(&spr, es->Num) != 0)
    {
      handleError(onError, 1);
      return -1;
    }

    if (spr.loc != 4)
    {
      if (model.readhars((void **)&metr, &spr) == 0)
      {
        make_fast_atrib(fs, i, (char *)metr, mhf, onError);
        if (har == 0)
        {
          fr.sbh = mhf.stm;
          fr.ssh = mhf.sts;
          har = 1;
        }
      }
    }
    if ((i + 1) % 400 == 0)
    {
      if (write(hsp, fsbase, 400 * sizeof(F_SPR)) == -1)
      {
        handleError(onError, 2);
        return -1;
      }
    }
  }
  if (write(hsp, fsbase, (n % 400) * sizeof(F_SPR)) == -1)
  {
    handleError(onError, 2);
    return -1;
  }
  if (lseek(hsp, 0, SEEK_SET) == -1)
  {
    handleError(onError, 2);
    return -1;
  }
  fr.maxn = n;
  fr.xmin = model.xminm;
  fr.ymin = model.yminm;
  fr.xmax = model.xmaxm;
  fr.ymax = model.ymaxm;
  if (write(hsp, &fr, sizeof(F_FORM)) == -1)
  {
    handleError(onError, 2);
    return -1;
  }
  if (close(hsp) == -1)
  {
    handleError(onError, 3);
    return -1;
  }
  *(mhf.mtbuf + mhf.nmtr) = 0xc018;
  *(mhf.mtbuf + mhf.nmtr + 1) = 0xc018;
  if (write(mhf.hmt, mhf.mtbuf, (mhf.nmtr & 0xff00) * 2 + 512) == -1)
  {
    handleError(onError, 1);
    return -1;
  }
  if (close(mhf.hmt) == -1)
  {
    handleError(onError, 3);
    return -1;
  }
  delete mhf.mtbuf;
  farfree(esbase);
  delete fsbase;
  of.close();
  handleError(onError, 5);
  return 0;
}

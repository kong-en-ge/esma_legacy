#ifndef geomodel
#define geomodel
#include <memory.h>

#include "files.hpp"

struct SPR
{
  unsigned rez1, nom;
  char clcod[8];
  unsigned char loc, rez2;
  int xmin, ymin, xmax, ymax;
  unsigned stblm, enblm, stblh;
  int rez[2];
};

struct FORM
{
  char nom[16],        //��� ������������
      kob[2],          //��� ��ࠡ�⪨
      tikm[2],         //��� ���
      nosi[2],         //����� ��
      time[10],        //�६� ᮧ�����
      nkom[2],         //����� ��������
      nrm[2],          //����� ࠡ�祣� ����
      nseg[2],         //����� ᥣ����
      rez1[4];         //�����
  unsigned maxn;       //���ᨬ���� ����� ��ꥪ�
  unsigned rez2[10];   //�����
  char nmen[2];        //����� ����
  unsigned scale;      //�������⥫� ����⠡� /1000
  int pak,             //�ਧ��� ��⮬���᪮�� ����஫�
      rez3;            //�����
  char sd[2],          //��� ����⨧�樨
      sp[4];           //��� ������(�� �ᯮ������)
  int rez4[9];         //�����
  float sgp;           //���� ������� ��ࠫ����
  int xuz, yuz,        //���न���� 㣫�� �����
      xsz, ysz,        //� ������
      xsv, ysv,        //
      xuv, yuv;        //
  float xuzf, yuzf,    //���न���� 㣫�� �����
      xszf, yszf,      //� �������᪨� ���न����
      xsvf, ysvf,      //
      xuvf, yuvf;      //
  float xuzt, yuzt,    //������᪨� ���न���� 㣫�� ࠬ��
      xszt, yszt,      //
      xsvt, ysvt,      //
      xuvt, yuvt;      //
  char cp[2];          //��� �஥�樨
  float par1,          //���� ��ࠬ��� �஥�樨
      par2,            //��ன
      par3;            //��⨩
  long zscal;          //�������⥫� ����⠡�
  char gsys[3],        //��������᪠� ��⥬�
      al[3];           //�����ᮨ�
  float shnt, dlnt;    //����+������ ��砫쭮� �窨
  float xuzp, yuzp,    //��אַ㣮��� ���न���� 㣫��
      xszp, yszp,      // x ��������� �����
      xsvp, ysvp,      //
      xuvp, yuvp;      //
  char pkoor[3],       //�।�⠢����� ���न���
      ezmk[3];         //������� ����७�� ���न���
  float rkoor;         //����襭�� ���न���
  char temobl;         //������᪠� �������
  char ezmhh[3];       //������� ����७�� ���⭮� �᭮��
  float razhh;         //���� �祭�� ५��
  int rez5[89];        //�����
  unsigned int u0, u1, //���न���� ����⠩���
      u2, u3,
      u4, u5,
      u6, u7;
  int rez6[22]; //�����
  int razd,     //�������⥫�, �ਧ��� ������ �����
      rez7[3];  //�����
  char sertc,   //����䨪��� � ��������
      sertu,    //����䨪��� 㧫��
      serts,    //����䨪��� ᥬ��⨪�
      sertm;    //����䨪��� ���ਪ�
};

unsigned size_methar(const void *buf);
void gab_ram(const void *metr, SPR *spr);
int writemet_har(const void *met_har, Files *f, unsigned *fbl, unsigned *ebl);
int readmet_har(void **met_har, Files *f, int no, int fbl, int ebl);
extern "C" double pascal square_metr(void const *);
extern "C" int pascal orient_metr(void const *);

//������ -1 - ��� �ࠢ��,-2 - ��� ���ਪ�,-3 - ��� �ࠪ����
int isexistmodel(const char *path, char *mod);

struct FINDHAR
{
  char *atr;      //��ਡ�� ���᪠ (��� ���祭�� ��� � ��� ���筨��)
  int blok, smes; //��砫�� ���� � ᬥ饭�� � �����
  FINDHAR();
};

inline FINDHAR::FINDHAR()
{
  memset(this, 0, sizeof(FINDHAR));
}

class GEO_MODEL
{
private:
  Files hsp, hmt, hhr; // handle-����⥫� 䠩���

public:
  int xminm, yminm, xmaxm, ymaxm;
  int xminw, yminw, xmaxw, ymaxw;
  char path[80], mod[3];
  int maxn;

  GEO_MODEL(void);
  ~GEO_MODEL(void);
  int openexist(const char *name, const char *model, int spbsize = 320,
                int mtsize = 80, int hrsize = 20, int readonly = 0);
  int close(void);
  int readform(void *buff);
  int writeform(const void *buff);
  int readsprb(void **sprp, unsigned no); //��⠥� �� ���� 䠩��
  int readspr(void *spr, unsigned no);    //��⠥� � ���� ���짮���
  int readmet(void **metp, unsigned no);  //��⠥� �� ���� 䠩��
  int readhar(void **harp, unsigned no);  //��⠥� �� ���� 䠩��
  int readmets(void **metp, SPR *spr);    //��⠥� �� ���� 䠩��
  int readhars(void **harp, SPR *spr);    //��⠥� �� ���� 䠩��
  int readhars(void **harp, SPR *spr, char *attr);
  int readhars(SPR *spr, char *attr, char *str, int n);
  //����頥� ��ਡ�� � ��ப� str �� ����� n ᨬ�����
  int readdoublehars(SPR *spr, char *attr, double *at);
  int findhar(FINDHAR *f);
  //�����頥� -1 - �訡�� ; 0 - �� ������ ��ꥪ� ; ���� ����� ��ꥪ�

  //�����뢠���� ���ਪ� �� ����७���� ���� 䠩��
  int writemetbuff(SPR *spr);
  int writeharbuff(SPR *spr);
  int writespr(const SPR *spr);
  int writemets(const void *metr, SPR *spr);
  int writehars(const void *har, SPR *spr);
  //� ����஥���� �����⭮� ࠬ��
  int writemetsgb(const void *metr, SPR *spr);
};

#endif
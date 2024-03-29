#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#include "graphics.h"
#include "fstmod.hpp"
#include "font.hpp"
#include "flibrt.hpp"
#include "indexmap.hpp"

#define WHITE 0
#define BLACK 15
#define MENUTOP 43
#define MENUBOT 188

#define MAX_STORED_VIEWS 10

struct List_base
{
  char name[20];
  char file[9];
  char mark[2];
  char activ;
};

extern int XMAXMAPE, YMAXMAPE;
extern double XMING, YMING, XMAXG, YMAXG;     // ���ᨬ���� ���न����
extern double XMINM, YMINM, XMAXM, YMAXM;     //������� ���न���� ����� � ��
extern double XMINV, YMINV, XMAXV, YMAXV;     // ������� ���न���� ���� ���
extern int XMINE, YMINE, XMAXE, YMAXE;
extern double XMINVC, YMINVC, XMAXVC, YMAXVC; // ������� ���न����  �����.���� ���
extern int XMINEC, YMINEC, XMAXEC, YMAXEC;
extern unsigned char SCALES;     // �⥯��� 㢥��祭��

extern double _DXM, _DYM;
extern int DXM;

extern SCRIPT scr;
extern fonttype ft;

extern float CURRENTSCALE;
extern float SCALEMIN, SCALEMAX; // min/max scale
extern float SCALEO; // �᭮����� �⥯���

extern int All_Map;

extern double otkat[];
extern int otkatn;

extern FST_MODEL model;
extern PART_MODEL models[]; // ��� ������
extern int nmodels;         // �᫮ ���������� ������

extern int _xMK, _yMK;

extern const int colordbf[];
extern List_base *LBS;
extern int LBSsize;
extern char const * LBSFilename;
extern int actdbf;

extern CIndexMapImage indexMapImage;
extern int GdEGA;

// external classifier data
extern FLIB flibrt[];
extern int flibsort[];
extern linesstyle fliblin[];
extern int LINsize;
extern RAS ras[];
extern int RASsize;
extern unsigned FLIBsize;
extern atrname ATRN[];
extern char near *KLSKP[]; // ᬥ饭�� ��ப �����
extern char KLSK[];        // ��ப� �����
extern far char ATCD[];
extern far unsigned ATCDP[];
extern int ATRsize; // �᫮ ��������� ��ਡ�⮢

#endif

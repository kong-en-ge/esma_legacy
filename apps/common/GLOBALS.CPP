#include "globals.hpp"

double XMING, YMING, XMAXG, YMAXG;     // ������� ���न����
double XMINV, YMINV, XMAXV, YMAXV;     // ������� ���न���� ���� ���
double XMINM, YMINM, XMAXM, YMAXM;     //������� ���न���� ����� � ��
int XMINE, YMINE, XMAXE, YMAXE;     // ������� ���न���� ���� ����� �� ���
double XMINVC, YMINVC, XMAXVC, YMAXVC; // ������� ���न���� �����.���� ���
int XMINRE, YMINRE, XMAXRE, YMAXRE; // ������� ���न���� ���� ࠧ���� ���
int XMINEC, YMINEC, XMAXEC, YMAXEC; // ������� ���न���� �����.���� ����� �� ���
unsigned char SCALES; // �⥯��� 㢥��祭��
int XMAXMAPE = 479;
int YMAXMAPE = 349;
double _DXM, _DYM;
int DXM;

SCRIPT scr;
fonttype ft;

float CURRENTSCALE;
float SCALEMIN, SCALEMAX;
float SCALEO;

int All_Map;

double otkat[4 * MAX_STORED_VIEWS];
int otkatn;

FST_MODEL model;

int _xMK = 240, _yMK = 240;

const int colordbf[] = {RED, YELLOW, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN};
List_base *LBS;
int LBSsize;
int actdbf = 0;

char const * LBSFilename;

CIndexMapImage indexMapImage;
int GdEGA = 0;
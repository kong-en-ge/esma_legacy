#ifndef XMSAH
#define XMSAH

struct EMMSTRUC
{
  unsigned long size;   //�⭮� !
  unsigned int handles; //���筨�, 0 - ��� ������
  union
  {
    unsigned long smess; //ᬥ饭�� ���筨��
    void far *ptrs;      //㪠��⥫� ��� �����
  };
  unsigned int handled; //�ਥ����, 0 - ��� ������
  union
  {
    unsigned long smesd; //ᬥ饭�� �ਥ�����
    void far *ptrd;      //㪠��⥫� ��� �����
  };
};

extern "C"
{
  unsigned char pascal XMSerror(void);
  int pascal XMSinstalled(void);
  void pascal DisableXMS();
}

void getXMSmem(unsigned int *Total, unsigned int *MaxBlock);
unsigned int allocXMS(unsigned int Mem);
int freeXMS(unsigned int handle);
int moveXMS(EMMSTRUC far *M);

#endif

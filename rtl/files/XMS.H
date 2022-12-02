#ifndef XMSAH
 #define XMSAH

struct EMMSTRUC {
  unsigned long size;   //четное !
  unsigned int  handles;//источник, 0 - дос память
  union {
   unsigned long smess;  //смещение источника
   void far *    ptrs ;  //указатель дос памяти
  };
  unsigned int  handled;//приемник, 0 - дос память
  union {
   unsigned long smesd;  //смещение приемника
   void far *    ptrd ;  //указатель дос памяти
  };
};

extern "C" {
  unsigned char  pascal XMSerror(void);
  int pascal XMSinstalled(void);
  void pascal DisableXMS();
}

void getXMSmem(unsigned int *Total,unsigned int *MaxBlock);
unsigned int allocXMS(unsigned int Mem);
int freeXMS(unsigned int  handle);
int moveXMS(EMMSTRUC far *M);

#endif

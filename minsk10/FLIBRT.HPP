#ifndef flibrthpp
 #define flibrthpp
struct OFLIB {
  char clcod[8];
  unsigned char loc,pr,color,num;
};
//Стуктура файла Flibrt.dat
struct FLIB {
  char clcod[8]        ;//код
  unsigned char loc    ;//локализация
  unsigned char pr     ;//приоритет
  unsigned char color  ;//цвет
  unsigned char colorg ;//цвет границы для площадей
  unsigned char num    ;//номер знака,тип линии,тип заливки
  unsigned char minw,maxw;//границы вывода
  unsigned char rezerv ;
  unsigned char atr[16];//номера возможных атрибутов,0-последний
};

//Стуктура файла Flibrt.ras
struct RAS {
  signed char x,y;
  unsigned int  obr[16];
};

//Стуктура файла Flibrt.atn
struct atrname {
  char atr[3];   //имя атрибута
  char name[41]; //расшифрованное имя атрибута
  char type;     //тип атрибута N(numeric),T(text) или K(kod)
  //только для кодовых начало,конец и число записей в файле кодов атрибутов
  unsigned start,end,ncod;
};

#endif
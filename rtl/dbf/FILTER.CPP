#include "base.hpp"
#include "dbf.hpp"

#include <string.h>
//  int Filter_set(char*)    - компиляция строки-фильтра и помещение ее
//                             в стек
//  возвращаемое значение: 0 - нормальное завершение установки фильтра
//                        -1 - база данных не была открыта
//                         1 - ошибка при компиляции
//                         2 - ошибка при интепретации

extern int parse (DBF*, char *, char **, char  * ,char * );
extern void *exec(DBF*, char *, char * , char * );


int pascal DBF::Filter_set(char *_filter)
{  char *compile,*ptr;
   char *eval_space,*eval_end;
   int len_comp;
	 if(!handle) return -1;      // не открыта база данных
	 if ((eval_space=new char[3001])==(char*) 0 ) return -1;
	 eval_end=eval_space+3000;
	 if((len_comp=parse(this,_filter,&compile,eval_space,eval_end))==-1)
		{ delete eval_space; return 1; }     // ошибка при компиляции
	 if((exec(this,compile,eval_space,eval_end))==(void*) 0)
		{ delete eval_space; return 2; }     // ошибка при интерпретации
	 if(!fltr)
	 { filter=compile;
	   *((char**)compile)=(char*) 0;
	 }
	 else
	 { ptr= compile;
	   *((char**)compile)=(char*) filter;
	   filter=ptr;
	 }
	 this->fltr=(char)1;
	 delete eval_space;
	 return 0;
}
void pascal DBF::Filter_reset(void)
{ char *ptr;
	 if(!handle || !filter) return;
	 do{ ptr = *((char**)filter);
	  delete filter;
	  filter = ptr;
	} while(filter);
	 return ;
}
void pascal DBF::Filter_pop(void)
{ char *ptr;
	 if(!handle || !filter) return;
	 ptr = *((char**)filter);
	 delete filter;
	 filter = ptr;
	 return ;
}
//-----------------------------------------------------------------------
//  First(long rec) - поиск первой не удаленной записи, удовлетворяющей
//		      установленному фильтру, начиная с записи rec
//  Next()          - продолжение поиска, с использованием того же шаб-
//                    лона с текущей записи+1
//
//  Возврат:     -1 - ошибка поиска
//                    - не открыта база
//                    - не установлен фильтр
//                    - указан не существующий номер записи
//                0 - конец файла
//		 >0 - номер записи, с установкой на нее указателя в буфере
//
int pascal Filter_do(DBF *file,char *eval_space,char *eval_end)
{  char *compile,*n_compile;
   void *ptr;

	 compile = n_compile = file->filter;
	 do{ if((ptr=exec(file,compile,eval_space,eval_end))==(void*) 0)
			 return -1;
	  if(!*((int*)ptr)) return  0;
	  compile = *((char**)n_compile);
	  n_compile = compile;
	} while(compile);
   return 1;
}
long pascal DBF::Next(int sw)
{ char *eval_space,*eval_end;
    if(!handle || !filter) return -1;

    if ((eval_space=new char[3001])==(char*) 0 ) return -1;
	eval_end=eval_space+3000;
    long i;
    int ret=0;
    for( i= sw ? rec_num : rec_num+1;i<=head.numb_rec;i++)
	  { if((ret=Go(i))==-1) { delete eval_space; return -1;}
	    if(ret==1)  break;
	    if(*buffer=='*') continue;
	    if(!filter) { delete eval_space; return i; }
	 switch (Filter_do(this,eval_space,eval_end))
	   { case -1: delete eval_space; return -1;
		case  1: delete eval_space; return  i;
		case  0: break;
	   }
	  }
    delete eval_space;
    return 0;
}
long pascal DBF::First(long rec)
{ int ret=0;

    if(!handle) return -1;
    if((ret=Go(rec))==-1) return -1;
    if(ret==1)  return  0;
    return Next(1);
}

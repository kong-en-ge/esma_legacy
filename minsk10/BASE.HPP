#ifndef _BASEHPP
#define _BASEHPP
/*  base.hpp   (c)Copyright Sequiter Software Inc., 1987, 1988, 1989.  All rights reserved.  */

// HEAD  - описание заголовка

struct HEAD { unsigned char vers,
			    yy,
			    mm,
			    dd;
	     unsigned long  numb_rec;
		unsigned header_len;
		unsigned rec_width;
			    HEAD();
	    };

// FIELD  - описание полей dbf-файла

struct FIELD  {
	    char    name[11] ;
	    char    type ;
	unsigned    offset ;
	    char    rez[2] ;
     unsigned char  width ,
	            decimals ;
			  FIELD (char*,char,int,int = 0);
		    };

// BASE - описание базы

struct  BASE
{  char    name[64] ;   // путь доступа
   int     handle;      // указатель на файл

   /* Buffer Information */

   long    rec_num  ;   // номер текущей записи
   int     buf_len ;    // длина записи
   long    rec_first;   // номер первой записи в буфере

   char   *old_buf,*buffer  ;    // буфер записи (-ей)

   int     num_fields ; // число полей в базе
   FIELD  *fields ;     // указатель на буфер, хранящий описание полей

   char   *filter ;     // указатель на стек, хранящий псевдокомп. строки
   HEAD    head;        // описание заголовка

};
#endif



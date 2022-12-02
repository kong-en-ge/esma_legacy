/****************************************************************************
*                      ëéáÑÄçàÖ à éíäêõíàÖ îÄâãÄ *.DBF                      *
*****************************************************************************/
#include "base.hpp"
#include "convert.hpp"
#include "dbf.hpp"

#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <io.h>
#include <dos.h>
#include <dir.h>

int pascal DBF::Creat(char *name,int num,FIELD *fields,int safety ) {
   char  *header ;
   HEAD  _head;
   char	 full_name[90], on_char ;
   int	 dos_file,is_memo= 0 ;
   long	 time_val, header_len ;
   struct tm   *tm_ptr ;

   header_len =  (long)num * 32  + 34 ;
   if ( header_len > 0x7fff )
   {
	 //u4error( E_CREATE, "Too Many Fields", name, (char *) 0 ) ;
	 return -1 ;
   }

   name_full( full_name, name, ".DBF" ) ;
   struct ffblk ffblk;
   if (safety&&findfirst(full_name,&ffblk,FA_ARCH)!=-1) {
     int ret=Use(full_name);
     if (ret==-1) return Creat(full_name,num,fields,0);
     if (F_count()==num) {
       FIELD *f=fields;
       for (int i=1;i<=num;i++,f++)
	 if ((F_type(i)!=f->type)||(F_width(i)!=f->width)
	     ||strcmp(F_name(i),f->name)) {
	     ret=-1;
	     break;
	 }
     } else ret=-1;
     if (ret==-1)  return Creat(full_name,num,fields,0);
     return 0;
   }
   header  = new char[(int) header_len] ;
   if ( header == (char *) 0 )  return -1 ;

   memset(header,0,header_len);

   _head.rec_width=1;

   for (int i=0; i< num; i++ )
   {  fields[i].offset = _head.rec_width;

	 fields[i].type=toupper( fields[i].type ) ;

	 if (fields[i].type=='F') fields[i].type='N';
	 if (fields[i].type  != 'D' && fields[i].type != 'L' &&
		fields[i].type != 'N'/* && fields[i].type != 'M' */)
	   fields[i].type =  'C' ;

	 if ( fields[i].type != 'C' )
	    { if ( fields[i].type == 'N')
			{if ( fields[i].width > 19 )  fields[i].width =  19 ;
			 if ( fields[i].width <= 2 )
				    fields[i].decimals =  0 ;
			 else
			 {
			    if ( fields[i].decimals >= fields[i].width-1 )
				    fields[i].decimals =  fields[i].width-2 ;
			 }
			}
	    }

	 if ( fields[i].width < 1 )    fields[i].width = 1 ;
	 if ( fields[i].type == 'L' )  fields[i].width =  1 ;
	 if ( fields[i].type == 'D' )  fields[i].width =  8 ;

/*	 if ( fields[i].type == 'M' )
	 { fields[i].width =  10 ;
	   is_memo =  1 ;
	 }
*/
	 strupr( fields[i].name ) ;

	 for (int j=0; j< 10; j++ ) {
	 on_char =  fields[i].name[j] ;
	 if ( on_char >= 'A' && on_char <= 'Z' )  continue ;
	 if ( on_char >= '0' && on_char <= '9' )  continue ;
	 if ( on_char == '_' ) continue ;

	 memset( fields[i].name+j, 0, (10-j) ) ;
	 break ;
	 }
	 fields[i].name[10] = '\0' ;

	 _head.rec_width +=  fields[i].width ;
	 memcpy(&header[(i+1)*32],&fields[i],sizeof(FIELD));
   }
   memcpy( &header[(i+1)*32] , "\x0D\x1A", 2 ) ;

   dos_file =  open( full_name,O_CREAT|O_TRUNC|O_BINARY|O_RDWR,
				    S_IWRITE ) ;
   if ( dos_file < 0 )
   {
	 delete header;
	 return -1 ;
   }
/*
   if ( is_memo )
	 _head.vers =  (char) 0x83 ;
   else
*/	 _head.vers =  3;

   time( (time_t *) &time_val ) ;
   tm_ptr  =  localtime( (time_t *) &time_val ) ;
   _head.yy =  (char) tm_ptr->tm_year ;
   _head.mm =  (char) tm_ptr->tm_mon+1 ;
   _head.dd =  (char) tm_ptr->tm_mday ;

   _head.header_len =  32*(num+1) + 1 ;

   memcpy(header,&_head,sizeof(HEAD));

   lseek( dos_file, 0L, 0 ) ;
   int rc = ::write( dos_file, header, header_len ) ;
   delete header;
   close( dos_file ) ;
   if ( rc != header_len )
   {
	 //u4error( E_WRITE, full_name, (char *) 0) ;
	 return  -1 ;
   }


/*   if ( is_memo )
   {
	 char   memo_name[90] ;
	 int    memo_file ;
	 MEMO_HEADER  m_hdr ;

	 u4name_part( memo_name, full_name, 1,0 ) ;
	 u4name_full( memo_name, memo_name, ".DBT" ) ;

	 if ( safety )
	 memo_file =  u4open( memo_name, 1 ) ;
	 else
	 memo_file =  u4open( memo_name, 2 ) ;

	 if ( memo_file < 0 )  return -1 ;

	 memset( (char *) &m_hdr, 0, sizeof(m_hdr) ) ;
	 m_hdr.next_block =     1L ;
	 u4name_part( m_hdr.file_name, full_name, 0,0 ) ;
	 m_hdr.x102       =  0x102 ;
	 m_hdr.block_size =  0x200 ;

	 lseek( memo_file, 0L, 0 ) ;
	 if ( write( memo_file, (char *) &m_hdr, (unsigned int) sizeof(m_hdr))
				    != (int) sizeof(m_hdr) )
	 {
	 close( memo_file ) ;
	 u4error( E_WRITE, memo_name, (char *) 0 ) ;
	 return( -1 ) ;
	 }

	 close( memo_file ) ;
   }           */

   return(Use(full_name)) ;
}

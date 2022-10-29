#include <string.h>
#include <stdlib.h>
char *_ltoa(long l_val,char *ptr,int num)
{
   int	 n, num_pos ;
   long  i_long ;

   i_long =  (l_val>0) ? l_val : -l_val ;
   num_pos =  n =  (num > 0) ? num : -num ;

   while (n-- > 0)
   {
	 ptr[n] = (char) ('0'+ i_long%10) ;
	 i_long = i_long/10 ;
   }

   if ( i_long > 0 )
   {
	memset( ptr, (int) '*', (size_t) num_pos ) ;
	return ptr ;
   }

   num--;
   for (n=0; n<num; n++)
	 if (ptr[n]=='0')
	 ptr[n]= ' ';
	 else
	 break ;

   if (l_val < 0)
   {
	 if ( ptr[0] != ' ' )
	 {
	 memset( ptr, (int) '*', (size_t) num_pos ) ;
	 return ptr ;
	 }
	 for (n=num; n>=0; n--)
	 if (ptr[n]==' ')
	 {
	    ptr[n]= '-' ;
	    break ;
	 }
   }

   return(ptr) ;
}
char * dtoa(double doub_val,char *buffer,int len,int dec)
{
   int	 dec_val, sign_val ;
   int	 pre_len, post_len, sign_pos ; /* pre and post decimal point lengths */
   char *result, *out_string ;

   if ( len < 0 )    len = -len  ;
   if ( len > 128 )  len =  128 ;

   memset( buffer, (int) '0', (size_t) len) ;
   out_string =  buffer ;

   if (dec > 0)
   {
      post_len =  dec ;
      if (post_len > 15)     post_len =  15 ;
      if (post_len > len-1)  post_len =  len-1 ;
      pre_len  =  len -post_len -1 ;

      out_string[ pre_len] = '.' ;
   }
   else
   {
      pre_len  =  len ;
      post_len = 0 ;
   }

   result =  fcvt( doub_val, post_len, &dec_val, &sign_val) ;

   if (dec_val > 0)
      sign_pos =   pre_len-dec_val -1 ;
   else
   {
      sign_pos =   pre_len - 2 ;
      if ( pre_len == 1) sign_pos = 0 ;
   }

   if ( dec_val > pre_len ||  pre_len<0  ||  sign_pos< 0 && sign_val)
   {
      /* overflow */
      memset( out_string, (int) '*', (size_t) len) ;
      return( buffer) ;
   }

   if (dec_val > 0)
   {
      memset( out_string, (int) ' ', (size_t) pre_len- dec_val) ;
      memmove( out_string+ pre_len- dec_val, result, dec_val) ;
   }
   else
   {
      if (pre_len> 0)  memset( out_string, (int) ' ', (size_t) (pre_len-1)) ;
   }
   if ( sign_val)  out_string[sign_pos] = '-' ;


   out_string += pre_len+1 ;
   if (dec_val >= 0)
   {
      result+= dec_val ;
   }
   else
   {
      out_string    -= dec_val ;
      post_len += dec_val ;
   }

   if ( post_len > strlen(result) )
      post_len =  strlen( result) ;

   /*  - out_string   points to where the digits go to
       - result       points to where the digits are to be copied from
       - post_len     is the number to be copied
   */

   if (post_len > 0)   memmove( out_string, result, post_len) ;

   buffer[len] =  '\0' ;

   return( buffer ) ;
}

double atod(char *char_string, int string_len )
{
   char buffer[50] ;
   int	len ;

   len =  (string_len >= 50 )  ?  49 : string_len ;
   memcpy( buffer, char_string, len ) ;
   buffer[len] = '\0' ;

   return( strtod( buffer, (char **) 0)   ) ;
}

void  name_full(char* result_name,char* in_name,char* default_extension )
{
   int   len, i ;
   char *ptr ;

   strcpy( result_name, in_name ) ;

   len =  strlen( result_name ) ;
   if ( len > 64 )  len =  64 ;
   while ( len >= 0  &&  (result_name[len-1] == '\0' || result_name[len-1] == ' ') )  len-- ;
   result_name[len] =  '\0' ;

   strupr( result_name ) ;

   for ( i=len-1, ptr= result_name+len-1; i >= 0;  i--,ptr-- )
   {
	 if ( *ptr == '\\' || *ptr == '\/' || *ptr == '.' )  break ;
   }
   if ( i < 0  ||  *ptr != '.' )
   {
	 /* Add Extension */
	 if ( *default_extension == '.' )
	 strcat( result_name + len, default_extension ) ;
	 else
	 {
	 result_name[len++] = '.' ;
	 strcat( result_name + len, default_extension ) ;
	 }
   }
}


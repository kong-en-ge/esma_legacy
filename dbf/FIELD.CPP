/*************************************************************************
 *                          РАБОТА С ПОЛЯМИ                              *
 *************************************************************************/
#include "dbf.hpp"
#include "convert.hpp"
#include <stdlib.h>
#include <string.h>
//----------------------------------------------------------------------
//   F_ref(char) - получение номера поля по его имени
//----------------------------------------------------------------------

int pascal DBF::F_ref(char *f_name)
{
   char  *name=strupr(f_name);

   for(int i=0;strcmp(fields[i].name,name) && i<num_fields;i++);
   if(i<num_fields) return i+1;
   else                   return  -1;
}
int pascal DBF::F_true(int f_ref)
{  char char_value ;
   switch( F_type(f_ref) )
   {
	 case  'N':
	 case  'F':
	 case  'C':
	 if ( F_value( f_ref) == 0.0 )
	    return( 0 ) ;
	 else
	    return( 1 ) ;

	 case  'L':
	 char_value =  *F_ptr( f_ref ) ;
	 if ( char_value == 'Y'  ||  char_value == 'y'  ||
		 char_value == 'T'  ||  char_value == 't'  ||
		 char_value == '1')
		 return( 1 ) ;
	 else
		 return( 0 ) ;
   }
   return( -1 ) ;
}

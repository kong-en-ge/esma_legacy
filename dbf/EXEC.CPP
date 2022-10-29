//************************* исполнение *******************************
#include "dbf.hpp"
#include "parse.h"
#include "convert.hpp"

#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define  RESULT_SIZE   258


char  *expr_on, *expr_string ;
u4error(int kod,char *str,char *str1);
type_error(char op_code,int first_type,int second_type,int third_type);
DBF *__file;

void strnrev(char *s,int s_len )  /* Reverse the character order */
{
   char *s2, c ;
   s2 = s+ s_len -1 ;
   while ( s2> s )
   {
      c     = *s ;
      *s++  = *s2 ;
      *s2-- = c ;
   }
}

void str_blank_cat(char* a,int a_len,int b_len )  /* move blanks to end */
{
   char *s ;
   int	 s_len ;

   s_len =  0 ;
   s =	a+a_len-1 ;
   while ( *s == ' '  &&  s>= a )
   {
      s-- ;
      s_len++ ;
   }
   s++;

   strnrev( s, s_len+b_len ) ;
   strnrev( s, b_len ) ;
   strnrev( s+b_len, s_len) ;
}


/* expr_ptr  is only for error messages.

   256 bytes must be declared for 'result_ptr'

       Result Type	result_ptr format		  Function Return

       Numeric		double				  8 (Length of double)
       Character	string				  Length of String
       Logical		int   ( -1 - TRUE;  0 - FALSE )   2 (Length of int)
       Date		YYYYMMDD			  8 (Length of date)

   Returns (character converted to an (int) )

       N - Numeric Result
       C - Character Result
       L - Logical Result
       D - Date Result
      -1 - Error
*/


int near exec_funct (char* r_ptr,int* r_len,char *eval_end )
{
   int	  a_type, a_len, b_type, b_len, c_type, c_len, len, num_parms, i ;
   union
   {
      char   *ch;
      double *doub;
      int    *i ;
   }  a, b, c ;

   char  op_code ;
   op_code =  *expr_on++ ;

   switch( op_code )
   {
      case A_ADD:
      case A_SUBTRACT:
      case A_MULTIPLY:
      case A_DIVIDE:
      #ifndef UNIX
      case A_POWER:
      #endif
	 a_type =  exec_funct((a.ch= r_ptr), &a_len, eval_end ) ;
	 if ( a_type == ERROR) return( ERROR) ;
	 b_type =  exec_funct((b.ch= r_ptr+ a_len), &b_len, eval_end ) ;
	 if ( b_type == ERROR) return( ERROR) ;
	 if (a_type != b_type  ||
	     a_type != NUM_CODE && a_type != CHAR_CODE )
	 {
	    /* operators in reverse order in epression */
	    //type_error( op_code, b_type, a_type, 0) ;
	    return(ERROR) ;
	 }

	 if ( a_type == NUM_CODE)
	 {
	    /* do not result error because type checking could be being done */

	    *r_len =  sizeof(double) ;

	    switch(op_code)
	    {
	       /* Multiply */
	       case A_MULTIPLY:
		  *a.doub = (*a.doub) * (*b.doub) ;
		  break ;
	       /* Divide */
	       case A_DIVIDE:
		  if (*a.doub != 0.0)
		     *a.doub = (*b.doub)/(*a.doub) ;
		  break ;
	       /* Add */
	       case A_ADD:
		  *a.doub =  (*a.doub)+ (*b.doub) ;
		  break ;
	       /* Subtract */
	       case A_SUBTRACT:
		  *a.doub =  (*b.doub) - (*a.doub) ;
		  break ;
	       #ifndef UNIX
	       /* Power */
	       case A_POWER:
		  *a.doub = pow(*b.doub,*a.doub) ;
		  break ;
	       #endif
	    }
	    return( NUM_CODE) ;
	 }
	 else	/*  CHAR_CODE */
	 {
	    if (op_code != A_ADD &&  op_code != A_SUBTRACT)
	    {
	       //type_error( op_code, a_type, b_type, 0) ;
	       return(ERROR) ;
	    }
	    *r_len =  a_len +  b_len ;
	    strnrev( a.ch, *r_len ) ;
	    strnrev( a.ch, b_len ) ;
	    strnrev( a.ch+b_len, a_len ) ;

	    if (op_code == A_SUBTRACT)	/* concatenation with blank shifting */
		  str_blank_cat( a.ch, b_len, a_len ) ;
	    return( CHAR_CODE) ;
	 }

      case I_FALSE:
      case I_TRUE:
	 *r_len =  sizeof(int) ;
	 if ( eval_end - r_ptr	< sizeof(int) )
	 {
	    u4error( 540, expr_string, (char *) 0 ) ;
	    return( ERROR ) ;
	 }
	 if ( op_code == I_TRUE )
	    *((int *)r_ptr) =	1 ;
	 else
	    *((int *)r_ptr) =	0 ;
	 return( LOG_CODE ) ;

      case I_CHAR:
	 *r_len = (int) *expr_on++ ;
	 if ( eval_end - r_ptr	< *r_len )
	 {
	    u4error( 540, expr_string, (char *) 0 ) ;
	    return( ERROR ) ;
	 }
	 memcpy( r_ptr, expr_on, *r_len) ;
	 expr_on += *r_len ;
	 return( CHAR_CODE ) ;


      case I_NUM:
	 *r_len =  sizeof(double) ;
	 if ( eval_end - r_ptr	< sizeof(double) )
	 {
	    u4error( 540, expr_string, (char *) 0 ) ;
	    return( ERROR ) ;
	 }
	 memcpy( r_ptr, expr_on, sizeof(double) ) ;
	 expr_on += sizeof(double) ;
	 return( NUM_CODE) ;


      case L_OR:
      case L_AND:
	 b_type =  exec_funct((b.ch= r_ptr), &b_len, eval_end ) ;
	 if (b_type == ERROR) return( ERROR) ;
	 a_type =  exec_funct((a.ch= r_ptr+ b_len), &a_len,  eval_end ) ;
	 if (a_type == ERROR) return( ERROR) ;
	 if (a_type != LOG_CODE  ||  b_type != LOG_CODE)
	 {
	    //type_error( op_code, b_type, a_type, 0) ;
	    return( ERROR) ;
	 }
	 *r_len =  sizeof(int) ;
	 if (op_code == L_AND)
	    *((int *) r_ptr) =	*a.i && *b.i ;
	 if (op_code == L_OR)
	    *((int *) r_ptr) =	*a.i || *b.i ;
	 return( LOG_CODE) ;


      case L_NOT:
	 a_type =  exec_funct((a.ch= r_ptr), &a_len, eval_end ) ;
	 if ( a_type == ERROR)	return( ERROR) ;
	 if ( a_type != LOG_CODE)
	 {
	    //type_error( L_NOT, a_type, 0, 0) ;
	    return( ERROR) ;
	 }
	 * ((int*) r_ptr) =  ! *a.i ;
	 *r_len =  sizeof(int) ;
	 return ( LOG_CODE) ;


	 case R_GE:
	 case R_LE:
	 case R_GT:
	 case R_LT:
	 case R_EQ:
	 case R_NE:
	 case R_SUB_COMPARE:
	 b_type =  exec_funct((b.ch= r_ptr), &b_len, eval_end ) ;
	 if ( b_type == ERROR)	  return( ERROR );
	 a_type =  exec_funct((a.ch= r_ptr+b_len), &a_len, eval_end ) ;
	 if ( a_type == ERROR) return( ERROR) ;
	 if (a_type != b_type	 ||
		a_type != CHAR_CODE && op_code == R_SUB_COMPARE   ||
		a_type == LOG_CODE)
	 {
	    //type_error( op_code, b_type, a_type, 0) ;
	    return( ERROR) ;
	 }
	 *r_len =  sizeof(int) ;

	 if ( a_type == NUM_CODE)
	 {
	    if (op_code == R_GE)
		  *((int *)r_ptr) =  *a.doub >= *b.doub ;else
	    if (op_code == R_LE)
		  *((int *)r_ptr) =  *a.doub <= *b.doub ;else
	    if (op_code == R_GT)
		  *((int *)r_ptr) =  *a.doub >  *b.doub ;else
	    if (op_code == R_LT)
		  *((int *)r_ptr) =  *a.doub <  *b.doub ;else
	    if (op_code == R_EQ)
		  *((int *)r_ptr) =  *a.doub == *b.doub ;else
	    if (op_code == R_NE)
		  *((int *)r_ptr) =  *a.doub != *b.doub ;
	    return( LOG_CODE) ;
	 }

	 if ( a_type == CHAR_CODE  ||  a_type == DATE_CODE)
	 {
	    int compare_result ;

	    len =( b_len < a_len ) ? b_len : a_len;

	    compare_result =  strncmpi( a.ch, b.ch, len ) ;

	    if (op_code == R_GE)
		  *((int *)r_ptr) =  compare_result >= 0 ;else
	    if (op_code == R_LE)
		  *((int *)r_ptr) =  compare_result <= 0 ;else
	    if (op_code == R_GT)
		  *((int *)r_ptr) =  compare_result >  0 ;else
	    if (op_code == R_LT)
		  *((int *)r_ptr) =  compare_result <  0 ;else
	    if (op_code == R_EQ)
		  *((int *)r_ptr) =  compare_result == 0 ;else
	    if (op_code == R_NE)
		  *((int *)r_ptr) =  compare_result != 0 ;else
	    if (op_code == R_SUB_COMPARE)
	    {
		  if ( a_len <= 0 )
		  {
		  *((int *) r_ptr) =  1 ;
		  return ( LOG_CODE ) ;
		  }

		  /* See if there is a match */
		  for ( i=0; i <= b_len-a_len; i++ )
		  if ( *a.ch == b.ch[i] )
			if ( memcmp( a.ch, b.ch+i, a_len ) == 0 )
			{
			*((int *) r_ptr) =  1 ;
			return ( LOG_CODE ) ;
			}

		  *((int *) r_ptr) =  0 ;
		  return ( LOG_CODE ) ;
	    }
	    return( LOG_CODE) ;
	 }

	 case F_DATE:
	 case F_TIME:
	 case F_DTOC:
	 case F_DTOS:
	 case F_CTOD:
	 case F_RECNO:
	 case F_RECCOUNT:
	 case F_STR:
	 case F_SUBSTR:
	 case F_VAL:
	 case F_IIF:
	 case F_DELETED:
	 case F_DEL:
	 case F_UPPER:
	 if ( eval_end - r_ptr	< 10 )
	 {
	    u4error( 540, expr_string, (char *) 0 ) ;
	    return( ERROR ) ;
	 }
	 *r_len =  8 ;	/* default */
	 num_parms =  (int)  (*expr_on++) ;
	 a_type =  b_type =  c_type = -1 ;   /* -1 means missing */
	 a_len	=  b_len  =  c_len  =  0 ;
	 while( num_parms >= 3)
	 {
	    num_parms-- ;
	    c_type =  exec_funct((c.ch= r_ptr), &c_len, eval_end ) ;
	    if (c_type == ERROR)   return( ERROR) ;
	 }
	 if ( num_parms >= 2)
	 {
	    b_type =  exec_funct((b.ch= r_ptr+ c_len), &b_len, eval_end ) ;
	    if (b_type == ERROR)   return( ERROR) ;
	 }
	 if ( num_parms >= 1)
	 {
	    a_type =  exec_funct((a.ch= r_ptr+ c_len+b_len), &a_len, eval_end ) ;
	    if (a_type == ERROR)   return( ERROR) ;
	 }

	 if (op_code == F_DATE || op_code == F_TIME)
	 {
	    long time_val ;
	    struct tm  *tm_ptr ;

	    time ( (time_t *) &time_val) ;
	    tm_ptr =  localtime( (time_t *) &time_val) ;

	    if (op_code == F_TIME)
	    {
		  _ltoa( (long) tm_ptr->tm_hour, r_ptr, -2) ;
		  r_ptr+= 2;
		  *r_ptr++ = ':' ;
		  _ltoa( (long) tm_ptr->tm_min, r_ptr, -2) ;
		  r_ptr+= 2;
		  *r_ptr++ = ':' ;
		  _ltoa( (long) tm_ptr->tm_sec, r_ptr, -2) ;
		  r_ptr+= 2;
		  return( CHAR_CODE) ;
	    }

	    if (op_code == F_DATE)
	    {
		  _ltoa( (long) tm_ptr->tm_year+1900, r_ptr, -4) ;
		  r_ptr+= 4;
		  _ltoa( (long) tm_ptr->tm_mon+1, r_ptr, -2) ;
		  r_ptr+= 2;
		  _ltoa( (long) tm_ptr->tm_mday, r_ptr, -2) ;
		  r_ptr+= 2;
		  return( DATE_CODE) ;
	    }
	 }

	 if (op_code == F_DTOC || op_code == F_DTOS )
	 {
	    if ( a_type != DATE_CODE )
	    {
		  //type_error( op_code, a_type, 0, 0) ;
		  return( ERROR) ;
	    }
	    memmove( r_ptr, a.ch, 8 ) ;
	    return( CHAR_CODE) ;
	 }

	 if (op_code == F_CTOD)
	 {
	    if ( a_type != CHAR_CODE )
	    {
		  //type_error( op_code, a_type, 0, 0) ;
		  return( ERROR) ;
	    }
	    memmove( r_ptr, a.ch, 8 ) ;
	    return( DATE_CODE) ;
	 }

	 if (op_code == F_RECNO)
	 {
	    *((double *)r_ptr) =  (double) __file->rec_num ;
	    return( NUM_CODE) ;
	 }

	 if (op_code == F_RECCOUNT)
	 {
	    *((double *)r_ptr) =  (double) __file->head.numb_rec ;
	    return( NUM_CODE) ;
	 }

	 if (op_code == F_DELETED)
	 {
	    if (*__file->buffer == '*')
		  *((int*)r_ptr) =  1 ;
	    else
		  *((int*)r_ptr) =  0 ;
	    *r_len =  sizeof(int) ;
	    return( LOG_CODE) ;
	 }

	 if (op_code == F_DEL)
	 {
	    *r_ptr =  *(__file->buffer) ;
	    *r_len =  1 ;
	    return( CHAR_CODE) ;
	 }

	 if (op_code == F_STR)
	 {
	    int  dec ;
	    char buffer[130];
	    /* defaults are length of 10 and dec of 2 */
	    if ( a_type != NUM_CODE    ||
		 b_type != -1 &&  b_type != NUM_CODE  ||
		 c_type != -1 &&  c_type != NUM_CODE)
	    {
		  //type_error( op_code, a_type, b_type, c_type) ;
		  return ( ERROR) ;
	    }
	    if (b_type == -1)
		  len =  10 ;
	    else
		  len =  (int) *b.doub ;

	    if (c_type == -1)
		  dec =   2 ;
	    else
		  dec =  (int) *c.doub ;

	    if ( eval_end - r_ptr  < len )
	    {
		  u4error( 540, expr_string, (char *) 0 ) ;
		  return( ERROR ) ;
	    }
	    strcpy( r_ptr, dtoa( *a.doub, buffer, len, dec) ) ;
	    *r_len =  len ;
	    return( CHAR_CODE) ;
	 }

	 if ( op_code == F_VAL)
	 {
	    if (a_type != CHAR_CODE)
	    {
		  //type_error( op_code, a_type, 0,0 ) ;
		  return( ERROR) ;
	    }

	    *((double *)r_ptr) =   strtod( a.ch, (char **) 0) ;
	    return (NUM_CODE) ;
	 }

	 if ( op_code == F_SUBSTR)
	 {
	    int b_val, c_val ;

	    if (a_type!= CHAR_CODE || b_type!= NUM_CODE || c_type!= NUM_CODE)
	    {
		  //type_error( op_code, a_type, b_type, c_type) ;
		  return( ERROR );
	    }
	    b_val =  (int) *b.doub ;
	    c_val =  (int) *c.doub ;

	    if ( b_val < 1)	b_val =  1 ;
	    if ( b_val > a_len) b_val =  a_len ;
	    if ( b_val+c_val-1 > a_len)  c_val =  a_len+1-b_val ;

	    memmove( r_ptr, a.ch+ b_val-1, c_val) ;
	    *r_len =  c_val ;

	    return (CHAR_CODE) ;
	 }

	 if ( op_code == F_IIF)
	 {
	    if (a_type!= LOG_CODE || b_type == -1 ||  c_type == -1
				  ||  b_type != c_type)
	    {
		  //type_error( op_code, a_type, b_type, c_type) ;
		  return( ERROR) ;
	    }
	    if ( *a.i)
	    {
		  memcpy( r_ptr, b.ch, b_len ) ;
		  *r_len =  b_len ;
	    }
	    else
	    {
		  memcpy( r_ptr, c.ch, c_len ) ;
		  *r_len =  c_len ;
	    }
	    return( b_type) ;
	 }

	 if ( op_code == F_UPPER )
	 {
	    if ( a_type != CHAR_CODE )
	    {
		  //type_error( op_code, a_type, 0, 0) ;
		  return( ERROR) ;
	    }
	    memmove( r_ptr, a.ch, a_len ) ;
	    r_ptr[a_len] =  '\0' ;
	    strupr( r_ptr ) ;
	    *r_len =  a_len ;
	    return( CHAR_CODE) ;
	 }

	 case F_BASE_FIELD:
	 {
	 long	field_num ;
	 char	f_type ;

	 field_num= *((long *)expr_on)++ ;
	 f_type = __file->F_type( field_num ) ;
	 if ( f_type == 'F' )  f_type = 'N' ;

	 *r_len =  -1 ;
	 if ( f_type == 'N' || f_type == 'D' )  *r_len =  8 ;
	 if ( f_type == 'L' ) *r_len =  2 ;
	 if ( f_type == 'C' ) *r_len =  __file->F_width(field_num) ;

	 if ( *r_len < 0 )
	 {
	    u4error( 580, expr_string, (char *) 0 ) ;
	    return( ERROR ) ;
	 }
	 if ( eval_end - r_ptr  < *r_len )
	 {
	    u4error( 540, expr_string, (char *) 0 ) ;
	    return( ERROR ) ;
	 }

	 if (f_type == 'N')
	    *((double *)r_ptr)  = __file->F_value( field_num ) ;
	 if (f_type == 'L')  *((int *)   r_ptr)  =  __file->F_true( field_num ) ;
	 if (f_type == 'D' ||  f_type == 'C' )
		 memcpy( r_ptr, __file->F_ptr(field_num), *r_len) ;

	 return( (int) f_type ) ;
	 }
   }
   return( ERROR ) ;
   /* END Switch */
}

char * type_str(int type)
{
   switch( type)
   {
	 case NUM_CODE:  return("Numeric") ;
	 case CHAR_CODE: return("Character") ;
      case LOG_CODE:  return("Logical") ;
      case DATE_CODE: return("Date") ;
   }
   return( (char *) 0) ;
}


/* type_error calls u4error with an appropriate error message. */

/*type_error(char op_code,int first_type,int second_type,int third_type)
{
   char *first_ptr, *second_ptr, *third_ptr ;

   first_ptr = second_ptr = third_ptr  =  (char *) 0 ;

   if ( first_type != 0 )
      first_ptr = "First Operand Type:" ;
   if ( second_type != 0 )
      second_ptr= "Second Operand Type:" ;
   if ( third_type != 0)
      third_ptr = "Third Operand Type:" ;

   u4error( E_TYPE,
	"Expression:",
	 expr_string,

	"Operator or Function:",
	 name(op_code),

	 first_ptr,
	 type_str( first_type),

	 second_ptr,
	 type_str( second_type),

	 third_ptr,
	 type_str( third_type),

	 (char *) 0) ;

   return(0) ;
}       */

void *exec(DBF* file, char *compile_ptr,char *eval_space,char *eval_end )
{
   int	 rc,eval_len ;
   __file=file;
   expr_on = (compile_ptr+sizeof(char*)) ;
   if (expr_on == ((char *) 0)	)
   {
      u4error( 0, (char *) 0,(char *) 0) ;
      return( (void *) 0) ;
   }

   rc =  exec_funct(eval_space, &eval_len, eval_end ) ;
   if ( eval_end - eval_space > eval_len )
	 eval_space[eval_len] =	'\0' ;

   if((char) rc !='L' || rc == ERROR )
      return( (void *) 0) ;
   else
      return( (void *) eval_space ) ;
}

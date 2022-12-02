
/*  e3parse.h */


/*   Integer Codes */

/* Data Types
   NUM_CODE   is in (double) format
   CHAR_CODE  1byte length;  length worth of character data
   DATE_CODE  CCYYMMDD
   LOG_CODE   as (int)
*/

#define  NUM_CODE  ((int)'N')
#define  CHAR_CODE ((int)'C')
#define  DATE_CODE ((int)'D')
#define  LOG_CODE  ((int)'L')

/* Special Return Codes */

#define  OK          0
#define  ERROR      -1
#define  DONE       -2




/* Function Numbers (Character Codes) */

/* Immediate Functions */

#define  I_TRUE            '\x0'
#define  I_FALSE           '\x1'
#define  I_CHAR            '\x2'
#define  I_NUM             '\x3'

/* Arithmetic & Concatenation Functions  +,-,*,/,**,^ */

#define  A_ADD             '\x4'
#define  A_SUBTRACT        '\x5'
#define  A_MULTIPLY        '\x6'
#define  A_DIVIDE          '\x7'
#define  A_POWER           '\x8'



/* Logical Operators  .AND., .OR., .NOT.  */

#define  L_AND             '\x9'
#define  L_OR              '\xA'
#define  L_NOT             '\xB'

/* Relational Operators >=, <=, >, <, =, <>, #, $  */

#define  R_GE              '\xC'
#define  R_LE              '\xD'
#define  R_GT              '\xE'
#define  R_LT              '\xF'
#define  R_EQ              '\x10'
#define  R_NE              '\x11'
#define  R_SUB_COMPARE     '\x12'


/* Functions - dBase functions as well */

#define  F_DATE       '\x13'
#define  F_DTOC       '\x14'
#define  F_CTOD       '\x15'
#define  F_RECNO      '\x16'
#define  F_RECCOUNT   '\x17'
#define  F_TIME       '\x18'
#define  F_STR        '\x19'
#define  F_VAL        '\x1A'
#define  F_SUBSTR     '\x1B'
#define  F_IIF        '\x1C'
#define  F_DELETED    '\x1D'
#define  F_DEL        '\x1E'
#define  F_UPPER      '\x1F'
#define  F_DTOS       '\x20' 

/* Special Functions */

#define  F_BASE_FIELD '\x51'


/* Quasi Operators */

#define  Q_NO_FUNCTION  '\xFE'
#define  Q_L_BRACKET    '\xFD'
#define  Q_R_BRACKET    '\xFC'
#define  Q_COMMA        '\xFB'


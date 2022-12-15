/*  parse.cpp	(c)Copyright Sequiter Software Inc., 1987, 1988, 1989.  All rights reserved.

*/

#include "base.hpp"
//#include "u4error.h"
#include "parse.h"
#include "dbf.hpp"

#include <string.h>
#include <stdlib.h>

int parse_expr();
int funct_num(char *, int);
int cur_operator(void);
int parse_function();
int parse_value();
int get_operator(char *);
int push_oper(char);
int push_check(void);
int push_double(double);
int push_long(long);
int push_char(char);
int push_str(char const *, int);
char pop_oper(void);

char *result_bottom, *oper, *oper_top, *oper_bottom;
char const *source, *err_source;

extern DBF *__file;

//************************* компиляция *******************************
static union
{
   char *ch;
   double *doub;
   int *i;
   long *l;
} result;

int u4error(int, char *, char *)
{
   return 0;
}

int name_char(char ch)
{
   return (ch >= 'a' && ch <= 'z' ||
           ch >= 'A' && ch <= 'Z' ||
           ch >= '0' && ch <= '9' ||
           ch == '\\' || ch == '.' || ch == '_');
}

int priority(char ch)
{
   switch (ch)
   {
   case Q_NO_FUNCTION: /* for no operator on stack -  never pop */
      return (0);

   case Q_COMMA:
      return (1);
   case Q_L_BRACKET:
      return (2);

   case L_OR:
      return (3);
   case L_AND:
      return (4);
   case L_NOT:
      return (5);

   case R_GE:
   case R_LE:
   case R_GT:
   case R_LT:
   case R_NE:
   case R_EQ:
   case R_SUB_COMPARE:
      return (6);

   case A_ADD:
   case A_SUBTRACT:
      return (7);

   case A_MULTIPLY:
   case A_DIVIDE:
      return (8);

   case A_POWER:
      return (9);

   case Q_R_BRACKET:
      return (100);

   default:
      return (7);
   }
}

int funct_num(char const *str, int len)
{
   char u_str[10];

   strncpy(u_str, str, sizeof(u_str));
   u_str[9] = '\0';
   strupr(u_str);

   if (len == 3)
   {
      if (strncmp(u_str, "STR", 3) == 0)
         return (F_STR);
      if (strncmp(u_str, "VAL", 3) == 0)
         return (F_VAL);
      if (strncmp(u_str, "IIF", 3) == 0)
         return (F_IIF);
      if (strncmp(u_str, "DEL", 3) == 0)
         return (F_DEL);
   }
   if (len == 4)
   {
      if (strncmp(u_str, "DATE", 4) == 0)
         return (F_DATE);
      if (strncmp(u_str, "DTOC", 4) == 0)
         return (F_DTOC);
      if (strncmp(u_str, "DTOS", 4) == 0)
         return (F_DTOS);
      if (strncmp(u_str, "CTOD", 4) == 0)
         return (F_CTOD);
      if (strncmp(u_str, "TIME", 4) == 0)
         return (F_TIME);
   }
   if (len == 5)
   {
      if (strncmp(u_str, "RECNO", 5) == 0)
         return (F_RECNO);
      if (strncmp(u_str, "UPPER", 5) == 0)
         return (F_UPPER);
   }
   if (len == 6)
      if (strncmp(u_str, "SUBSTR", 6) == 0)
         return (F_SUBSTR);
   if (len == 7)
      if (strncmp(u_str, "DELETED", 7) == 0)
         return (F_DELETED);
   if (len == 8)
      if (strncmp(u_str, "RECCOUNT", 8) == 0)
         return (F_RECCOUNT);

   return (Q_NO_FUNCTION);
}

int push_oper(char oper_code)
{
   *(--oper) = oper_code;
   if (oper < oper_bottom)
      return -1;
   // u4error( E_OVERFLOW, err_source, (char *) 0 ) ;
   return 0;
}

char pop_oper()
{
   if (oper >= oper_top)
      return ('\0');
   return (*(oper++));
}

int cur_oper()
{
   if (oper >= oper_top)
      return (Q_NO_FUNCTION);
   return (*oper);
}

int push_check()
{
   if (result.ch < result_bottom)
      return -1;
   // u4error( E_OVERFLOW, err_source, (char *) 0) ;
   return 0;
}

int push_double(double doub)
{
   *(--result.doub) = doub;
   push_char((char)I_NUM);
   return push_check();
}

int push_long(long l)
{
   *(--result.l) = l;
   return push_check();
}

int push_char(char ch)
{
   *(--result.ch) = ch;
   return push_check();
}

int push_str(char const *str, int len)
{
   if (len > 255 || len < 0)
      return -1;
   // u4error( E_OVERFLOW, err_source, (char *) 0) ;
   result.ch -= len;
   if (push_check() == -1)
      return -1;
   memmove(result.ch, str, len);
   if (push_char((char)len) == -1)
      return -1;
   if (push_char(I_CHAR) == -1)
      return -1;
   return 0;
}

/* Returns  OK, ERROR */
int parse_function()
{
   int i, f_num, num_param;

   i = 0;
   while (name_char(source[i]))
      i++;

   f_num = funct_num(source, i);
   if (f_num == Q_NO_FUNCTION)
      return -1;
   // u4error( E_FUNCTION, err_source, (char *) 0) ;

   source += i;
   while (*source != '(')
      source++;
   source++;

   if (push_oper(Q_L_BRACKET) == -1)
      return -1;

   num_param = 0;

   while (1)
   {
      if (*source == '\0')
         return -1;
      // u4error( E_RIGHT, err_source, (char *) 0) ;

      if (*source == ')')
      {
         source++;
         break;
      }

      if (parse_expr() == ERROR)
         return (ERROR);
      num_param++;

      while (*source <= ' ' && *source >= '\1')
         source++;
      if (*source == ')')
      {
         source++;
         break;
      }
      if (*source != ',')
         return -1;
      // u4error( E_EXPECT, err_source, (char *) 0) ;

      source++;
   }
   pop_oper(); /* pop the left bracket */

   if (push_char((char)num_param) == -1)
      return -1;
   if (push_char((char)f_num) == -1)
      return -1;
   return (OK);
}

/*  Returns:  OK, ERROR */
int parse_value()
{
   while (*source <= ' ' && *source >= '\1')
      source++;

   /* expression */
   if (*source == '(')
   {
      ++source;
      if (push_oper(Q_L_BRACKET) == -1)
         return -1;
      if (parse_expr() == ERROR)
         return (-1);
      while (*source <= ' ' && *source >= '\1')
         source++;
      if (*source++ != ')')
         return -1;
      // u4error( E_RIGHT, err_source, (char *) 0) ;

      pop_oper();
      return (OK);
   }

   /* logical */
   if (*source == '.')
   {
      char log_str[8];

      strncpy(log_str, source, 7);
      log_str[7] = '\0';
      strupr(log_str);
      if (strncmp(log_str, ".TRUE.", 6) == 0)
      {
         if (push_char(I_TRUE) == -1)
            return -1;
         source += 6;
         return (OK);
      }
      if (strncmp(log_str, ".FALSE.", 7) == 0)
      {
         if (push_char(I_FALSE) == -1)
            return -1;
         source += 7;
         return (OK);
      }
      if (strncmp(log_str, ".T.", 3) == 0)
      {
         if (push_char(I_TRUE) == -1)
            return -1;
         source += 3;
         return (OK);
      }
      if (strncmp(log_str, ".F.", 3) == 0)
      {
         if (push_char(I_FALSE) == -1)
            return -1;
         source += 3;
         return (OK);
      }
      if (strncmp(log_str, ".NOT.", 5) == 0)
      {
         /* special case of a one operand oper */
         source += 5;
         if (parse_value() == ERROR)
            return (ERROR);
         if (push_char(L_NOT) == -1)
            return -1;
         return (OK);
      }
   }

   /* function or base/field */
   if (name_char(*source))
   { /* Must be a function or a  base,field */
      char f_name[11];
      int i, j;
      long f_num;

      i = 0;
      while (name_char(source[i]))
         i++;
      j = i;
      while (source[j] <= ' ' && source[j] >= '\1')
         j++;

      /* Function */
      if (source[j] == '(')
         return (parse_function());

      if (i <= 10)
      {
         memmove(f_name, source, i);
         f_name[i] = '\0';
         f_num = __file->F_ref(f_name);
         if (f_num >= 0)
         {
            source += i;

            if (push_long(f_num) == -1)
               return -1;
            if (push_char(F_BASE_FIELD) == -1)
               return -1;
            return (OK);
         }
      }
   }

   /* real */
   if (*source >= '0' && *source <= '9' || *source == '-' || *source == '+')
   {
      if (push_double(strtod(source, &(char*)source)) == -1)
         return -1;
      if (strcmp(source - 1, ".AND.") == 0 || strcmp(source - 1, ".OR.") == 0 ||
          strcmp(source - 1, ".NOT.") == 0)
         source--;
      return (OK);
   }
   /* string */
   if (*source == '\'' || *source == '\"')
   {
      char end_char;
      char const *on;
      int len;

      on = source;
      end_char = *on++;
      len = 0;
      while (on[len] != end_char && on[len] != '\0')
         len++;

      if (on[len] == '\0')
         return -1;
      // u4error( E_STRING_LONG, err_source, (char *) 0 ) ;

      push_str(++source, len);
      source += len + 1;
      return (OK);
   }
   // u4error( E_VALUE, err_source, (char *) 0) ;
   return -1;
}

/*    Looks at the input string and returns and puts a character code on the
   result stack corresponding to the next oper.  The opers all operate
   on two operands.  Ex. +,-,*,/, >=, <, .AND., ...

    If the oper is ambiguous, return the arithmatic choice.

   Returns DONE, OK, ERROR
*/

int get_oper(char *op_return)
{
   char op;

   op = '\xFF';

   while (*source <= ' ' && *source >= '\1')
      source++;
   if (*source == '\0' || *source == ')' || *source == ',')
      return (DONE);

   switch (*source)
   {
   case '+':
      op = A_ADD;
      source++;
      break;
   case '-':
      op = A_SUBTRACT;
      source++;
      break;

   case '/':
      op = A_DIVIDE;
      source++;
      break;
   case '*':
      source++;
#ifndef UNIX
      if (*source == '*')
      {
         op = A_POWER;
         source++;
      }
      else
#endif
         op = A_MULTIPLY;
      break;
   case '^':
      op = A_POWER;
      source++;
   case '.':
      if (strnicmp(source, ".AND.", 5) == 0)
      {
         op = L_AND;
         source += 5;
      }
      if (strnicmp(source, ".OR.", 4) == 0)
      {
         op = L_OR;
         source += 4;
      }
      if (strnicmp(source, ".NOT.", 5) == 0)
      {
         op = L_NOT;
         source += 5;
      }
      break;
   case '<':
      source++;
      if (*source == '=' || *source == '>')
      {
         if (*source == '=')
            op = R_LE;
         else
            op = R_NE;
         source++;
      }
      else
         op = R_LT;
      break;
   case '>':
      source++;
      if (*source == '=')
      {
         op = R_GE;
         source++;
      }
      else
         op = R_GT;
      break;
   case '#':
      op = R_NE;
      source++;
      break;
   case '=':
      op = R_EQ;
      source++;
      break;
   case '$':
      op = R_SUB_COMPARE;
      source++;
      break;
   }

   if (op == '\xFF')
      return -1;
   // u4error( E_OPERATOR, err_source, (char *) 0) ;
   else
   {
      *op_return = op;
      return (OK);
   }
}

/*
   Parses an expression constisting of	 value [[oper value] ...]
   The expression is ended by a ')', a ',' or a '\0'.
   Operators are only popped until a '(', a ',' or the start of the stack.
   Left to right evaluation for opers of equal priority.

      An ambiguous oper is one which can be interpreted differently
   depending on its operands.  However, its operands depend on the
   priority of the opers and the evaluation order.	Fortunately, the
   priority of an ambigous oper is constant regardless of its
   interpretation.  Consequently, the evaluation order is determined first.
   Then ambiguous opers can be exactly determined.

   Ambigous opers:	+, -,  >, <, <=, >=, =, <>, #

   Return

       0  Normal
      -1  Error
*/
int parse_expr()
{
   int rc;
   char op_value;

   if (ERROR == parse_value())
      return (ERROR);

   while (1)
   {
      rc = get_oper(&op_value);
      if (rc == ERROR)
         return (ERROR);
      if (rc == DONE)
      {
         while (cur_oper() != Q_L_BRACKET && cur_oper() != Q_COMMA && cur_oper() != Q_NO_FUNCTION)
         {
            if (push_char(pop_oper()) == -1)
               return -1;
         }
         return (OK);
      }

      /* Everything with a higher or equal priority than 'op_value' must be
      exectuted first. (equal because of left to right evaluation order)
      Consequently, all high priority opers are sent to the result
      stack.
      */
      while (priority(op_value) <= priority((char)cur_oper()))
      {
         if (push_char(pop_oper()) == -1)
            return -1;
      }
      if (push_oper(op_value) == -1)
         return -1;

      if (parse_value() == -1)
         return -1;
   }
}

/*
   parse

   Parameter Name    Type      Purpose

   base_ref	     int       The expression will be parsed assuming
                'base_ref' refers to the active database.
   expr_ptr	     char *    Points to the source expression.
   compile_ptr_ptr   char **   The value of the memory allocated
              compile string will be returned through
              'compile_ptr_ptr'.
   Function Returns

    >=0   The length of the compiled expression
     <0   Error
*/

int parse(DBF *file, char const *expr_ptr, char **compile_ptr_ptr,
          char *eval_space, char *eval_end)
{
   __file = file;
   int OPERATOR_LEN = 64;
   int len_used;
   err_source = source = expr_ptr;

   oper = oper_top = eval_space + OPERATOR_LEN;
   result.ch = eval_end;

   oper_bottom = eval_space;
   result_bottom = eval_space + OPERATOR_LEN;

   if (parse_expr() == -1)
      return -1;

   if (cur_oper() != Q_NO_FUNCTION)
      return -1;
   // u4error( E_COMPLETE, expr_ptr, (char *) 0) ;

   for (int i = 0; i < sizeof(char *); i++)
      push_char(' ');

   len_used = (int)(eval_end - result.ch);
   *compile_ptr_ptr = new char[len_used + sizeof(char *)];
   if (*compile_ptr_ptr == (char *)0)
      return (-1);

   memcpy(*compile_ptr_ptr, result.ch, len_used);

   return (len_used);
}

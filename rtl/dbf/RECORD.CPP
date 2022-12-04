/**********************************************************************
 *					   РАБОТА С ЗАПИСЯМИ                         *
 **********************************************************************/
#include <io.h>
#include <string.h>

#include "base.hpp"
#include "dbf.hpp"
#include "convert.hpp"
//------------------------------------------------------------------
//  Go(long) - перемещение указателя на нужную запись;
//             номера записей 1...max
//------------------------------------------------------------------
char *delprb(char *); // return dest удаляет ведущие и замыкающие пробелы

int DBF::Go(long rec_num)
{
	if (!handle)
		return -1;
	if (rec_num < 1 || rec_num > head.numb_rec)
		return (1);
	if (rec_num == this->rec_num)
		return 0;

	if (rec_num >= rec_first && rec_num < rec_first + b_size)
		buffer += (rec_num - this->rec_num) * buf_len;
	else
	{
		buffer = old_buf + buf_len;
		lseek(handle, head.header_len + (rec_num - 1) * buf_len, 0);
		if (read(handle, buffer, buf_len * b_size) == -1)
			return (-1);
		rec_first = rec_num;
	}
	this->rec_num = rec_num;
	return 0;
	// return (*buffer=='*') ? 2 : 0;
}

//---------------------------------------------------------------------
// R_write(void) - физическая запись в файл текущей записи
//---------------------------------------------------------------------

int DBF::R_write(void)
{
	lseek(handle, head.header_len + (rec_num - 1) * buf_len, 0);
	write = 1;
	return ::write(handle, buffer, buf_len) == -1 ? -1 : 0;
}

//---------------------------------------------------------------------
// R_put(void *) - заполнение записи значениями из структуры с
//                 копированием ее в файл
//---------------------------------------------------------------------

int DBF::R_put(void *buff)
{
	for (int i = 1; i <= num_fields; i++)
	{
		int len = F_width(i);
		switch (F_type(i))
		{
		case 'L':
		case 'D':
		case 'C':
			memset(F_ptr(i), ' ', len);
			if (len == 1)
			{
				*F_ptr(i) = *(char *)buff;
				(char *)buff += 1;
			}
			else
			{
				if (strlen((char *)buff) < len)
					strncpy(F_ptr(i), (char *)buff, strlen((char *)buff));
				else
					strncpy(F_ptr(i), (char *)buff, len);
				(char *)buff += len + 1;
			}
			break;
		case 'F':
		case 'N':
			char buffer[130];
			dtoa(*((double *)buff), buffer, F_width(i), F_decim(i));
			if (*buffer == '*')
				return -1;
			strncpy(F_ptr(i), buffer, F_width(i));
			(char *)buff += sizeof(double);
			break;
		default:
			return -1;
		}
	}
	return R_write();
}

//---------------------------------------------------------------------
// R_get(void *) - заполнение структуры значениями из записи
//
//---------------------------------------------------------------------

int DBF::R_get(void *buff)
{
	for (int i = 1; i <= num_fields; i++)
	{
		int len = F_width(i);
		switch (F_type(i))
		{
		case 'L':
		case 'D':
		case 'C':
			if (len == 1)
			{
				*(char *)buff = *F_ptr(i);
				(char *)buff += 1;
			}
			else
			{
				memset(buff, 0, len + 1);
				strncpy((char *)buff, F_ptr(i), len);
				delprb((char *)buff);
				(char *)buff += len + 1;
			}
			break;
		case 'F':
		case 'N':
			memset(buff, 0, sizeof(double));
			*((double *)buff) = atod(F_ptr(i), len);
			(char *)buff += sizeof(double);
			break;
		default:
			return -1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------
// R_blank( void ) - добавление чистой записи в конец файла *.DBF
//                   с установкой указателя в буфере на нее
//-------------------------------------------------------------------
long DBF::R_blank(void)
{
	char buff[] = "\x1A";
	long mem = rec_num;
	buffer = old_buf + buf_len;
	memset(buffer, ' ', b_size * buf_len);
	rec_num = ++(head.numb_rec);
	if (R_write() == -1 || ::write(handle, buff, strlen(buff)) == -1)
	{
		head.numb_rec--;
		rec_num = mem;
		Go(rec_num = mem);
		return -1;
	}
	return rec_first = rec_num;
}
//-------------------------------------------------------------------
// R_del( void ) - логическое удаление записи
//-------------------------------------------------------------------

long DBF::R_del(void)
{
	*buffer = '*';
	del = 1;
	if (R_write() == -1)
		return -1;
	return rec_num;
}
//-------------------------------------------------------------------
// R_undel( void ) - отмена логического удаления записи
//-------------------------------------------------------------------

long DBF::R_undel(void)
{
	*buffer = ' ';
	if (R_write() == -1)
		return -1;
	return rec_num;
}

//-------------------------------------------------------------------
// R_copy ( long ) - копирование текущей записи в другую без установки
//                   указателя на измененную запись
//-------------------------------------------------------------------
long DBF::R_copy(long rec_num)
{
	long mem = rec_num;
	memcpy(old_buf, buffer, buf_len);
	if (Go(rec_num) != 0)
		return -1;
	memcpy(buffer, old_buf, buf_len);
	if (R_write() == -1)
	{
		Go(rec_num);
		Go(mem);
		return -1;
	}
	if (Go(mem) != 0)
		return -1;
	return mem;
}

//чтение с заполнением,int !=0-
//перейти к rec записи,=0 - перейти к следующей
//возвращает номер записи,-1 если ошибка,0 - конец файла
long DBF::R_get_next(void *buff, long rec)
{
	long ret = 0;
	if (rec)
	{
		if ((ret = Go(rec)) == -1)
			return -1l;
		if (ret != 0)
			return 0;
		goto l2;
	}
l1:
	if ((ret = Go(rec_num + 1)) == -1)
		return -1;
	else if (ret)
		return 0;
l2:
	if (*buffer == '*')
		goto l1;
	R_get(buff);
	return rec_num;
}

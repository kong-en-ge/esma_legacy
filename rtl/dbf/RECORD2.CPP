/**********************************************************************
 *					   РАБОТА С ЗАПИСЯМИ                         *
 **********************************************************************/
#include <io.h>
#include <string.h>

#include "base.hpp"
#include "dbf.hpp"
#include "convert.hpp"

long DBF::R_search(long rec, int (*func)(DBF *ptr))
{
	int ret;
	for (long il = rec; il <= R_count(); il++)
	{
		if ((ret = Go(il)) == -1)
			return -1;
		if (ret == 1)
			return 0;
		if (func(this))
			return il;
	}
	return 0;
}
//--------------------------------------------------------------------
//  R_search(long,char*,void*,...) - поиск записи с использованием
//                                   шаблонов для каждого поля, объе-
//                                   диненных по AND
//                                   Последним параметром всегда NULL!
//--------------------------------------------------------------------

long DBF::R_search(long rec, char const *name, void const *templ, ...)
{
	char const **ptr = &name;
	int *num;

	if (!handle)
		return -1;

	if ((num = new int[num_fields]) == NULL)
		return -1;

	for (int i = 0; i < F_count() && *ptr; i++)
	{
		if ((num[i] = F_ref(*ptr)) == -1)
		{
			delete num;
			return -1;
		}
		ptr += 2;
	}

	int fields = i;
	for (long il = rec; il <= R_count(); il++)
	{
		int sum = 0;
		ptr = (char **)&templ;
		int ret;
		if ((ret = Go(il)) == -1)
		{
			delete num;
			return -1;
		}
		if (ret == 1)
			return 0;
		if (*buffer == '*')
			continue;
		for (int j = 0; j < fields; j++, ptr += 2)
		{
			switch (F_type(num[j]))
			{
			case 'D':
			case 'C':
			case 'L':
				if (F_width(num[j]) == 1)
				{
					if (*F_ptr(num[j]) == **ptr)
					{
						sum++;
						break;
					}
					else
					{
						j = fields;
						break;
					}
				}
				else
				{
					int len = strlen(*ptr);
					if (!strncmpi(F_ptr(num[j]), *ptr, len))
					{
						if (len == F_width(num[j]))
						{
							sum++;
							break;
						}
						if (*(F_ptr(num[j]) + len) == ' ')
						{
							sum++;
							break;
						}
						else
						{
							j = fields;
							break;
						}
					}
					else
					{
						j = fields;
						break;
					}
				}
			case 'F':
			case 'N':
				if (((double *)(*ptr))[0] <= F_value(num[j]) &&
					((double *)(*ptr))[1] >= F_value(num[j]))
				{
					sum++;
					break;
				}
				else
				{
					j = fields;
					break;
				}
			default:
				break;
			}
		}
		if (sum == fields)
		{
			delete num;
			return rec_num;
		}
	}
	return 0L;
}

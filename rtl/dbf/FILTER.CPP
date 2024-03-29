#include <string.h>

#include "base.hpp"
#include "dbf.hpp"

//  int Filter_set(char*)    - ��������� ��ப�-䨫��� � ����饭�� ��
//                             � �⥪
//  �����頥��� ���祭��: 0 - ��ଠ�쭮� �����襭�� ��⠭���� 䨫���
//                        -1 - ���� ������ �� �뫠 �����
//                         1 - �訡�� �� �������樨
//                         2 - �訡�� �� ��⥯��樨

extern int parse(DBF *, char const *, char **, char *, char *);
extern void *exec(DBF *, char *, char *, char *);

int DBF::Filter_set(char const *_filter)
{
	char *compile, *ptr;
	char *eval_space, *eval_end;
	int len_comp;
	if (!handle)
		return -1; // �� ����� ���� ������
	if ((eval_space = new char[3001]) == (char *)0)
		return -1;
	eval_end = eval_space + 3000;
	if ((len_comp = parse(this, _filter, &compile, eval_space, eval_end)) == -1)
	{
		delete eval_space;
		return 1;
	} // �訡�� �� �������樨
	if ((exec(this, compile, eval_space, eval_end)) == (void *)0)
	{
		delete eval_space;
		return 2;
	} // �訡�� �� ������樨
	if (!fltr)
	{
		filter = compile;
		*((char **)compile) = (char *)0;
	}
	else
	{
		ptr = compile;
		*((char **)compile) = (char *)filter;
		filter = ptr;
	}
	this->fltr = (char)1;
	delete eval_space;
	return 0;
}

void DBF::Filter_reset(void)
{
	char *ptr;
	if (!handle || !filter)
		return;
	do
	{
		ptr = *((char **)filter);
		delete filter;
		filter = ptr;
	} while (filter);

	return;
}

void DBF::Filter_pop(void)
{
	char *ptr;
	if (!handle || !filter)
		return;
	ptr = *((char **)filter);
	delete filter;
	filter = ptr;
	return;
}

//-----------------------------------------------------------------------
//  First(long rec) - ���� ��ࢮ� �� 㤠������ �����, 㤮���⢮���饩
//		      ��⠭��������� 䨫����, ��稭�� � ����� rec
//  Next()          - �த������� ���᪠, � �ᯮ�짮������ ⮣� �� 蠡-
//                    ���� � ⥪�饩 �����+1
//
//  ������:     -1 - �訡�� ���᪠
//                    - �� ����� ����
//                    - �� ��⠭����� 䨫���
//                    - 㪠��� �� �������騩 ����� �����
//                0 - ����� 䠩��
//		 >0 - ����� �����, � ��⠭����� �� ��� 㪠��⥫� � ����
//
int Filter_do(DBF *file, char *eval_space, char *eval_end)
{
	char *compile, *n_compile;
	void *ptr;

	compile = n_compile = file->filter;
	do
	{
		if ((ptr = exec(file, compile, eval_space, eval_end)) == (void *)0)
			return -1;

		if (!*((int *)ptr))
			return 0;

		compile = *((char **)n_compile);
		n_compile = compile;
	} while (compile);

	return 1;
}

long DBF::Next(int sw)
{
	char *eval_space, *eval_end;
	if (!handle || !filter)
		return -1;

	if ((eval_space = new char[3001]) == (char *)0)
		return -1;
	eval_end = eval_space + 3000;
	long i;
	int ret = 0;
	for (i = sw ? rec_num : rec_num + 1; i <= head.numb_rec; i++)
	{
		if ((ret = Go(i)) == -1)
		{
			delete eval_space;
			return -1;
		}
		if (ret == 1)
			break;
		if (*buffer == '*')
			continue;
		if (!filter)
		{
			delete eval_space;
			return i;
		}
		switch (Filter_do(this, eval_space, eval_end))
		{
		case -1:
			delete eval_space;
			return -1;
		case 1:
			delete eval_space;
			return i;
		case 0:
			break;
		}
	}
	delete eval_space;
	return 0;
}

long DBF::First(long rec)
{
	int ret = 0;

	if (!handle)
		return -1;

	if ((ret = Go(rec)) == -1)
		return -1;

	if (ret == 1)
		return 0;

	return Next(1);
}

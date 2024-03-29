#ifndef _BASEHPP
#define _BASEHPP
/*  base.hpp   (c)Copyright Sequiter Software Inc., 1987, 1988, 1989.  All rights reserved.  */

// HEAD  - ���ᠭ�� ���������

struct HEAD
{
	unsigned char vers,
		yy,
		mm,
		dd;
	unsigned long numb_rec;
	unsigned header_len;
	unsigned rec_width;
	HEAD();
};

// FIELD  - ���ᠭ�� ����� dbf-䠩��

struct FIELD
{
	char name[11];
	char type;
	unsigned offset;
	char rez[2];
	unsigned char width,
		decimals;
	FIELD(char const *, char, int, int = 0);
};

// BASE - ���ᠭ�� ����

struct BASE
{
	char name[64]; // ���� ����㯠
	int handle;	   // 㪠��⥫� �� 䠩�

	/* Buffer Information */

	long rec_num;	// ����� ⥪�饩 �����
	int buf_len;	// ����� �����
	long rec_first; // ����� ��ࢮ� ����� � ����

	char *old_buf, *buffer; // ���� ����� (-��)

	int num_fields; // �᫮ ����� � ����
	FIELD *fields;	// 㪠��⥫� �� ����, �࠭�騩 ���ᠭ�� �����

	char *filter; // 㪠��⥫� �� �⥪, �࠭�騩 �ᥢ������. ��ப�
	HEAD head;	  // ���ᠭ�� ���������
};

#endif

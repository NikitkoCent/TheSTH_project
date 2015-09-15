#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <sys/stat.h>
#include "file_header.h"

typedef struct
{
	char string[FILENAMELENGTH_MAX];
	uint8_t strLen;
} FileName;

typedef FileName ListType;	//������������ ��� �������� ��� ������

typedef struct List
{
	ListType value;
	struct List *next;
} *List;


/*
List List_Create(const ListType startVal)
	������� ������ ������ � ��������������� ��������� ���������

	���������:
		const ListType startVal - ��������� ��������

	������������ �������� (List):
		������������������ ������
*/
List List_Create(const ListType startVal);


/*
void List_Free(List *list)
	������� ���������� ������ � ����������� ������, ���������� ��� ����

	���������:
		List *list - ����� ������������ ������

	������������ ��������:
		�����������
*/
void List_Free(List *list);


/*
int List_Insert(List list, const ListType val)
	������� ��������� � ����� ������ ����� �������

	���������:
		List list - ������, � ������� ��������� ���������� �������
		const ListType val - �������� ������ ��������

	������������ ��������:
		0	-	������� ����������� �������
		1	-	������ ������������ ������ (NULL ���������)
		2	-	�������� ������
*/
int List_Insert(List list, const ListType val);


/*
int List_IsEmpty(List list)
	������� �������� ������ �� �������

	���������:
		List list -	����������� ������

	������������ ��������:
		0	-	������ ������
		1	-	����� ����
*/
int List_IsEmpty(List list);

#endif
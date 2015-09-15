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

typedef FileName ListType;	//используемый тип значения для списка

typedef struct List
{
	ListType value;
	struct List *next;
} *List;


/*
List List_Create(const ListType startVal)
	Функция создаёт список с устанавливаемым начальным значением

	Параметры:
		const ListType startVal - начальное значение

	Возвращаемое значение (List):
		Инициализированный список
*/
List List_Create(const ListType startVal);


/*
void List_Free(List *list)
	Функция уничтожает список и освобождает память, выделенную под него

	Параметры:
		List *list - адрес уничтожаемог списка

	Возвращаемое значение:
		отсутствует
*/
void List_Free(List *list);


/*
int List_Insert(List list, const ListType val)
	Функция вставляет в конец списка новый элемент

	Параметры:
		List list - список, в который требуется произвести вставку
		const ListType val - значение нового элемента

	Возвращаемое значение:
		0	-	вставка произведена успешно
		1	-	указан некорректный список (NULL указатель)
		2	-	нехватка памяти
*/
int List_Insert(List list, const ListType val);


/*
int List_IsEmpty(List list)
	Функция проверят список на пустоту

	Параметры:
		List list -	проверяемый список

	Возвращаемое значение:
		0	-	список непуст
		1	-	спсок пуст
*/
int List_IsEmpty(List list);

#endif
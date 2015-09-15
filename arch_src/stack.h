/**
\file
\brief Функции для работы со стеком

\author Никита Провоторов
\date Март 2015
*/
#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include "errcodes.h"

#define STACK_BLOCKSIZE 256

typedef struct
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fName[_MAX_FNAME];
	char ext[_MAX_EXT];
} FileName;

typedef struct {
	FileName fullName;
	FileName shortName;
} Token;

typedef struct
{
	Token *arr;
	size_t len;
	size_t size;
} Array;

typedef Array Stack;

/**
	Функция создает стек

	\retval NULL Не удалось создать стек (нет памяти)
	\retval !NULL Созданный стек
*/
Stack * Stack_Create();

/**
	Функция уничтожает стек, созданный ранее функцией Stack_Create

	\param[in] stack Уничтожаемый стек
*/
void Stack_Destroy(Stack *stack);

/**
	Функция добавляет элемент на вершину стека

	\param[in,out] stack Стек
	\param[in] token Указатель на добавляемый элемент стека

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM

	\see errcodes.h Описание кодов ошибок
*/
err_type Stack_Push(Stack *stack, const Token *token);

/** 
	Функция снимает элемент с вершины стека

	\param[in] stack Стек
	\param[out] result Адрес, по которому будет записано значение с вершины stack

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h Описание кодов ошибок
*/
err_type Stack_Pop(Stack *stack, Token *result);


/**
	Функция получает элемент с вершины стека

	\param[in] stack Стек
	\param[out] result Адрес, по которому будет записано значение с вершины stack

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h Описание кодов ошибок
*/
err_type Stack_Peek(const Stack *stack, Token *result);


/**
	Функция проверяет стек на пустоту

	\param[in] stack Стек

	\retval 0 Стек не пуст
	\retval 1 Стек пуст
*/
char Stack_IsEmpty(const Stack* stack);

#endif
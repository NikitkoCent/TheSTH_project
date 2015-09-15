/**
\file
\brief ������� ��� ������ �� ������

\author ������ ����������
\date ���� 2015
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
	������� ������� ����

	\retval NULL �� ������� ������� ���� (��� ������)
	\retval !NULL ��������� ����
*/
Stack * Stack_Create();

/**
	������� ���������� ����, ��������� ����� �������� Stack_Create

	\param[in] stack ������������ ����
*/
void Stack_Destroy(Stack *stack);

/**
	������� ��������� ������� �� ������� �����

	\param[in,out] stack ����
	\param[in] token ��������� �� ����������� ������� �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������
*/
err_type Stack_Push(Stack *stack, const Token *token);

/** 
	������� ������� ������� � ������� �����

	\param[in] stack ����
	\param[out] result �����, �� �������� ����� �������� �������� � ������� stack

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h �������� ����� ������
*/
err_type Stack_Pop(Stack *stack, Token *result);


/**
	������� �������� ������� � ������� �����

	\param[in] stack ����
	\param[out] result �����, �� �������� ����� �������� �������� � ������� stack

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h �������� ����� ������
*/
err_type Stack_Peek(const Stack *stack, Token *result);


/**
	������� ��������� ���� �� �������

	\param[in] stack ����

	\retval 0 ���� �� ����
	\retval 1 ���� ����
*/
char Stack_IsEmpty(const Stack* stack);

#endif
/**
	\file
	\brief ������� ��� ������ � ������� ��������

	������ ������������ ���� �������� ��������� ������� ��� ������ � ������� �������� ��� ������������� ��������� ������ ��������

	\author ������ ����������
	\date ��� 2015
*/
#ifndef FGK_TREE
#define FGK_TREE

#include <stdint.h>
#include "errcodes.h"
#include "bit_array.h"

#define NOTBYTE (UINT16_MAX)
#define NULLNODEBYTE (UINT16_MAX - 1)
#define FILEENDBYTE (UINT16_MAX - 2)
typedef struct Node
{
	uint16_t byte;
	uint64_t weight;
	struct Node *next;
	struct Node *prev;
	struct Node *right;
	struct Node *left;
	struct Node *parent;
} Node;

typedef struct List
{
	Node *start;
	Node *end;				//"0-node"
	Node *byteNodes[256];
} *FGKTree;


/**
	\brief ������� ������ ������ ��������

	\retval NULL ������ �� ������� �������
	\retval !NULL ��������� ������
*/
FGKTree FGKTree_create();

/**
	\brief ������� ���������� ������ ��������

	\param[in] tree ������������ ������
*/
void FGKTree_destroy(FGKTree tree);

/**
	\brief ������� ���������� ������� ��� "0-node" �������

	\param[in] tree ������ ��������
	\param[out] result ������� ������, ���������� ��� "0-node" �������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h �������� ����� ������
*/
err_type FGKTree_getNullNodeSymbol(const FGKTree tree, BitArray result);

/**
	\brief ������� ���������� ������� ��� ����� �����

	\param[in] tree ������ ��������
	\param[out] result ������� ������, ���������� ��� ����� �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h �������� ����� ������
*/
err_type FGKTree_getFileEndSymbol(const FGKTree tree, BitArray result);

/**
	\brief ������� ���������� ������� ��� ���������� �����

	\param[in] tree ������ ��������
	\param[in] byte ����, ��� �������� ��������� ����� � ������
	\param[out] result ������� ������, ���������� ��� ���������� �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID
	\retval ERR_INDEX_OUT
	\retval ERR_BYTENOTFOUND

	\see errcodes.h �������� ����� ������
*/
err_type FGKTree_getByteCode(const FGKTree tree, const uint8_t byte, BitArray result);

/**
	\brief ������� ��������� ������ �������� ��������� ������

	\param[in,out] tree ������ ��������
	\param[in] byte �������� �����, ����������� ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_BYTENOTFOUND

	\see errcodes.h �������� ����� ������
*/
err_type FGKTree_update(FGKTree tree, const uint8_t byte);

/**
	\brief ������� ��������� � ������ �������� ���� � ��������� ��������� �����

	\param[in,out] tree ������ ��������
	\param[in] byte ����������� ����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID

	\see errcodes.h �������� ����� ������
*/
err_type FGKTree_addByte(FGKTree tree, const uint8_t byte);

#endif
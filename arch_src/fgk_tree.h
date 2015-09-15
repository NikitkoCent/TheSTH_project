/**
	\file
	\brief Функции для работы с деревом Хаффмана

	Данный заголовочный файл содержит прототипы функций для работы с деревом Хаффмана для динамического алгоритма сжатия Хафммана

	\author Никита Провоторов
	\date Май 2015
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
	\brief Функция создаёт дерево Хаффмана

	\retval NULL Дерево не удалось создать
	\retval !NULL Созданное дерево
*/
FGKTree FGKTree_create();

/**
	\brief Функция уничтожает дерево Хаффмана

	\param[in] tree Уничтожаемое дерево
*/
void FGKTree_destroy(FGKTree tree);

/**
	\brief Функция возвращает текущий код "0-node" символа

	\param[in] tree Дерево Хаффмана
	\param[out] result Битовый массив, содержащий код "0-node" символа

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h Описание кодов ошибок
*/
err_type FGKTree_getNullNodeSymbol(const FGKTree tree, BitArray result);

/**
	\brief Функция возвращает текущий код конца файла

	\param[in] tree Дерево Хаффмана
	\param[out] result Битовый массив, содержащий код конца файла

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h Описание кодов ошибок
*/
err_type FGKTree_getFileEndSymbol(const FGKTree tree, BitArray result);

/**
	\brief Функция возвращает текущий код указанного байта

	\param[in] tree Дерево Хаффмана
	\param[in] byte Байт, код которого требуется найти в дереве
	\param[out] result Битовый массив, содержащий код указанного байта

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID
	\retval ERR_INDEX_OUT
	\retval ERR_BYTENOTFOUND

	\see errcodes.h Описание кодов ошибок
*/
err_type FGKTree_getByteCode(const FGKTree tree, const uint8_t byte, BitArray result);

/**
	\brief Функция обновляет дерево Хаффмана указанным байтом

	\param[in,out] tree Дерево Хаффмана
	\param[in] byte Значение байта, обновляющее дерево

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_BYTENOTFOUND

	\see errcodes.h Описание кодов ошибок
*/
err_type FGKTree_update(FGKTree tree, const uint8_t byte);

/**
	\brief Функция добавляет в дерево Хаффмана узел с указанным значением байта

	\param[in,out] tree Дерево Хаффмана
	\param[in] byte Добавляемый байт

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_TREEINVALID

	\see errcodes.h Описание кодов ошибок
*/
err_type FGKTree_addByte(FGKTree tree, const uint8_t byte);

#endif
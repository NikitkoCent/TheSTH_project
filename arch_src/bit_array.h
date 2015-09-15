/**
	\file
	���������� �������� �������

	\author ������ ����������
	\date ������ 2015
*/
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include "errcodes.h"

typedef struct BitArray 
{
	uint8_t *arr;			
	uint64_t length;
	uint64_t byteLength;
	uint64_t size;
} *BitArray;


/**
	������� ������ ������� ������ ��������� �����

	\param[in] length ����� ������������ �������

	\retval NULL �� ������� ������� ������� ������ (������ �����, ��� ������)
	\retval ������ �����
*/
BitArray BitArray_create(const uint64_t length);

/**
	������� ���������� ��������� ����� ������� ������

	\param[in] bitArray ������������ ������� ������
*/
void BitArray_destroy(BitArray bitArray);

/**
	������� ���������, �������� �� ������� ������ ������������������

	\param[in] bitArray ���������� ������

	\retval ERR_PARAM_INVALID
	\retval ERR_NO

	\see errcodes.h �������� ����� ������
*/
_inline err_type BitArray_isInitialized(const BitArray bitArray);

/**
	������� ������������� ��������� ��� ������� � ��������� ��������

	\param[out] bitArray ������� ������
	\param[in] index ������ ������� ����
	\param[in] bit ��������, � ������� ������� ���������� ��������� ���

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h �������� ����� ������
*/
err_type BitArray_setBit(BitArray bitArray, const uint64_t index, const uint8_t bit);

/**
	������� ���������� �������� ���������� ���� �������
	\param[in] bitArray ������� ������
	\param[in] index ������ ������� ����
	\param[out] bit �����, �� �������� ����� �������� �������� ���������� ����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h �������� ����� ������
*/
err_type BitArray_getBit(const BitArray bitArray, const uint64_t index, uint8_t *bit);

/**
	������� ��������� � ������ ����� �������� �������

	\param[out] bitArray ������� ������
	\param[in] value ��������� �� ������, ����� �������� ����� ����������� � ������
	\param[in] valueSize ������ value � ������

	\warning ���� valueSize ������ ����� bitArray*8, �� �������� ����������� �� ����� ������������
*/
void BitArray_setValue(BitArray bitArray, const void *value, const uint64_t valueSize);

/**
	������� ���������� ����� �������� �������

	\param[in] bitArray ������� ������

	\retval 0 bitArray �� ���������������
	\retval >0 ����� �������

	\see errcodes.h �������� ����� ������
*/
uint64_t BitArray_getLength(const BitArray bitArray);

/**
	������� ��������� � ����� ������� ���

	\param[in,out] bitArray ������� ������
	\param[in] �������� ������������ ����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������
*/
err_type BitArray_pushBack(BitArray bitArray, const uint8_t bit);

/**
	������� ������� ��������� ��� �������� �������

	\param[in,out] bitArray ������� ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID

	\see errcodes.h �������� ����� ������
*/
err_type BitArray_popBack(BitArray bitArray);

/**
	������� �������� ������� ������

	\param[out] dst ������� ������, ���� ��������� �����������
	\param[in] src ������� ������, ������� ��������� �����������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID

	\see errcodes.h �������� ����� ������
*/
err_type BitArray_copy(BitArray dst, const BitArray src);

/**
	������� ���������� ������������ ������� ��������

	\param[out] dst ������� ������, � ����� �������� ����� ��������� ���������� src
	\param[in] src ������� ������, ������� ����� �������� � ����� dst

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������
*/
err_type BitArray_concatenate(BitArray dst, const BitArray src);

/**
	������� ������������� ����� �������� ������� � 0

	\param[in,out] bitArray ������� ������
*/
void BitArray_reset(BitArray bitArray);

#endif
/**
	\file
	������� ������ � big-endian ������� ������

	\authors ������ ����������
	\date ������� 2015
*/
#ifndef BIG_ENDIAN_H
#define BIG_ENDIAN_H

#include <stdint.h>
#include <stdlib.h>
#include "errcodes.h"


/**
	������� ��������� ����������� ����� ������ � big-endian ������

	\param[in] src �������������� ���� ������
	\param[in] size ������ src � ������
	\param[out] result ������ ����������

	\warning ���� �������� ������������ �������� ����������, ������ result �� ����������
*/
void convertToBigEndian(const void *src, const uint64_t size, void *result);

/**
	������� ��������� ����������� ����� ������ �� big-endian ������� � ������ ������������� ������� ������

	\param[in] src �������������� ���� ������
	\param[in] size ������ src � ������
	\param[out] result ������ ����������

	\warning ���� �������� ������������ �������� ����������, ������ result �� ����������
*/
void convertFromBigEndian(const void *src, const uint64_t size, void *result);

#endif
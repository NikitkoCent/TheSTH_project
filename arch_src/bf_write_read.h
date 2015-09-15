/**
	\file 
	\brief ��������� ������� ��������� �����-������ � ����

	������ ���� �������� ���������� ������� ��� ������\������ � �������� ���� � big-endian �������

	\authors ������ ����������
	\date ������� 2015
*/
#ifndef BF_WRITE_READ_H
#define BF_WRITE_READ_H

#include "errcodes.h"
#include <stdint.h>
#include <stdio.h>
#include "big_endian.h"

/**
	�������� ������ � ���� � big-endian �������

	\param[out] f ���������� ��������� �� �������� ������ �����
	\param[in] src ���� ������������ ������
	\param[in] srcSize ������ src � ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������
*/
err_type bfwrite(FILE *f, const void *src, const uint64_t srcSize);

/**
	������� ������ ������� bfwrite

	\param[out] f ���������� ��������� �� �������� ������ �����
	\param[in,out] src ���� ������������ ������
	\param[in] srcSize ������ src � ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������

	\warning ������ ������� ����� �������� ���������� src!
*/
err_type bfwrite_fast(FILE *f, void *src, const uint64_t srcSize);

/**
	�������� ������ �� ����� ����� ������ � big-endian �������

	\param[in] f ���������� ��������� �� �������� ������ �����
	\param[out] result ����� ��� ����������
	\param[in] byteCount ������ ������������ ����� � ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF

	\see errcodes.h �������� ����� ������
*/
err_type bfread(FILE *f, void *result, const uint64_t byteCount);

#endif
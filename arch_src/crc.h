/**
	\file
	\brief ��������� ������� ��� �������� CRC

	\authors ������ ����������
	\date ������� 2015
*/
#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include "errcodes.h"
#include <stdlib.h>

/**
	\brief ������� ����������� ����� CRC-16

	������ ������� ��������� ������� CRC-16 (ARC) ��������� �������.
	\n ������� : 0x8005
	\n ��������� �������� : 0xFFFF
	\n CRC("123456789") : 0x4B37

	\param[in] src ���� ������, �� �������� ��������� ���������� ������ CRC
	\param[in] len ����� src � ������
	\param[out] result ��������� ����������
	\param[in] oldcrc ���� ��������� ���������� ������ CRC �� ������ ����� ������������ ��������, ��������� ���� ����� ����� �������� ; ����� NULL

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\see errcodes.h �������� ����� ������
*/
err_type getCrc16(const void *src, uint16_t len, uint16_t *result, const uint16_t *oldcrc);

/**
	\brief ������� ����������� ����� CRC-32

	������ ������� ��������� ������� CRC-32 ��������� �������.

	\param[in] src ���� ������, �� �������� ��������� ���������� ������ CRC
	\param[in] len ����� src � ������
	\param[out] result ��������� ����������
	\param[in] oldcrc ���� ��������� ���������� ������ CRC �� ������ ����� ������������ ��������, ��������� ���� ����� ����� �������� ; ����� NULL

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\see errcodes.h �������� ����� ������
*/
err_type getCrc32(const void *src, uint32_t len, uint32_t *result, const uint32_t *oldcrc);


#endif
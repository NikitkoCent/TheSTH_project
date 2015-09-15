/**
	\file
	\brief ��������� ���������� ����� � ������ � �������

	������ ������������ ���� �������� �������� File_Header (���������� ����� � ������) � ��������� ���� ������� ��� ������ � ���

	\author ������ ����������
	\date ������� 2015
*/
#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <stdio.h>
#include <stdint.h>
#include "bf_write_read.h"
#include "crc.h"
#include "errcodes.h"

#define FILENAMELENGTH_MAX 512

typedef struct
{
	uint16_t headCrc;						///< CRC ���� ���������, �� ����������� fileCrc
	uint16_t fileNameLength;				///< ����� ����� �����
	uint8_t fileName[FILENAMELENGTH_MAX];	///< ��� �����
	uint16_t attr;							///< ��������� �����
	uint8_t method;							///< ����� ������
	uint64_t fileSize;						///< ������ �����
	uint32_t fileCrc;						///< CRC �����
} File_Header;

/**
	�������� ����������� ����� File_Header

	\param[in] fHead ����������� ���������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_HEAD_BADCRC

	\see errcodes.h �������� ����� ������
*/
err_type File_Header_checkCrc(const File_Header *fHead);

/**
	������� ����������� ����� File_Header

	\param[in,out] fHead �������������� ���������
*/
void File_Header_setCrc(File_Header *fHead);

/**
	\brief ������ ���������� � ����

	������ ������� ���������� ����������� ��������� � ������� ������� ��������� ����� 

	\param[out] f �������� �� �������� ������ ����, � ������� ��������� �������� ���������
	\param[in,out] fHead ����������� ���������, ������� ����� ��������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������
*/
err_type File_Header_fwrite(FILE *f, const File_Header *fHead);

/**
	\brief ������ ���������� �� �����

	������ ������� ��������� ��������� �� ������� ������� ��������� �� ������ �����

	\param[in] f �������� �� �������� ������ ����, � �������� ����� ������� ���������
	\param[out] fHead ����� ���������, � ������� ��������� ���������� ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_HEAD_BADCRC

	\see errcodes.h �������� ����� ������
*/
err_type File_Header_fread(FILE *f, File_Header *fHead);

/**
	\brief ��������� ������� � ������ ����������� ���������

	������ ������� ������������ ������ ����������� ��������� � ������

	\param[in] fHead ����������� ���������

	\retval 0 fHead == NULL
	\retval >0 �����
*/
uint16_t File_Header_getSize(const File_Header *fHead);

#endif
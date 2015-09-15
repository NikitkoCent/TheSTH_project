/**
\file
\brief ������������ ����, ���������� low-level API ������ � �������

\author ������ ����������
\date ���� 2015

\todo ������� ������������ ������ �� �����������
*/
#ifndef KERNEL_H
#define KERNEL_H

#include <stdio.h>
#include <io.h>
#include <sys/stat.h>
#include <string.h>
#include "big_endian.h"
#include "bit_array.h"
#include "crc.h"
#include "errcodes.h"
#include "bf_write_read.h"
#include "file_header.h"
#include "fgk_tree.h"

#define ARCH_VERSION 0x00090000			///<0:9:0:0
#define ARCH_MARKER  0x34FFCE84

#define BUFFER_SIZE 2*1024*1024			///<2 MB \warning ���� �������� ������ ��������, ����� ��������� ������ �� ����� ��������������!

/**
	\brief ���������� ����� � ����� ��� ������

	\param[out] arch ���������� ��������� �� ������ �����, ����������� �������
	\param[in] file ���������� ��������� �� ������ �����, ������� ������� �������� � �����
	\param[in,out] fHead ��������� �� ��� ����������� ��������� File_Header

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM
	\retval ERR_READ_FAILED
	\retval ERR_EOF

	\see errcodes.h �������� ����� ������
*/
err_type arch_addFile(FILE *arch, FILE *file, File_Header *fHead);

/**
	\brief ���������� ����� � ����� �� ������� (� ������� ������������� ��������� ��������)

	\param[out] arch ���������� ��������� �� ������ �����, ����������� �������
	\param[in] file ���������� ��������� �� ������ �����, ������� ������� �������� � �����
	\param[in,out] fHead ��������� �� ��� ����������� ��������� File_Header

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM
	\retval ERR_READ_FAILED
	\retval ERR_EOF

	\see errcodes.h �������� ����� ������
*/
err_type arch_addFileCompress(FILE *arch, FILE *file, File_Header *fHead);

/**
	\brief ������� ��������� ���������� �� ������� ����� �� ������

	\param[in] arch �������� �� ������ �����
	\param[out] outFile �������� �� ������ ���� 
	\param[in] bytesCount ������ � ������ ����� � ������

	\retval ERR_NO
	\retval ERR_NOMEM
	\retval ERR_WRITE_FAILED
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_FILE_BADCRC

	\see errcodes.h �������� ����� ������
*/
err_type arch_extractFile(FILE *arch, FILE *outFile, uint64_t bytesCount);

/**
	\brief ������� ��������� ���������� ������� ����� �� ������

	\param[in] arch �������� �� ������ �����
	\param[out] outFile �������� �� ������ ����
	\param[in] bytesCount ������ � ������ ����� � ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_WRITE_FAILED
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_FILE_BADCRC

	\see errcodes.h �������� ����� ������
*/
err_type arch_extractFileCompress(FILE *arch, FILE *outFile, uint64_t bytesCount);

/**
	\brief ������� ������� ���� �� ������

	\param[in,out] arch �������� �� ������+������ �����
	\param[in] fileName ��� ���������� �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_READ_FAILED
	\retval ERR_WRITE_FAILED

	\see errcodes.h �������� ����� ������
*/
err_type arch_deleteFile(FILE *arch, const uint8_t *fileName);

/**
	\brief ������� �������� ������ ������ �� ������

	������� ���������� � ������ result ������ ������ ; ����� �������� ���� ������ ������, ��� ������� ������� �������� �� ��� ���, ���� ��� �� ����� �������� ERR_EOF

	\param[in] arch �������� �� ������ �����
	\param[out] result ������, � ������� ����� ������� ������ ������
	\param[in] maxCount ���������, ���������� � �������� �������� ������ ������� �������� ; ����������, ��� �������� �� ������ ���� ������ ����� ������� result
	\param[out] resultCount ���������, ���������� � �������� ������ ���� �������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED

	\see errcodes.h �������� ����� ������
*/
err_type arch_getFileList(FILE *arch, File_Header *result, uint64_t maxCount, uint64_t *resultCount);

/**
	\brief ������� ������� � ������ ���� �� ���������� �����

	���� ���� ������, ������ ���������� � �� ����� ����������� � fHead_result

	\param[in] arch �������� �� ������ �����
	\param[in] fileName ��� �����
	\param[out] fHead_result �����, �� �������� ����� �������� ���������� � ��������� �����
	
	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_ARCH_FILENOTFOUND

	\see errcodes.h �������� ����� ������
*/
err_type arch_findFile(FILE *arch, char *fileName, File_Header *fHead_result);

/**
	\brief ������� ��������� ��������� ���� ������ � ������

	\param[in] arch �������� �� ������ �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_ARCH_BADMARKER
	\retval ERR_ARCH_OLDVERSION

	\see errcodes.h �������� ����� ������
*/
err_type arch_checkMarkerBlock(FILE *arch);

/**
	\brief ������� ���������� � ����� ��������� ���� ������

	\param[out] arch �������� �� ������ �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h �������� ����� ������
*/
err_type arch_writeMarkerBlock(FILE *arch);

/*
*/
//err_type arch_testFile(FILE *arch);

/* 
*/
err_type arch_test(FILE *arch);

#endif
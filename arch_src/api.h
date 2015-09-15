/**
\file
\brief ������������ ����, ���������� high-level API ������ � �������

\author ������ ����������
\date ��� 2015

\todo ������� ������������ ������ �� �����������
*/
#ifndef API_H
#define API_H

#include <sys/stat.h>
#include <Windows.h>
#include <stdio.h>
#include <errno.h>
#include <direct.h>
#include "errcodes.h"
#include "kernel.h"
#include "stack.h"

/**
	\brief ������� ��������� ���� � �����

	\param[in] archiveName ������ ��� ������
	\param[in] fileName ������ ��� �����
	\param[in] compression ������ ; ���� ���� ������� 0, �� ������ �� ����� �����������, ����� �����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_WRITE_FAILED
	\retval ERR_FILE_NOTEXISTS
	\retval ERR_NOMEM
	\retval ERR_UNKNOWN
	\retval ERR_NOREADACCESS
	\retval ERR_NOWRITEACCESS
	\retval ERR_NAME_TOOLONG
	\retval ERR_UNKNOWN_FILETYPE
	\retval ERR_ARCH_BADMARKER
	\retval ERR_ARCH_OLDVERSION
	\retval ERR_ARCH_FILENOTFOUND

	\see errcodes.h �������� ����� ������
*/
err_type archive_addFile(const char *archiveName, const char *fileName, const uint8_t compression);


//err_type archive_check

/**
	\brief ������� ��������� ���� �� ������

	\param[in] archiveName ������ ��� ������
	\param[in] fileName ��� ����� � ������
	\param[in] dirPathOut ������ ��� ��������, � ������� ��������� ������� ����

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_DIR_NOTEXISTS
	\retval ERR_NOREADACCESS
	\retval ERR_NOMEM
	\retval ERR_NOWRITEACCESS
	\retval ERR_NAME_TOOLONG
	\retval ERR_UNKNOWN
	\retval ERR_FILE_NOTEXISTS
	\retval ERR_READ_FAILED
	\retval ERR_WRITE_FAILED
	\retval	ERR_EOF
	\retval ERR_FILE_BADCRC
	\retval ERR_UNKNOWN_FILETYPE
	\retval ERR_ARCH_BADMARKER
	\retval ERR_ARCH_OLDVERSION
	\retval ERR_ARCH_FILENOTFOUND

	\see errcodes.h �������� ����� ������
*/
err_type archive_extractFile(const char *archiveName, const char *fileName, const char *dirPathOut);

/**
	\brief ������� ������� ���� �� ������

	\param[in] archiveName ������ ��� ������
	\param[in] fileName ��� ����� � ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOREADACCESS
	\retval ERR_NOMEM
	\retval ERR_NOWRITEACCESS
	\retval ERR_NAME_TOOLONG
	\retval ERR_UNKNOWN
	\retval ERR_FILE_NOTEXISTS
	\retval ERR_READ_FAILED
	\retval ERR_WRITE_FAILED
	\retval	ERR_EOF
	\retval ERR_UNKNOWN_FILETYPE
	\retval ERR_ARCH_OLDVERSION
	\retval ERR_ARCH_FILENOTFOUND
	\retval ERR_ARCH_BADMARKER

	\see errcodes.h �������� ����� ������
*/
err_type archive_deleteFile(const char *archiveName, const char *fileName);


/**
	\brief ������� �������� ������ ������ ������

	\param[in] archiveName ��� ������
	\param[in] callback ������� ��������� ������ ; ����� ������� ������ ��� ��� ���������� ������ ����� � ������ ; � ������� ����� �������� ���������� � ����� � data ��� ���������	
	\param[in] data �������������� �������� ; �������������� ������

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_ARCH_BADMARKER
	\retval ERR_ARCH_OLDVERSION
*/
err_type archive_getFileList(const char *archiveName, void(*callback)(File_Header *fHead, void *data), const void *data);

#endif
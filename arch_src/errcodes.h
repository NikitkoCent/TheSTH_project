/**
	\file
	\brief �������� ���� ����� ������

*/
#ifndef BIT_ARRAY_ERRCODES_H
#define BIT_ARRAY_ERRCODES_H

#include <stdint.h>

typedef uint8_t err_type;

/* General */
	#define ERR_NO 0				///< ������ ���
	#define ERR_PARAM_INVALID 1		///< ������������ �������� ���������� ����������
	#define ERR_NOMEM 2				///< ������������ ������ ��� ������

/* For bit_array.h */ 
	#define ERR_INDEX_OUT 4			///< ����� �� ������� �������� �������

/* For bf_write_read.h */
	#define ERR_EOF 5				///< ����������� ����� �����
	#define ERR_WRITE_FAILED 6		///< ������ ������
	#define ERR_READ_FAILED 7		///< ������ ������
	
/* For kernel.h */
	#define ERR_FILE_BADCRC 8		///< ������������ CRC ����� � ������ (���� ��������)
	#define ERR_HEAD_BADCRC 9		///< ������������ CRC ������������� ����� ����� � ������
	#define ERR_ARCH_BADMARKER 16	///< ������������ ������ ������ (�������� ���� �� �������� �������)
	#define ERR_ARCH_OLDVERSION 17	///< ������ ���������� ������� ����� ��� ��������� ������
	#define ERR_ARCH_FILENOTFOUND 18 ///< ��������� ���� � ������ �� ������

/* For api.h */
	#define ERR_FILE_NOTEXISTS 10	///< ��������� ���� �� ������
	#define ERR_DIR_NOTEXISTS 19	///< ���������� �� �������
	#define ERR_NAME_TOOLONG 11		///< ������� ������� ��� �����
	#define ERR_UNKNOWN 12			///< ����������� ������
	#define ERR_NOREADACCESS 13		///< ��� ���� �� ������
	#define ERR_NOWRITEACCESS 14	///< ��� ���� �� ������
	#define ERR_UNKNOWN_FILETYPE 15	///< ����������� ��� ���������� �����

/* For fgk_tree.h */
	#define ERR_BYTENOTFOUND 20		///< ��������� ���� � ������ �� ������
	#define ERR_TREEINVALID 21		///< ������������ ������
#endif
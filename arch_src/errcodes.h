/**
	\file
	\brief Описание всех кодов ошибок

*/
#ifndef BIT_ARRAY_ERRCODES_H
#define BIT_ARRAY_ERRCODES_H

#include <stdint.h>

typedef uint8_t err_type;

/* General */
	#define ERR_NO 0				///< ошибок нет
	#define ERR_PARAM_INVALID 1		///< некорректные значения переданных параметров
	#define ERR_NOMEM 2				///< недостаточно памяти для работы

/* For bit_array.h */ 
	#define ERR_INDEX_OUT 4			///< выход за границы битового массива

/* For bf_write_read.h */
	#define ERR_EOF 5				///< неожиданный конец файла
	#define ERR_WRITE_FAILED 6		///< ошибка записи
	#define ERR_READ_FAILED 7		///< ошибка чтения
	
/* For kernel.h */
	#define ERR_FILE_BADCRC 8		///< некорректный CRC файла в архиве (файл повреждён)
	#define ERR_HEAD_BADCRC 9		///< некорректный CRC заголовочного блока файла в архиве
	#define ERR_ARCH_BADMARKER 16	///< некорректный маркер архива (открытый файл не является архивом)
	#define ERR_ARCH_OLDVERSION 17	///< версия архиватора слишком стара для открытого архива
	#define ERR_ARCH_FILENOTFOUND 18 ///< указанный файл в архиве не найден

/* For api.h */
	#define ERR_FILE_NOTEXISTS 10	///< указанный файл не найден
	#define ERR_DIR_NOTEXISTS 19	///< директория не найдена
	#define ERR_NAME_TOOLONG 11		///< слишком длинное имя файла
	#define ERR_UNKNOWN 12			///< неожиданная ошибка
	#define ERR_NOREADACCESS 13		///< нет прав на чтение
	#define ERR_NOWRITEACCESS 14	///< нет прав на запись
	#define ERR_UNKNOWN_FILETYPE 15	///< неизвестный тип указанного файла

/* For fgk_tree.h */
	#define ERR_BYTENOTFOUND 20		///< указанный байт в дереве не найден
	#define ERR_TREEINVALID 21		///< некорректное дерево
#endif
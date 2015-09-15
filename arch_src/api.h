/**
\file
\brief Заголовочный файл, содержащий high-level API работы с архивом

\author Никита Провоторов
\date Май 2015

\todo Функция тестирование архива на целостность
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
	\brief Функция добавляет файл в архив

	\param[in] archiveName Полное имя архива
	\param[in] fileName Полное имя файла
	\param[in] compression Сжатие ; если сюда передан 0, то сжатие не будет выполняться, иначе будет

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

	\see errcodes.h Описание кодов ошибок
*/
err_type archive_addFile(const char *archiveName, const char *fileName, const uint8_t compression);


//err_type archive_check

/**
	\brief Функция извлекает файл из архива

	\param[in] archiveName Полное имя архива
	\param[in] fileName Имя файла в архиве
	\param[in] dirPathOut Полное имя каталога, в который требуется извлечь файл

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

	\see errcodes.h Описание кодов ошибок
*/
err_type archive_extractFile(const char *archiveName, const char *fileName, const char *dirPathOut);

/**
	\brief Функция удаляет файл из архива

	\param[in] archiveName Полное имя архива
	\param[in] fileName Имя файла в архиве

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

	\see errcodes.h Описание кодов ошибок
*/
err_type archive_deleteFile(const char *archiveName, const char *fileName);


/**
	\brief Функция получает список файлов архива

	\param[in] archiveName Имя архива
	\param[in] callback Функция обратного вызова ; будет вызвана каждый раз при нахождении нового файла в архиве ; в функцию будет переданы информация о файле и data без изменения	
	\param[in] data Необязательный параметр ; дополнительный данные

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_ARCH_BADMARKER
	\retval ERR_ARCH_OLDVERSION
*/
err_type archive_getFileList(const char *archiveName, void(*callback)(File_Header *fHead, void *data), const void *data);

#endif
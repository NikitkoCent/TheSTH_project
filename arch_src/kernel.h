/**
\file
\brief Заголовочный файл, содержащий low-level API работы с архивом

\author Никита Провоторов
\date Март 2015

\todo Функция тестирование архива на целостность
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

#define BUFFER_SIZE 2*1024*1024			///<2 MB \warning если изменить данное значение, ранее созданные архивы не будут распознаваться!

/**
	\brief Добавление файла в архив без сжатия

	\param[out] arch Дескриптор открытого на запись файла, являющегося архивом
	\param[in] file Дескриптор открытого на чтение файла, который следует добавить в архив
	\param[in,out] fHead Указатель на уже заполненную структуру File_Header

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM
	\retval ERR_READ_FAILED
	\retval ERR_EOF

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_addFile(FILE *arch, FILE *file, File_Header *fHead);

/**
	\brief Добавление файла в архив со сжатием (с помощью динамического алгоритма Хаффмана)

	\param[out] arch Дескриптор открытого на запись файла, являющегося архивом
	\param[in] file Дескриптор открытого на чтение файла, который следует добавить в архив
	\param[in,out] fHead Указатель на уже заполненную структуру File_Header

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM
	\retval ERR_READ_FAILED
	\retval ERR_EOF

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_addFileCompress(FILE *arch, FILE *file, File_Header *fHead);

/**
	\brief Функция выполняет извлечение не сжатого файла из архива

	\param[in] arch Открытый на чтение архив
	\param[out] outFile Открытый на запись файл 
	\param[in] bytesCount Размер в байтах файла в архиве

	\retval ERR_NO
	\retval ERR_NOMEM
	\retval ERR_WRITE_FAILED
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_FILE_BADCRC

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_extractFile(FILE *arch, FILE *outFile, uint64_t bytesCount);

/**
	\brief Функция выполняет извлечение сжатого файла из архива

	\param[in] arch Открытый на чтение архив
	\param[out] outFile Открытый на запись файл
	\param[in] bytesCount Размер в байтах файла в архиве

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_WRITE_FAILED
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_FILE_BADCRC

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_extractFileCompress(FILE *arch, FILE *outFile, uint64_t bytesCount);

/**
	\brief Функция удаляет файл из архива

	\param[in,out] arch Открытый на чтение+запись архив
	\param[in] fileName Имя удаляемого файла

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM
	\retval ERR_READ_FAILED
	\retval ERR_WRITE_FAILED

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_deleteFile(FILE *arch, const uint8_t *fileName);

/**
	\brief Функция получает список файлов из архива

	Функция записывает в массив result список файлов ; чтобы получить весь список файлов, эту функцию следует вызывать до тех пор, пока она не вернёт значение ERR_EOF

	\param[in] arch Открытый на чтение архив
	\param[out] result Массив, в который будет записан список файлов
	\param[in] maxCount Указывает, информацию о скольких максимум файлах следует узнавать ; фактически, это значение не должно быть больше длины массива result
	\param[out] resultCount Указывает, информация о скольких файлах была считана

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_getFileList(FILE *arch, File_Header *result, uint64_t maxCount, uint64_t *resultCount);

/**
	\brief Функция находит в архиве файл по указанному имени

	Если файл найден, полная информация о нём будет содержаться в fHead_result

	\param[in] arch Открытый на чтение архив
	\param[in] fileName Имя файла
	\param[out] fHead_result Адрес, по которому будет записана информация о найденном файле
	
	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_ARCH_FILENOTFOUND

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_findFile(FILE *arch, char *fileName, File_Header *fHead_result);

/**
	\brief Функция проверяет первичный блок данных в архиве

	\param[in] arch Открытый на чтение архив

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_ARCH_BADMARKER
	\retval ERR_ARCH_OLDVERSION

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_checkMarkerBlock(FILE *arch);

/**
	\brief Функция записывает в архив первичный блок данных

	\param[out] arch Открытый на запись архив

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h Описание кодов ошибок
*/
err_type arch_writeMarkerBlock(FILE *arch);

/*
*/
//err_type arch_testFile(FILE *arch);

/* 
*/
err_type arch_test(FILE *arch);

#endif
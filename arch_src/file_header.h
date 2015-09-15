/**
	\file
	\brief Структура метаданных файла в архиве и функции

	Данный заголовочный файл содержит описание File_Header (метаданные файла в архиве) и прототипы всех функций для работы с ним

	\author Никита Провоторов
	\date Февраль 2015
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
	uint16_t headCrc;						///< CRC всей структуры, за исключением fileCrc
	uint16_t fileNameLength;				///< Длина имени файла
	uint8_t fileName[FILENAMELENGTH_MAX];	///< Имя файла
	uint16_t attr;							///< Аттрибуты файла
	uint8_t method;							///< Метод сжатия
	uint64_t fileSize;						///< Размер файла
	uint32_t fileCrc;						///< CRC файла
} File_Header;

/**
	Проверка контрольной суммы File_Header

	\param[in] fHead Проверяемая структура

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_HEAD_BADCRC

	\see errcodes.h Описание кодов ошибок
*/
err_type File_Header_checkCrc(const File_Header *fHead);

/**
	Подсчёт контрольной суммы File_Header

	\param[in,out] fHead подсчитываемая структура
*/
void File_Header_setCrc(File_Header *fHead);

/**
	\brief Запись метаданных в файл

	Данная функция записывает заполненную структуру в текущую позицию открытого файла 

	\param[out] f Открытый на двоичную запись файл, в который требуется записать структуру
	\param[in,out] fHead Заполненная структура, которая будет записана

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h Описание кодов ошибок
*/
err_type File_Header_fwrite(FILE *f, const File_Header *fHead);

/**
	\brief Чтение метаданных из файла

	Данная функция считывает структуру из текущей позиции открытого на чтение файла

	\param[in] f Открытый на двоичное чтение файл, с которого будет считана структура
	\param[out] fHead Адрес структуры, в которую требуется произвести чтение

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF
	\retval ERR_HEAD_BADCRC

	\see errcodes.h Описание кодов ошибок
*/
err_type File_Header_fread(FILE *f, File_Header *fHead);

/**
	\brief Получение размера в байтах заполненной структуры

	Данная функция подсчитывает размер заполненной структуры в байтах

	\param[in] fHead Заполненная структура

	\retval 0 fHead == NULL
	\retval >0 Иначе
*/
uint16_t File_Header_getSize(const File_Header *fHead);

#endif
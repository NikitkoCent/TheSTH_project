/**
	\file 
	\brief Прототипы функций двоичного ввода-вывода в файл

	Данный файл содержит объявление функций для чтения\записи в двоичный файл в big-endian формате

	\authors Никита Провоторов
	\date Февраль 2015
*/
#ifndef BF_WRITE_READ_H
#define BF_WRITE_READ_H

#include "errcodes.h"
#include <stdint.h>
#include <stdio.h>
#include "big_endian.h"

/**
	Двоичная запись в файл в big-endian формате

	\param[out] f Дескриптор открытого на двоичную запись файла
	\param[in] src Блок записываемых данных
	\param[in] srcSize Размер src в байтах

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h описание кодов ошибок
*/
err_type bfwrite(FILE *f, const void *src, const uint64_t srcSize);

/**
	Быстрая версия функции bfwrite

	\param[out] f Дескриптор открытого на двоичную запись файла
	\param[in,out] src Блок записываемых данных
	\param[in] srcSize Размер src в байтах

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_WRITE_FAILED
	\retval ERR_NOMEM

	\see errcodes.h описание кодов ошибок

	\warning Данная функция может изменяет содержимое src!
*/
err_type bfwrite_fast(FILE *f, void *src, const uint64_t srcSize);

/**
	Двоичное чтение из файла блока данных в big-endian формате

	\param[in] f Дескриптор открытого на двоичное чтение файла
	\param[out] result Место для считывания
	\param[in] byteCount Размер считываемого блока в байтах

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_READ_FAILED
	\retval ERR_EOF

	\see errcodes.h описание кодов ошибок
*/
err_type bfread(FILE *f, void *result, const uint64_t byteCount);

#endif
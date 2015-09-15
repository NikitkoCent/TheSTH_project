/**
	\file
	Функции работы с big-endian блоками данных

	\authors Никита Провоторов
	\date Февраль 2015
*/
#ifndef BIG_ENDIAN_H
#define BIG_ENDIAN_H

#include <stdint.h>
#include <stdlib.h>
#include "errcodes.h"


/**
	Функция выполняет конвертацию блока данных в big-endian формат

	\param[in] src Конвертируемый блок данных
	\param[in] size Размер src в байтах
	\param[out] result Буффер результата

	\warning Если переданы некорректные значения параметров, буффер result не изменяется
*/
void convertToBigEndian(const void *src, const uint64_t size, void *result);

/**
	Функция выполняет конвертацию блока данных из big-endian формата в формат представления текушей машины

	\param[in] src Конвертируемый блок данных
	\param[in] size Размер src в байтах
	\param[out] result Буффер результата

	\warning Если переданы некорректные значения параметров, буффер result не изменяется
*/
void convertFromBigEndian(const void *src, const uint64_t size, void *result);

#endif
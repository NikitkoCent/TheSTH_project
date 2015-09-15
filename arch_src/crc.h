/**
	\file
	\brief Прототипы функций для подсчёта CRC

	\authors Никита Провоторов
	\date Февраль 2015
*/
#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include "errcodes.h"
#include <stdlib.h>

/**
	\brief Подсчёт контрольной суммы CRC-16

	Данная функция выполняет подсчёт CRC-16 (ARC) табличным методом.
	\n Полином : 0x8005
	\n Начальное значение : 0xFFFF
	\n CRC("123456789") : 0x4B37

	\param[in] src Блок данных, по которому требуется произвести расчёт CRC
	\param[in] len Длина src в байтах
	\param[out] result Результат вычисления
	\param[in] oldcrc Если требуется произвести расчёт CRC на основе ранее вычисленного значения, передайте сюда адрес этого значения ; иначе NULL

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\see errcodes.h Описание кодов ошибок
*/
err_type getCrc16(const void *src, uint16_t len, uint16_t *result, const uint16_t *oldcrc);

/**
	\brief Подсчёт контрольной суммы CRC-32

	Данная функция выполняет подсчёт CRC-32 табличным методом.

	\param[in] src Блок данных, по которому требуется произвести расчёт CRC
	\param[in] len Длина src в байтах
	\param[out] result Результат вычисления
	\param[in] oldcrc Если требуется произвести расчёт CRC на основе ранее вычисленного значения, передайте сюда адрес этого значения ; иначе NULL

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\see errcodes.h Описание кодов ошибок
*/
err_type getCrc32(const void *src, uint32_t len, uint32_t *result, const uint32_t *oldcrc);


#endif
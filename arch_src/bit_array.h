/**
	\file
	Реализация битового массива

	\author Никита Провоторов
	\date Апрель 2015
*/
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include "errcodes.h"

typedef struct BitArray 
{
	uint8_t *arr;			
	uint64_t length;
	uint64_t byteLength;
	uint64_t size;
} *BitArray;


/**
	Функция создаёт битовый массив указанной длины

	\param[in] length Длина создаваемого массива

	\retval NULL не удалось создать битовый массив (скорее всего, нет памяти)
	\retval массив иначе
*/
BitArray BitArray_create(const uint64_t length);

/**
	Функция уничтожает созданный ранее битовый массив

	\param[in] bitArray Уничтожаемый битовый массив
*/
void BitArray_destroy(BitArray bitArray);

/**
	Функция проверяет, является ли битовый массив инициализированным

	\param[in] bitArray проверямый массив

	\retval ERR_PARAM_INVALID
	\retval ERR_NO

	\see errcodes.h Описание кодов ошибок
*/
_inline err_type BitArray_isInitialized(const BitArray bitArray);

/**
	Функция устанавливает указанный бит массива в указанное значение

	\param[out] bitArray Битовый массив
	\param[in] index Индекс нужного бита
	\param[in] bit Значение, в которое следует установить указанный бит

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h Описание кодов ошибок
*/
err_type BitArray_setBit(BitArray bitArray, const uint64_t index, const uint8_t bit);

/**
	Функция возвращает значение указанного бита массива
	\param[in] bitArray Битовый массив
	\param[in] index Индекс нужного бита
	\param[out] bit Адрес, по которому будет записано значение указанного бита

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT

	\see errcodes.h Описание кодов ошибок
*/
err_type BitArray_getBit(const BitArray bitArray, const uint64_t index, uint8_t *bit);

/**
	Функция загружает в массив байты внешнего буффера

	\param[out] bitArray Битовый массив
	\param[in] value Указатель на буффер, байты которого будут скопированы в массив
	\param[in] valueSize размер value в байтах

	\warning Если valueSize больше длины bitArray*8, то операция копирования не будет осуществлена
*/
void BitArray_setValue(BitArray bitArray, const void *value, const uint64_t valueSize);

/**
	Функция возвращает длину битового массива

	\param[in] bitArray Битовый массив

	\retval 0 bitArray не инициализирован
	\retval >0 Длина массива

	\see errcodes.h Описание кодов ошибок
*/
uint64_t BitArray_getLength(const BitArray bitArray);

/**
	Функция добавляет в конец массива бит

	\param[in,out] bitArray Битовый массив
	\param[in] Значение добавляемого бита

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_NOMEM

	\see errcodes.h Описание кодов ошибок
*/
err_type BitArray_pushBack(BitArray bitArray, const uint8_t bit);

/**
	Функция удаляет последний бит битового массива

	\param[in,out] bitArray Битовый массив

	\retval ERR_NO
	\retval ERR_PARAM_INVALID

	\see errcodes.h Описание кодов ошибок
*/
err_type BitArray_popBack(BitArray bitArray);

/**
	Функция копирует битовый массив

	\param[out] dst Битовый массив, куда требуется скопировать
	\param[in] src Битовый массив, который требуется скопировать

	\retval ERR_NO
	\retval ERR_PARAM_INVALID

	\see errcodes.h Описание кодов ошибок
*/
err_type BitArray_copy(BitArray dst, const BitArray src);

/**
	Функция производит конкатенацию битовых массивов

	\param[out] dst Битовый массив, в конец которого будет добавлено содержимое src
	\param[in] src Битовый массив, который будет добавлен в конец dst

	\retval ERR_NO
	\retval ERR_PARAM_INVALID
	\retval ERR_INDEX_OUT
	\retval ERR_NOMEM

	\see errcodes.h Описание кодов ошибок
*/
err_type BitArray_concatenate(BitArray dst, const BitArray src);

/**
	Функция устанавливает длину битового массива в 0

	\param[in,out] bitArray Битовый массив
*/
void BitArray_reset(BitArray bitArray);

#endif
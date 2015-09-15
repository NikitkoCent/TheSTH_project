#include "bit_array.h"

BitArray BitArray_create(const uint64_t length)
{
	BitArray result = NULL;

	if ((result = (BitArray)malloc(sizeof(struct BitArray))) != NULL)
	{
		result->arr = NULL;
		
		result->size = length / 8 + (length % 8 > 0);
		result->size = result->size == 0 ? 1 : result->size;
		
		result->byteLength = 0;
		result->length = 0;

		if ((result->arr = (uint8_t *)calloc(result->size, sizeof(uint8_t))) == NULL)
		{
			free(result);
			result = NULL;
		}
		else
		{
			memset(result->arr, 0, result->size * sizeof(uint8_t));
		}
	}

	return result;
}


void BitArray_destroy(BitArray bitArray)
{
	if (bitArray != NULL)
	{
		if (bitArray->arr != NULL)
		{
			free(bitArray->arr);
			bitArray->arr = NULL;
		}

		bitArray->length = 0;
		bitArray->byteLength = 0;
		bitArray->size = 0;

		free(bitArray);
	}
}


_inline err_type BitArray_isInitialized(const BitArray bitArray)
{
	return ((bitArray == NULL) || (bitArray->arr == NULL) || (bitArray->size == 0)) ? ERR_PARAM_INVALID : ERR_NO;
}


err_type BitArray_setBit(BitArray bitArray, const uint64_t index, const uint8_t bit)
{
	uint8_t mask, shiftCount;
	uint64_t byteIndex;
	
	if (BitArray_isInitialized(bitArray) == ERR_PARAM_INVALID)
		return ERR_PARAM_INVALID;

	if (index >= bitArray->length)
		return ERR_INDEX_OUT;

	byteIndex = index / 8;
	shiftCount = (byteIndex + 1) * 8 - 1 - index;                    

	mask = 1;
	mask = mask << shiftCount;

	if (bit == 0)
		bitArray->arr[byteIndex] &= (~mask);
	else
		bitArray->arr[byteIndex] |= mask;

	return ERR_NO;
}


err_type BitArray_getBit(const BitArray bitArray, const uint64_t index, uint8_t *bit)
{
	uint8_t mask, shiftCount;
	uint64_t byteIndex;
	
	if (BitArray_isInitialized(bitArray) == ERR_PARAM_INVALID)
		return ERR_PARAM_INVALID;

	if (index >= bitArray->length)
		return ERR_INDEX_OUT;

	if (bit == NULL)
		return ERR_PARAM_INVALID;

	byteIndex = index / 8;
	shiftCount = (byteIndex + 1) * 8 - 1 - index;

	mask = 1;
	mask = mask << shiftCount;

	*bit = ((bitArray->arr[byteIndex] | mask) == bitArray->arr[byteIndex]);

	return ERR_NO;
}


uint64_t BitArray_getLength(const BitArray bitArray)
{
	if (BitArray_isInitialized(bitArray) == ERR_PARAM_INVALID)
		return 0;

	return bitArray->length;
}


err_type BitArray_pushBack(BitArray bitArray, const uint8_t bit)
{
	uint8_t *temp = NULL;
	
	if (BitArray_isInitialized(bitArray) == ERR_PARAM_INVALID)
		return ERR_PARAM_INVALID;

	if (bitArray->length == bitArray->size * 8)
	{
		temp = bitArray->arr;
		if ((bitArray->arr = (uint8_t *)realloc(bitArray->arr, (bitArray->size + 1)*sizeof(uint8_t))) == NULL)
		{
			bitArray->arr = temp;
			return ERR_NOMEM;
		}

		bitArray->size++;
	}
	bitArray->length++;
	bitArray->byteLength = (bitArray->length / 8) + (bitArray->length % 8 != 0);
	
	return BitArray_setBit(bitArray, bitArray->length - 1, bit);
}


err_type BitArray_popBack(BitArray bitArray)
{
	if (BitArray_isInitialized(bitArray) == ERR_PARAM_INVALID)
		return ERR_PARAM_INVALID;

	if (bitArray->length > 0)
	{
		bitArray->length--;
	}
	bitArray->byteLength = (bitArray->length / 8) + (bitArray->length % 8 != 0);

	return ERR_NO;
}


void BitArray_setValue(BitArray bitArray, const void *value, const uint64_t valueSize)
{
	if ((BitArray_isInitialized(bitArray) == ERR_NO) && (value != NULL) && (bitArray->size >= valueSize))
	{
		memcpy(bitArray->arr, value, valueSize);
		bitArray->length = valueSize * 8;
		bitArray->byteLength = valueSize;
	}
}


err_type BitArray_copy(BitArray dst, const BitArray src)
{
	if ((BitArray_isInitialized(dst) != ERR_NO) || ((BitArray_isInitialized(src)) != ERR_NO))
		return ERR_PARAM_INVALID;

	dst->byteLength = min(dst->size, src->byteLength);
	memcpy(dst->arr, src->arr, dst->byteLength * sizeof(uint8_t));
	dst->length = min(dst->byteLength * 8, src->length);

	return ERR_NO;
}


err_type BitArray_concatenate(BitArray dst, const BitArray src)
{
	uint64_t i;
	uint8_t bit;
	err_type result;

	if ((BitArray_isInitialized(dst) != ERR_NO) || ((BitArray_isInitialized(src)) != ERR_NO))
		return ERR_PARAM_INVALID;

	for (i = 0; i < src->length; i++)
	{
		if ((result = BitArray_getBit(src, i, &bit)) != ERR_NO)
			return result;
		if ((result = BitArray_pushBack(dst, bit)) != ERR_NO)
			return result;
	}

	return ERR_NO;
}


void BitArray_reset(BitArray bitArray)
{
	if (BitArray_isInitialized(bitArray) == ERR_NO)
	{
		bitArray->length = 0;
		bitArray->byteLength = 0;
	}
}
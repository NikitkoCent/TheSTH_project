#include "big_endian.h"


static uint8_t isBigEndian()
{
	const uint16_t temp = 1;

	return (*((uint8_t *)&temp) == 0);
}


void convertToBigEndian(const void *src, const uint64_t size, void *result)
{
	uint8_t *ptr, *ptr2;
	uint64_t i;
	uint8_t temp;

	if ((src == NULL) || (size == 0) || (result == NULL))
		return;

	ptr = (uint8_t *)src;
	ptr2 = (uint8_t *)result;
	if (!isBigEndian())
	{
		if (src != result)
			for (i = 0; i < size; i++)
				ptr2[i] = ptr[size - 1 - i];
		else
			for (i = 0; i < size / 2; i++)
			{
				temp = ptr2[i];
				ptr2[i] = ptr[size - 1 - i];
				ptr[size - 1 - i] = temp;
			}
	}
	else
	{
		if (src != result)
			for (i = 0; i < size; i++)
				ptr2[i] = ptr[i];
	}
}


void convertFromBigEndian(const void *src, const uint64_t size, void *result)
{
	convertToBigEndian(src, size, result);
}
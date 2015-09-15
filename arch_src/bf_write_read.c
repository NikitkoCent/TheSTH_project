#include "bf_write_read.h"


err_type bfwrite(FILE *f, const void *src, const uint64_t srcSize)
{
	uint8_t buf_fast[1 * 1024];
	uint8_t *buf = NULL;
	uint8_t result;

	if ((f == NULL) || (src == NULL) || (srcSize == 0))
		return ERR_PARAM_INVALID;

	if (srcSize <= 1024)
	{
		convertToBigEndian(src, srcSize, buf_fast);
		result = (fwrite(buf_fast, srcSize, 1, f) == 1) ? ERR_NO : ERR_WRITE_FAILED;
	}
	else
	{
		if ((buf = (uint8_t *)malloc(srcSize)) == NULL)
			return ERR_NOMEM;

		convertToBigEndian(src, srcSize, buf);
		result = (fwrite(buf, srcSize, 1, f) == 1) ? ERR_NO : ERR_WRITE_FAILED;
		free(buf);
	}

	return result;
}


err_type bfwrite_fast(FILE *f, void *src, const uint64_t srcSize)			//src maybe changed!
{
	if ((f == NULL) || (src == NULL) || (srcSize == 0))
		return ERR_PARAM_INVALID;

	convertToBigEndian(src, srcSize, src);

	return (fwrite(src, srcSize, 1, f) == 1) ? ERR_NO : ERR_WRITE_FAILED;
}


err_type bfread(FILE *f, void *result, const uint64_t byteCount)
{
	if ((f == NULL) || (result == NULL) || (byteCount == 0))
		return ERR_PARAM_INVALID;

	if (fread(result, byteCount, 1, f) != 1)
		return (feof(f) == 0) ? ERR_READ_FAILED : ERR_EOF;

	convertFromBigEndian(result, byteCount, result);

	return ERR_NO;
}
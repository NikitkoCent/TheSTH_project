#include "kernel.h"

typedef enum SymbolState { NEW, EXISTS } SymbolState;

err_type arch_addFile(FILE *arch, FILE *file, File_Header *fHead)
{
	uint8_t *buffer = NULL;
	uint32_t buf_size = BUFFER_SIZE;
	err_type result = ERR_NO;
	uint64_t bytesCount = 0;
	uint64_t temp;
	void *temp2;

	if (file == NULL)
		return ERR_PARAM_INVALID;
	
	if ((result = File_Header_fwrite(arch, fHead)) != ERR_NO)
		return result;

	bytesCount = fHead->fileSize;

	if (bytesCount == 0)
		return ERR_NO;

	if ((buffer = (uint8_t *)calloc(buf_size, sizeof(uint8_t))) == NULL)
		return ERR_NOMEM;

	temp2 = NULL;
	while (bytesCount != 0)
	{
		temp = (bytesCount < buf_size) ? bytesCount : buf_size;
		
		if (fread(buffer, temp, 1, file) == 0)
		{
			free(buffer);
			return (feof(file) == 0) ? ERR_READ_FAILED : ERR_EOF;
		}

		getCrc32(buffer, temp, &(fHead->fileCrc), temp2);
		temp2 = &(fHead->fileCrc);
		
		if ((result = bfwrite_fast(arch, buffer, temp)) != ERR_NO)
		{
			free(buffer);
			return result;
		}

		bytesCount -= temp;
	}

	free(buffer);

	result = bfwrite(arch, &(fHead->fileCrc), sizeof(fHead->fileCrc));
	return result;
}


err_type arch_addFileCompress(FILE *arch, FILE *file, File_Header *fHead)
{
	err_type result = ERR_NO;
	uint8_t *inBuffer = NULL;
	BitArray outBuffer = NULL;
	BitArray inSymbol = NULL, outSymbol = NULL;
	FGKTree tree = NULL;
	uint64_t fileSize, readSize, i, j;
	int64_t filePos;
	uint32_t *crc = NULL;
	uint8_t bit;

	if (file == NULL)
		return ERR_PARAM_INVALID;

	if ((inBuffer = (uint8_t *)malloc(BUFFER_SIZE * sizeof(uint8_t))) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((outBuffer = BitArray_create(BUFFER_SIZE * 8)) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((inSymbol = BitArray_create(40 * 8)) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((outSymbol = BitArray_create(40 * 8)) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((tree = FGKTree_create()) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}

	if ((filePos = _ftelli64(arch)) == -1)
	{
		result = ERR_WRITE_FAILED;
		goto finalization;
	}
	if ((result = File_Header_fwrite(arch, fHead)) != ERR_NO)
	{
		goto finalization;
	}

	fileSize = fHead->fileSize;
	fHead->fileSize = 0;

	if (fileSize > 0)
	{
		while (fileSize > 0)
		{
			readSize = min(BUFFER_SIZE, fileSize);

			if ((result = fread(inBuffer, readSize, 1, file)) == 0)
			{
				result = (feof(file) == 0) ? ERR_READ_FAILED : ERR_EOF;
				goto finalization;
			}

			getCrc32(inBuffer, readSize, &(fHead->fileCrc), crc);
			crc = &(fHead->fileCrc);

			for (i = 0; i < readSize; i++)
			{
				BitArray_reset(inSymbol);
				BitArray_reset(outSymbol);

				if ((result = FGKTree_getByteCode(tree, inBuffer[i], outSymbol)) == ERR_BYTENOTFOUND)
				{
					if ((result = FGKTree_getNullNodeSymbol(tree, outSymbol)) != ERR_NO)
						goto finalization;
					BitArray_setValue(inSymbol, inBuffer + i, sizeof(inBuffer[i]));
					if ((result = BitArray_concatenate(outSymbol, inSymbol)) != ERR_NO)
						goto finalization;

					if ((result = FGKTree_addByte(tree, inBuffer[i])) != ERR_NO)
						goto finalization;
				}
				else if (result != ERR_NO)
					goto finalization;

				if (BitArray_getLength(outSymbol) + BitArray_getLength(outBuffer) > outBuffer->size * 8)
				{
					for (j = 0; BitArray_getLength(outBuffer) < outBuffer->size * 8; j++)
					{
						if ((result = BitArray_getBit(outSymbol, j, &bit)) != ERR_NO)
							goto finalization;
						if ((result = BitArray_pushBack(outBuffer, bit)) != ERR_NO)
							goto finalization;
					}

					if ((result = bfwrite_fast(arch, outBuffer->arr, outBuffer->byteLength)) != ERR_NO)
						goto finalization;
					fHead->fileSize += outBuffer->byteLength;
					BitArray_reset(outBuffer);

					for (; j < BitArray_getLength(outSymbol); j++)
					{
						if ((result = BitArray_getBit(outSymbol, j, &bit)) != ERR_NO)
							goto finalization;
						if ((result = BitArray_pushBack(outBuffer, bit)) != ERR_NO)
							goto finalization;
					}
				}
				else
				{
					if ((result = BitArray_concatenate(outBuffer, outSymbol)) != ERR_NO)
						goto finalization;
				}

				if ((result = FGKTree_update(tree, inBuffer[i])) != ERR_NO)
					goto finalization;
			}

			fileSize -= readSize;

			if (fileSize == 0)
			{
				if ((result = FGKTree_getFileEndSymbol(tree, outSymbol)) != ERR_NO)
					goto finalization;

				if (BitArray_getLength(outSymbol) + BitArray_getLength(outBuffer) > outBuffer->size * 8)
				{
					for (j = 0; BitArray_getLength(outBuffer) < outBuffer->size * 8; j++)
					{
						if ((result = BitArray_getBit(outSymbol, j, &bit)) != ERR_NO)
							goto finalization;
						if ((result = BitArray_pushBack(outBuffer, bit)) != ERR_NO)
							goto finalization;
					}

					if ((result = bfwrite_fast(arch, outBuffer->arr, outBuffer->byteLength)) != ERR_NO)
						goto finalization;
					fHead->fileSize += outBuffer->byteLength;
					BitArray_reset(outBuffer);

					for (; j < BitArray_getLength(outSymbol); j++)
					{
						if ((result = BitArray_getBit(outSymbol, j, &bit)) != ERR_NO)
							goto finalization;
						if ((result = BitArray_pushBack(outBuffer, bit)) != ERR_NO)
							goto finalization;
					}
				}
				else
				{
					if ((result = BitArray_concatenate(outBuffer, outSymbol)) != ERR_NO)
						goto finalization;
				}

				if ((result = bfwrite_fast(arch, outBuffer->arr, outBuffer->byteLength)) != ERR_NO)
					goto finalization;

				fHead->fileSize += outBuffer->byteLength;
			}
		}

		if ((result = bfwrite(arch, &(fHead->fileCrc), sizeof(fHead->fileCrc))) != ERR_NO)
			goto finalization;

		if (_fseeki64(arch, filePos, SEEK_SET) != 0)
		{
			result = ERR_WRITE_FAILED;
			goto finalization;
		}
		if ((result = File_Header_fwrite(arch, fHead)) != ERR_NO)
			goto finalization;

		if (_fseeki64(arch, 0, SEEK_END) != 0)
		{
			result = ERR_WRITE_FAILED;
			goto finalization;
		}
	}

finalization:
	if (inBuffer != NULL)
		free(inBuffer);
	if (outBuffer != NULL)
		BitArray_destroy(outBuffer);
	if (inSymbol != NULL)
		BitArray_destroy(inSymbol);
	if (outSymbol != NULL)
		BitArray_destroy(outSymbol);
	if (tree != NULL)
		FGKTree_destroy(tree);

	return result;
}


err_type arch_extractFile(FILE *arch, FILE *outFile, uint64_t bytesCount)
{
	uint8_t *buffer = NULL;
	uint32_t buf_size = BUFFER_SIZE;
	err_type result = ERR_NO;
	uint64_t temp;
	uint32_t crc = 0;
	void *temp2;

	if (bytesCount == 0)
		return ERR_NO;

	if ((buffer = (uint8_t *)calloc(buf_size, sizeof(uint8_t))) == NULL)
		return ERR_NOMEM;

	temp2 = NULL;
	while (bytesCount != 0)
	{
		temp = (bytesCount < buf_size) ? bytesCount : buf_size;

		if ((result = bfread(arch, buffer, temp)) != ERR_NO)
		{
			free(buffer);
			return result;
		}

		getCrc32(buffer, temp, &crc, temp2);
		temp2 = &crc;

		if (fwrite(buffer, temp, 1, outFile) != 1)
		{
			free(buffer);
			return ERR_WRITE_FAILED;
		}

		bytesCount -= temp;
	}

	free(buffer);

	temp = crc;

	if ((result = bfread(arch, &crc, sizeof(crc))) != ERR_NO)
		return result;

	return (temp == crc) ? ERR_NO : ERR_FILE_BADCRC;
}


err_type arch_extractFileCompress(FILE *arch, FILE *outFile, uint64_t bytesCount)
{
	err_type result = ERR_NO;
	BitArray inBuffer = NULL;
	uint8_t *outBuffer = NULL;
	BitArray outByte = NULL;
	Node *currentNode = NULL;
	FGKTree tree = NULL;
	uint64_t readSize, i, outBufferLength = 0;
	uint8_t bit;
	SymbolState state = EXISTS;
	uint32_t crc, *oldCrc = NULL, crc2;

	if (bytesCount == 0)
		return ERR_NO;

	if ((inBuffer = BitArray_create(BUFFER_SIZE * 8)) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((outBuffer = (uint8_t *)calloc(BUFFER_SIZE, sizeof(uint8_t))) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((tree = FGKTree_create()) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((outByte = BitArray_create(8)) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}

	currentNode = tree->start;
	while (bytesCount > 0)
	{
		readSize = min(BUFFER_SIZE, bytesCount);

		bfread(arch, inBuffer->arr, readSize*sizeof(uint8_t));
		inBuffer->byteLength = readSize;
		inBuffer->length = readSize * 8;

		for (i = 0; i < BitArray_getLength(inBuffer); i++)
		{
			if ((result = BitArray_getBit(inBuffer, i, &bit)) != ERR_NO)
				goto finalization;
			
			if (state == NEW)
			{
				if ((result = BitArray_pushBack(outByte, bit)) != ERR_NO)
					goto finalization;

				if (BitArray_getLength(outByte) == 8)
				{
					if (outBufferLength == BUFFER_SIZE)
					{
						getCrc32(outBuffer, BUFFER_SIZE, &crc, oldCrc);
						oldCrc = &crc;

						if (fwrite(outBuffer, BUFFER_SIZE, 1, outFile) == 0)
						{
							result = ERR_WRITE_FAILED;
							goto finalization;
						}
						outBufferLength = 0;
					}

					outBuffer[outBufferLength++] = outByte->arr[0];
					state = EXISTS;

					if ((result = FGKTree_addByte(tree, outByte->arr[0])) != ERR_NO)
						goto finalization;
					if ((result = FGKTree_update(tree, outByte->arr[0])) != ERR_NO)
						goto finalization;
				}
			}
			else
			{
				if (bit == 0)
					currentNode = currentNode->left;
				else
					currentNode = currentNode->right;

				if (currentNode == NULL)
				{
					result = ERR_FILE_BADCRC;
					goto finalization;
				}

				switch (currentNode->byte)
				{
					case NULLNODEBYTE:
						state = NEW;
						currentNode = tree->start;
						BitArray_reset(outByte);
						break;
					case FILEENDBYTE:
						i = BitArray_getLength(inBuffer);
						bytesCount = readSize;
						break;
					case NOTBYTE:
						break;
					default:
						if (outBufferLength == BUFFER_SIZE)
						{
							getCrc32(outBuffer, BUFFER_SIZE, &crc, oldCrc);
							oldCrc = &crc;

							if (fwrite(outBuffer, BUFFER_SIZE, 1, outFile) == 0)
							{
								result = ERR_WRITE_FAILED;
								goto finalization;
							}
							outBufferLength = 0;
						}

						outBuffer[outBufferLength++] = (uint8_t)currentNode->byte;

						if ((result = FGKTree_update(tree, (uint8_t)currentNode->byte)) != ERR_NO)
							goto finalization;

						currentNode = tree->start;
						break;
				}
			}
		}

		bytesCount -= readSize;
		if (bytesCount == 0)
		{
			getCrc32(outBuffer, outBufferLength, &crc, oldCrc);

			if (fwrite(outBuffer, outBufferLength, 1, outFile) == 0)
			{
				result = ERR_WRITE_FAILED;
				goto finalization;
			}
		}
	}

	if ((result = bfread(arch, &crc2, sizeof(crc2))) != ERR_NO)
		goto finalization;

	result = (crc == crc2 ? ERR_NO : ERR_FILE_BADCRC);

finalization:
	if (inBuffer != NULL)
		BitArray_destroy(inBuffer);
	if (outBuffer != NULL)
		free(outBuffer);
	if (outByte != NULL)
		BitArray_destroy(outByte);
	if (tree != NULL)
		FGKTree_destroy(tree);

	return result;
}


err_type arch_deleteFile(FILE *arch, const uint8_t *fileName) //1.режим чтения-записи 2.после работы всегда в конце файла
{
	uint8_t *buffer = NULL;
	File_Header fHead;
	uint32_t fHeadSize;
	err_type result;
	uint64_t readSize;
	int64_t writePos, readPos;
	int fileNo;

	if ((arch == NULL) || (fileName == NULL))
		return ERR_PARAM_INVALID;

	if ((result = arch_findFile(arch, fileName, &fHead)) == ERR_NO)
	{
		if ((buffer = (uint8_t *)calloc(BUFFER_SIZE, sizeof(uint8_t))) == NULL)
			return ERR_NOMEM;

		fHeadSize = File_Header_getSize(&fHead);

		if (_fseeki64(arch, -((long long)fHeadSize), SEEK_CUR) != 0)
			result = ERR_READ_FAILED;
		else
		{
			if ((writePos = _ftelli64(arch)) == -1)
				result = ERR_READ_FAILED;
			
			if (fHead.fileSize != 0)
				readPos = writePos + fHeadSize + fHead.fileSize + sizeof(fHead.fileCrc);
			else
				readPos = writePos + fHeadSize;
			readSize = min(BUFFER_SIZE*sizeof(uint8_t), fHeadSize + fHead.fileSize + sizeof(fHead.fileCrc));

			do
			{
				if (_fseeki64(arch, readPos, SEEK_SET) != 0)
				{
					result = ERR_READ_FAILED;
					break;
				}
						
				if ((readSize = fread(buffer, sizeof(uint8_t), readSize, arch)*sizeof(uint8_t)) != 0)
				{
					if (_fseeki64(arch, writePos, SEEK_SET) != 0)
					{
						result = ERR_WRITE_FAILED;
						break;
					}

					if (fwrite(buffer, readSize*sizeof(uint8_t), 1, arch) == 0)
					{
						result = ERR_WRITE_FAILED;
						break;
					}
					fflush(arch);
				}
				else
				{
					if (!feof(arch))
					{
						result = ERR_READ_FAILED;
						break;
					}
				}

				readPos += readSize;
				writePos += readSize;
			}
			while (!feof(arch));

			if (_fseeki64(arch, writePos, SEEK_SET) != 0)
				result = ERR_WRITE_FAILED;
			else
			{
				fileNo = _fileno(arch);
				if (_chsize_s(fileNo, writePos) != 0)
					result = ERR_WRITE_FAILED;
			}
		}

		free(buffer);
	}

	return result;
}


err_type arch_getFileList(FILE *arch, File_Header *result, uint64_t maxCount, uint64_t *resultCount)
{
	err_type err;

	if (resultCount == NULL)
		return ERR_PARAM_INVALID;

	for (*resultCount = 0; *resultCount < maxCount; )
	{
		if ((err = File_Header_fread(arch, result)) != ERR_NO)
			return err;
		(*resultCount)++;
		
		if (result->fileSize != 0)
		{
			if (_fseeki64(arch, result->fileSize + sizeof(result->fileCrc), SEEK_CUR) != 0)
				return ERR_READ_FAILED;
		}

		if ((fgetc(arch) == EOF) && (feof(arch)))
			break;

		if (_fseeki64(arch, -1, SEEK_CUR) != 0)
			return ERR_READ_FAILED;

		result++;
	}

	return ERR_NO;
}


err_type arch_findFile(FILE *arch, char *fileName, File_Header *fHead_result)
{
	err_type result;

	while (!feof(arch))
	{
		if ((result = File_Header_fread(arch, fHead_result)) != ERR_NO)
			return result;
		fHead_result->fileName[fHead_result->fileNameLength] = 0;

		if (strcmp(fHead_result->fileName, fileName) == 0)
			return ERR_NO;
			
		if (fHead_result->fileSize != 0)
		{
			if (_fseeki64(arch, fHead_result->fileSize + sizeof(fHead_result->fileCrc), SEEK_CUR) != 0)
				return ERR_READ_FAILED;
		}
		if (fgetc(arch) == EOF)
			break;
		if (_fseeki64(arch, -1, SEEK_CUR) != 0)
			return ERR_READ_FAILED;
	}

	return ERR_ARCH_FILENOTFOUND;
}


err_type arch_checkMarkerBlock(FILE *arch)
{
	uint32_t version, marker;
	err_type result = ERR_NO;

	if ((result = bfread(arch, &marker, sizeof(marker))) != ERR_NO)
		return result;

	if (marker != ARCH_MARKER)
		return ERR_ARCH_BADMARKER;

	if ((result = bfread(arch, &version, sizeof(version))) != ERR_NO)
		return result;

	if (version > ARCH_VERSION)
		return ERR_ARCH_OLDVERSION;

	return ERR_NO;
}


err_type arch_writeMarkerBlock(FILE *arch)
{
	uint32_t version = ARCH_VERSION, marker = ARCH_MARKER;
	err_type result;

	if ((result = bfwrite_fast(arch, &marker, sizeof(marker))) != ERR_NO)
		return result;
	if ((result = bfwrite_fast(arch, &version, sizeof(version))) != ERR_NO)
		return result;

	return ERR_NO;
}

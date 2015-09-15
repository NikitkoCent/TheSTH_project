#include "file_header.h"

err_type File_Header_checkCrc(const File_Header *fHead)
{
	uint16_t crc;

	if (fHead == NULL)
		return ERR_PARAM_INVALID;

	getCrc16(&(fHead->fileNameLength), sizeof(fHead->fileNameLength), &crc, NULL);
	getCrc16(&(fHead->fileName), fHead->fileNameLength * sizeof(fHead->fileName[0]), &crc, &crc);
	getCrc16(&(fHead->attr), sizeof(fHead->attr), &crc, &crc);
	getCrc16(&(fHead->method), sizeof(fHead->method), &crc, &crc);
	getCrc16(&(fHead->fileSize), sizeof(fHead->fileSize), &crc, &crc);
	//getCrc16(&(fHead->fileCrc), sizeof(fHead->fileCrc), &crc, &crc);

	return (crc == fHead->headCrc) ? ERR_NO : ERR_HEAD_BADCRC;
}


void File_Header_setCrc(File_Header *fHead)
{
	if (fHead == NULL)
		return;

	getCrc16(&(fHead->fileNameLength), sizeof(fHead->fileNameLength), &(fHead->headCrc), NULL);
	getCrc16(fHead->fileName, fHead->fileNameLength * sizeof(fHead->fileName[0]), &(fHead->headCrc), &(fHead->headCrc));
	getCrc16(&(fHead->attr), sizeof(fHead->attr), &(fHead->headCrc), &(fHead->headCrc));
	getCrc16(&(fHead->method), sizeof(fHead->method), &(fHead->headCrc), &(fHead->headCrc));
	getCrc16(&(fHead->fileSize), sizeof(fHead->fileSize), &(fHead->headCrc), &(fHead->headCrc));
	//getCrc16(&(fHead->fileCrc), sizeof(fHead->fileCrc), &(fHead->headCrc), &(fHead->headCrc));
}


err_type File_Header_fwrite(FILE *f, const File_Header *fHead)
{
	err_type result = ERR_NO;

	if ((f == NULL) || (fHead == NULL))
		return ERR_PARAM_INVALID;

	File_Header_setCrc(fHead);

	if ((result = bfwrite(f, &(fHead->headCrc), sizeof(fHead->headCrc))) != ERR_NO)
		return result;
	if ((result = bfwrite(f, &(fHead->fileNameLength), sizeof(fHead->fileNameLength))) != ERR_NO)
		return result;
	if ((result = bfwrite(f, fHead->fileName, fHead->fileNameLength * sizeof(fHead->fileName[0]))) != ERR_NO)
		return result;
	if ((result = bfwrite(f, &(fHead->attr), sizeof(fHead->attr))) != ERR_NO)
		return result;
	if ((result = bfwrite(f, &(fHead->method), sizeof(fHead->method))) != ERR_NO)
		return result;
	/*if ((result = bfwrite(f, &(fHead->fileSize), sizeof(fHead->fileSize))) != ERR_NO)
		return result;
	result = bfwrite(f, &(fHead->fileCrc), sizeof(fHead->fileCrc));*/
	result = bfwrite(f, &(fHead->fileSize), sizeof(fHead->fileSize));
	return result;
}


err_type File_Header_fread(FILE *f, File_Header *fHead)
{
	err_type result;

	if ((f == NULL) || (fHead == NULL))
		return ERR_PARAM_INVALID;

	if ((result = bfread(f, &(fHead->headCrc), sizeof(fHead->headCrc))) != ERR_NO)
		return result;
	if ((result = bfread(f, &(fHead->fileNameLength), sizeof(fHead->fileNameLength))) != ERR_NO)
		return result;
	if ((result = bfread(f, fHead->fileName, fHead->fileNameLength * sizeof(fHead->fileName[0]))) != ERR_NO)
		return result;
	if ((result = bfread(f, &(fHead->attr), sizeof(fHead->attr))) != ERR_NO)
		return result;
	if ((result = bfread(f, &(fHead->method), sizeof(fHead->method))) != ERR_NO)
		return result;
	if ((result = bfread(f, &(fHead->fileSize), sizeof(fHead->fileSize))) != ERR_NO)
		return result;
	/*if ((result = bfread(f, &(fHead->fileCrc), sizeof(fHead->fileCrc))) != ERR_NO)
		return result;*/

	return File_Header_checkCrc(fHead);
}


uint16_t File_Header_getSize(const File_Header *fHead)
{
	if (fHead == NULL)
		return 0;

	return (sizeof(fHead->headCrc) + sizeof(fHead->fileNameLength) + fHead->fileNameLength*sizeof(fHead->fileName[0]) + sizeof(fHead->attr)
		    + sizeof(fHead->method) + sizeof(fHead->fileSize));
}
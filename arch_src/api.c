#include "api.h"

typedef Token FileName_FS;

static err_type _getStat(const char *fName, struct stat *fInfo)
{
	if (stat(fName, fInfo) == -1)
	{
		switch (errno)
		{
		case ENOENT:
		case ENOTDIR:
		case ELOOP:
		case EFAULT:
			return ERR_FILE_NOTEXISTS;
		case EACCES:
			return ERR_NOREADACCESS;
		case ENOMEM:
			return ERR_NOMEM;
		case ENAMETOOLONG:
			return ERR_NAME_TOOLONG;
		default:
			return ERR_UNKNOWN;
		}
	}

	return ERR_NO;
}


static err_type _File_Header_fill(const char *fileName, const uint8_t compression, File_Header *fHead, const struct stat *fInfo)
{
	size_t temp;

	temp = strlen(fileName);
	fHead->fileNameLength = temp;
	if (temp > fHead->fileNameLength)
		return ERR_NAME_TOOLONG;
	strcpy(fHead->fileName, fileName);
	fHead->method = compression;
	fHead->attr = fInfo->st_mode;
	
	fHead->fileSize = fInfo->st_size;
	File_Header_setCrc(fHead);

	return ERR_NO;
}


static void _getFileDir(const char *fileName, char *result)
{
	FileName fileDir;

	_splitpath(fileName, fileDir.drive, fileDir.dir, fileDir.fName, fileDir.ext);
	_makepath(result, fileDir.drive, fileDir.dir, NULL, NULL);

	if ((strcmp(fileDir.dir, "\\") != 0) && (strcmp(fileDir.dir, "") != 0))
		result[strlen(result) - 1] = 0;
}


static err_type _checkDirAccess(const char *fileName)
{
	char dirPath[_MAX_PATH];
	struct stat fInfo;
	err_type result;

	_getFileDir(fileName, dirPath);

	if ((result = _getStat(dirPath, &fInfo)) != ERR_NO)
		return result;
	if ((fInfo.st_mode & S_IREAD) == 0)
		return ERR_NOREADACCESS;
	if ((fInfo.st_mode & S_IWRITE) == 0)
		return ERR_NOWRITEACCESS;

	return ERR_NO;
}


static err_type _addFile(FILE *arch, const Token *fileName, const struct stat *fInfo, const uint8_t compression)
{
	FILE *file;
	char fileNameOut_full[_MAX_PATH];
	char fileNameOut_short[_MAX_PATH];
	File_Header fHead;
	err_type result = ERR_NO;

	_makepath(fileNameOut_short, fileName->shortName.drive, fileName->shortName.dir, fileName->shortName.fName, fileName->shortName.ext);
	_makepath(fileNameOut_full, fileName->fullName.drive, fileName->fullName.dir, fileName->fullName.fName, fileName->fullName.ext);

	if ((result = _File_Header_fill(fileNameOut_short, compression, &fHead, fInfo)) != ERR_NO)
		return result;

	if ((file = fopen(fileNameOut_full, "rb")) == NULL)
		return ERR_READ_FAILED;

	if (compression == 0)
		result = arch_addFile(arch, file, &fHead);
	else
		result = arch_addFileCompress(arch, file, &fHead);

	fclose(file);

	return result;
}


static err_type _copyArchWithoutFile(FILE *archDst, FILE *archSrc, const char *fileName)
{
	err_type result;
	File_Header fHead;
	uint8_t *buffer = NULL;
	uint8_t mask[_MAX_PATH] = "#@$%";
	uint64_t readSize;
	int ch;

	if ((result = arch_checkMarkerBlock(archSrc)) != ERR_NO)
		return result;
	if ((result = arch_writeMarkerBlock(archDst)) != ERR_NO)
		return result;

	if ((buffer = (uint8_t *)calloc(BUFFER_SIZE, sizeof(uint8_t))) == NULL)
		return ERR_NOMEM;

	ch = getc(archSrc);
	while (!feof(archSrc))
	{
		ungetc(ch, archSrc);
		
		if ((result = File_Header_fread(archSrc, &fHead)) != ERR_NO)
			break;
		fHead.fileName[fHead.fileNameLength] = 0;

		if (strcmp(fHead.fileName, fileName) == 0)
		{
			if (_fseeki64(archSrc, fHead.fileSize, SEEK_CUR) != 0)
			{
				result = ERR_READ_FAILED;
				break;
			}
			if (fHead.fileSize > 0)
			{
				if (_fseeki64(archSrc, sizeof(fHead.fileCrc), SEEK_CUR) != 0)
				{
					result = ERR_READ_FAILED;
					break;
				}
			}

			break;
		}
		else
		{
			if ((result = File_Header_fwrite(archDst, &fHead)) != ERR_NO)
				break;

			if (fHead.fileSize > 0)
			{
				while (fHead.fileSize > 0)
				{
					readSize = min(BUFFER_SIZE, fHead.fileSize);

					if (fread(buffer, readSize * sizeof(uint8_t), 1, archSrc) == 0)
					{
						result = feof(archSrc) ? ERR_EOF : ERR_READ_FAILED;
						break;
					}
					if (fwrite(buffer, readSize * sizeof(uint8_t), 1, archDst) == 0)
					{
						result = ERR_WRITE_FAILED;
						break;
					}

					fHead.fileSize -= readSize;
				}
				if (result != ERR_NO)
					break;

				if (fread(&fHead.fileCrc, sizeof(fHead.fileCrc), 1, archSrc) == 0)
				{
					result = feof(archSrc) ? ERR_EOF : ERR_READ_FAILED;
					break;
				}
				if (fwrite(&fHead.fileCrc, sizeof(fHead.fileCrc), 1, archDst) == 0)
				{
					result = ERR_WRITE_FAILED;
					break;
				}
			}
		}

		ch = getc(archSrc);
	}

	if ((result == ERR_NO) && (!feof(archSrc)))
	{
		if ((fHead.attr & S_IFDIR) != 0)
			strcat(strcpy(mask, fHead.fileName), "\\");

		ch = getc(archSrc);
		while (!feof(archSrc))
		{
			ungetc(ch, archSrc);

			if ((result = File_Header_fread(archSrc, &fHead)) != ERR_NO)
				break;
			fHead.fileName[fHead.fileNameLength] = 0;

			if (strstr(fHead.fileName, mask) == fHead.fileName)
			{
				if (_fseeki64(archSrc, fHead.fileSize, SEEK_CUR) != 0)
				{
					result = ERR_READ_FAILED;
					break;
				}
				if (fHead.fileSize > 0)
				{
					if (_fseeki64(archSrc, sizeof(fHead.fileCrc), SEEK_CUR) != 0)
					{
						result = ERR_READ_FAILED;
						break;
					}
				}
			}
			else
			{
				if ((result = File_Header_fwrite(archDst, &fHead)) != ERR_NO)
					break;

				if (fHead.fileSize == 0)
					continue;

				while (fHead.fileSize > 0)
				{
					readSize = min(BUFFER_SIZE, fHead.fileSize);

					if (fread(buffer, readSize * sizeof(uint8_t), 1, archSrc) == 0)
					{
						result = feof(archSrc) ? ERR_EOF : ERR_READ_FAILED;
						break;
					}
					if (fwrite(buffer, readSize * sizeof(uint8_t), 1, archDst) == 0)
					{
						result = ERR_WRITE_FAILED;
						break;
					}

					fHead.fileSize -= readSize;
				}
				if (result != ERR_NO)
					break;

				if (fread(&fHead.fileCrc, sizeof(fHead.fileCrc), 1, archSrc) == 0)
				{
					result = feof(archSrc) ? ERR_EOF : ERR_READ_FAILED;
					break;
				}
				if (fwrite(&fHead.fileCrc, sizeof(fHead.fileCrc), 1, archDst) == 0)
				{
					result = ERR_WRITE_FAILED;
					break;
				}
			}

			ch = getc(archSrc);
		}
	}

	free(buffer);
	return result;
}


err_type archive_addFile(const char *archiveName, const char *fileName, const uint8_t compression) //!!!œŒÀÕ€≈ »Ã≈Õ¿!!!
{
	FILE *arch, *oldArch;
	err_type result = ERR_NO;
	struct stat objInfo;
	Stack *dirs = NULL;
	char *tempArchName = NULL;
	char buffer[_MAX_PATH];
	FileName_FS outFileName;
	File_Header dirHead;

	if ((archiveName == NULL) || (fileName == NULL))
		return ERR_PARAM_INVALID;
	
	if ((result = _checkDirAccess(archiveName)) != ERR_NO)
		return result;

	_getFileDir(archiveName, buffer);
	if ((tempArchName = _tempnam(buffer, "sth")) == NULL)
		return ERR_WRITE_FAILED;

	if ((arch = fopen(tempArchName, "wb")) == NULL)
	{
		free(tempArchName);
		return ERR_WRITE_FAILED;
	}

	if ((oldArch = fopen(archiveName, "rb")) != NULL)
	{
		_splitpath(fileName, NULL, NULL, outFileName.shortName.fName, outFileName.shortName.ext);
		_makepath(buffer, NULL, NULL, outFileName.shortName.fName, outFileName.shortName.ext);
		result = _copyArchWithoutFile(arch, oldArch, buffer);
		fclose(oldArch);
	}
	else
	{
		result = arch_writeMarkerBlock(arch);
	}
		
	if (result != ERR_NO)
		goto finalization;

	/*if ((_fseeki64(arch, 0, SEEK_END)) != 0)
	{
		result = ERR_WRITE_FAILED;
		goto finalization;
	}*/

	if ((dirs = Stack_Create()) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}

	if ((result = _getStat(fileName, &objInfo)) != ERR_NO)
		goto finalization;

	_splitpath(fileName, outFileName.fullName.drive, outFileName.fullName.dir, outFileName.fullName.fName, outFileName.fullName.ext);
	_splitpath(fileName, NULL, NULL, outFileName.shortName.fName, outFileName.shortName.ext);
	outFileName.shortName.drive[0] = 0;
	outFileName.shortName.dir[0] = 0;

	if ((objInfo.st_mode & S_IFREG) != 0)
	{
		result = _addFile(arch, &outFileName, &objInfo, compression);
	}
	else if ((objInfo.st_mode & S_IFDIR) != 0)
	{
		if ((result = Stack_Push(dirs, &outFileName)) != ERR_NO)
			goto finalization;
	}
	else
	{
		result = ERR_UNKNOWN_FILETYPE;
		goto finalization;
	}

	while (!Stack_IsEmpty(dirs))
	{
		WIN32_FIND_DATAA fileFindData;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		Stack_Pop(dirs, &outFileName);

		_makepath(buffer, outFileName.fullName.drive, outFileName.fullName.dir, outFileName.fullName.fName, outFileName.fullName.ext);
		if ((result = _getStat(buffer, &objInfo)) != ERR_NO)
			goto finalization;
		objInfo.st_size = 0;
		_makepath(buffer, outFileName.shortName.drive, outFileName.shortName.dir, outFileName.shortName.fName, outFileName.shortName.ext);
		if ((result = _File_Header_fill(buffer, compression, &dirHead, &objInfo)) != ERR_NO)
			goto finalization;

		if ((result = File_Header_fwrite(arch, &dirHead)) != ERR_NO)
			goto finalization;

		_makepath(buffer, outFileName.fullName.drive, outFileName.fullName.dir, outFileName.fullName.fName, outFileName.fullName.ext);
		strcat(buffer, "\\*");

		if ((hFind = FindFirstFileA(buffer, &fileFindData)) == INVALID_HANDLE_VALUE)
		{
			result = ERR_UNKNOWN;
			goto finalization;
		}
		if (FindNextFileA(hFind, &fileFindData) == 0)
		{
			continue;
		}
		if (strcmp(fileFindData.cFileName, "..") == 0)
			if (FindNextFileA(hFind, &fileFindData) == 0)
				continue;

		strcat(strcat(strcat(outFileName.fullName.dir, outFileName.fullName.fName), outFileName.fullName.ext), "\\");
		strcat(strcat(strcat(outFileName.shortName.dir, outFileName.shortName.fName), outFileName.shortName.ext), "\\");
		do
		{
			_splitpath(fileFindData.cFileName, NULL, NULL, outFileName.fullName.fName, outFileName.fullName.ext);	
			_splitpath(fileFindData.cFileName, NULL, NULL, outFileName.shortName.fName, outFileName.shortName.ext);

			_makepath(buffer, outFileName.fullName.drive, outFileName.fullName.dir, outFileName.fullName.fName, outFileName.fullName.ext);
			if ((result = _getStat(buffer, &objInfo)) != ERR_NO)
				break;


			if ((objInfo.st_mode & S_IFREG) != 0)
			{
				if ((result = _addFile(arch, &outFileName, &objInfo, compression)) != ERR_NO)
					break;
			}
			else if ((objInfo.st_mode & S_IFDIR) != 0)
			{
				if ((result = Stack_Push(dirs, &outFileName)) != ERR_NO)
					break;
			}
			else
			{
				result = ERR_UNKNOWN_FILETYPE;
				break;
			}
		} 
		while (FindNextFileA(hFind, &fileFindData) != 0);
		FindClose(hFind);

		if (result != ERR_NO)
			goto finalization;
	}

finalization:
	fclose(arch);

	if (result == ERR_NO)
	{
		remove(archiveName);
		rename(tempArchName, archiveName);
	}
	else
	{
		remove(tempArchName);
	}

	free(tempArchName);

	Stack_Destroy(dirs);

	return result;
}


err_type archive_extractFile(const char *archiveName, const char *fileName, const char *dirPathOut)
{
	FILE *arch = NULL, *file = NULL;
	struct stat objInfo;
	err_type result;
	FileName outFileName;
	File_Header header;
	char buffer[_MAX_PATH];

	if ((archiveName == NULL) || (fileName == NULL) || (dirPathOut == NULL))
		return ERR_PARAM_INVALID;

	if ((result = _getStat(dirPathOut, &objInfo)) != ERR_NO)
	{
		if (result == ERR_FILE_NOTEXISTS)
			return ERR_DIR_NOTEXISTS;
		else
			return result;
	}
	if ((objInfo.st_mode & S_IFDIR) == 0)
		return ERR_DIR_NOTEXISTS;
	if ((objInfo.st_mode & S_IWRITE) == 0)
		return ERR_NOWRITEACCESS;

	if ((result = _getStat(archiveName, &objInfo)) != ERR_NO)
		return result;
	if ((objInfo.st_mode & S_IFREG) == 0)
		return ERR_FILE_NOTEXISTS;
	if ((objInfo.st_mode & S_IREAD) == 0)
		return ERR_NOREADACCESS;

	if ((arch = fopen(archiveName, "rb")) == NULL)
		return ERR_READ_FAILED;
	if ((result = arch_checkMarkerBlock(arch)) != ERR_NO)
	{
		fclose(arch);
		return result;
	}

	_splitpath(dirPathOut, outFileName.drive, outFileName.dir, outFileName.fName, outFileName.ext);
	strcat(strcat(strcat(outFileName.dir, outFileName.fName), outFileName.ext), "\\");
	outFileName.fName[0] = 0;
	outFileName.ext[0] = 0;

	if ((result = arch_findFile(arch, fileName, &header)) == ERR_NO)
	{
		header.fileName[header.fileNameLength] = 0;
		
		if ((header.attr & S_IFREG) != 0)
		{
			_splitpath(header.fileName, NULL, NULL, outFileName.fName, outFileName.ext);
			_makepath(buffer, outFileName.drive, outFileName.dir, outFileName.fName, outFileName.ext);
			
			if ((file = fopen(buffer, "wb")) == NULL)
			{
				result = ERR_WRITE_FAILED;
				goto finalization;
			}

			if (header.method == 0)
				result = arch_extractFile(arch, file, header.fileSize);
			else
				result = arch_extractFileCompress(arch, file, header.fileSize);
		}
		else if ((header.attr & S_IFDIR) != 0)
		{
			char dirName[_MAX_PATH];
			
			strcpy(dirName, header.fileName);

			do
			{
				header.fileName[header.fileNameLength] = 0;
				
				if (strstr(header.fileName, dirName) == header.fileName)
				{
					_makepath(buffer, outFileName.drive, outFileName.dir, outFileName.fName, outFileName.ext);
					strcat(buffer, header.fileName);

					if ((header.attr & S_IFREG) != 0)
					{
						if ((file = fopen(buffer, "wb")) == NULL)
						{
							result = ERR_WRITE_FAILED;
							goto finalization;
						}

						if (header.method == 0)
							result = arch_extractFile(arch, file, header.fileSize);
						else
							result = arch_extractFileCompress(arch, file, header.fileSize);
						
						fclose(file);
							
						if (result != ERR_NO)
						{
							goto finalization;
						}
					}
					else if ((header.attr & S_IFDIR) != 0)
					{
						mkdir(buffer);
					}
					else
					{
						result = ERR_UNKNOWN_FILETYPE;
						goto finalization;
					}
				}
				else if (header.fileSize != 0)
				{
					if (_fseeki64(arch, header.fileSize + sizeof(header.fileCrc), SEEK_CUR) != 0)
					{
						result = ERR_READ_FAILED;
						goto finalization;
					}
					if (fgetc(arch) != EOF)
					{
						if (_fseeki64(arch, -1, SEEK_CUR) != 0)
						{
							result = ERR_READ_FAILED;
							goto finalization;
						}
					}
					else
						break;
				}
			}
			while ((result = File_Header_fread(arch, &header)) == ERR_NO);
			result = ERR_NO;
		}
		else
		{
			result = ERR_UNKNOWN_FILETYPE;
			goto finalization;
		}
	}

finalization:
	if (arch != NULL)
		fclose(arch);

	return result;
}


err_type archive_deleteFile(const char *archiveName, const char *fileName)
{
	FILE *arch = NULL;
	struct stat info;
	err_type result;

	if ((result = _getStat(archiveName, &info)) != ERR_NO)
		return result;

	if ((info.st_mode & S_IFREG) == 0)
		return ERR_FILE_NOTEXISTS;
	if ((info.st_mode & S_IREAD) == 0)
		return ERR_NOREADACCESS;
	if ((info.st_mode & S_IWRITE) == 0)
		return ERR_NOWRITEACCESS;

	if ((arch = fopen(archiveName, "r+b")) == NULL)
		return ERR_WRITE_FAILED;

	if ((result = arch_checkMarkerBlock(arch)) == ERR_NO)
	{
		result = arch_deleteFile(arch, fileName);
	}
	fclose(arch);

	return result;
}


err_type archive_getFileList(const char *archiveName, void (*callback)(File_Header *fHead, void *data), const void *data)
{
	FILE *arch;
	File_Header fHead[10];
	uint64_t readCount, i;
	err_type result = ERR_NO;

	if (callback == NULL)
		return ERR_PARAM_INVALID;

	if ((arch = fopen(archiveName, "rb")) == NULL)
		return ERR_READ_FAILED;

	if ((result = arch_checkMarkerBlock(arch)) == ERR_NO)
	{
		while ((result = arch_getFileList(arch, fHead, 10, &readCount)) == ERR_NO)
		{
			for (i = 1; i <= readCount; i++)
			{
				callback(fHead + i - 1, data);
			}
		}

		if ((result == ERR_EOF) && (readCount == 0))
			result = ERR_NO;
	}

	fclose(arch);
	return result;
}
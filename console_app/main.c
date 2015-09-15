#include "../arch_src/api.h"
#include <stdio.h>
#include <string.h>

void printFileInfo(File_Header *fInfo, void *file)
{
	fInfo->fileName[fInfo->fileNameLength] = 0;
	fprintf(file, "NAME : \" %s \" ; SIZE : %lld ; ", fInfo->fileName, fInfo->fileSize);

	if ((fInfo->attr & S_IFREG) != 0)
		fprintf(file, "TYPE : file ; ");
	else
		fprintf(file, "TYPE : directory ; ");

	if (fInfo->method == 0)
		fprintf(file, "not compressed ; ");
	else
		fprintf(file, "compressed ; ");

	fprintf(file, "\n");
}

void printHelp()
{
	printf("======== TheStorageHouse archiver ===========\n");
	printf("Run this program with parameters: \n");
	printf("  <operation> <arch_name> [<file_name1>] [<file_name2>] ...\n");
	printf("    where : \n");
	printf("	  <operation> is :\n");
	printf("		'ac' - add files to archive with compression\n");
	printf("		'au' - add files to archive without compression\n");
	printf("		'x'  - extract files from archive\n");
	printf("		'l'  - print files list from archive\n");
	printf("		'd'  - delete files from archive\n");
	printf("	  <arch_name> - full name of archive\n");
	printf("	  [<file_name>] - name of needed file\n");
	printf("=============================================\n");
}

void printErrInfo(const err_type err)
{
	switch (err)
	{
	case ERR_NO:
		printf("Operation is successful.\n");
		break;
	case ERR_NOMEM:
		printf("Not enough memory.\n");
		break;
	case ERR_EOF:
		printf("Unexpected end of file.\n");
		break;
	case ERR_WRITE_FAILED:
		printf("Write error.\n");
		break;
	case ERR_READ_FAILED:
		printf("Read error.\n");
		break;
	case ERR_FILE_BADCRC:
	case ERR_ARCH_BADMARKER:
	case ERR_HEAD_BADCRC:
		printf("Archive is corrupted.\n");
		break;
	case ERR_ARCH_OLDVERSION:
		printf("Archiver's version is old\n.");
		break;
	case ERR_ARCH_FILENOTFOUND:
		printf("File in archive not found.\n");
		break;
	case ERR_FILE_NOTEXISTS:
		printf("File not exists.\n");
		break;
	case ERR_DIR_NOTEXISTS:
		printf("Directory not exists.\n");
		break;
	case ERR_NAME_TOOLONG:
		printf("Name of archive or file is too long.\n");
		break;
	case ERR_NOREADACCESS:
		printf("No read access.\n");
		break;
	case ERR_NOWRITEACCESS:
		printf("No write access.\n");
		break;
	case ERR_UNKNOWN_FILETYPE:
		printf("Unknown type of file.\n");
		break;
	case ERR_UNKNOWN:
		printf("Unexpected error.\n");
		break;
	default:
		printf("Internal archiver's error.\n");
		break;
	}
}

int main(int argc, char *argv[])
{	
	int i;
	err_type result;
	char fullFileName[_MAX_PATH], fullArchName[_MAX_PATH];

	if (argc < 3)
	{
		printHelp();
		return 0;
	}

	getcwd(fullArchName, _MAX_PATH);
	strcat(fullArchName, "\\");

	if (strcmp(argv[1], "au") == 0)
	{
		uint16_t temp = strlen(fullFileName);
		
		if (argc < 4)
		{
			printHelp();
			return 0;
		}

		if (strstr(argv[2], ":") != NULL)
			strcpy(fullArchName, argv[2]);
		else
			strcat(fullArchName, argv[2]);

		for (i = 3; i < argc; i++)
		{
			getcwd(fullFileName, _MAX_PATH);
			strcat(fullFileName, "\\");
			if (strstr(argv[i], ":") != NULL)
				strcpy(fullFileName, argv[i]);
			else
				strcat(fullFileName, argv[i]);

			result = archive_addFile(fullArchName, fullFileName, 0);

			if (result != ERR_NO)
			{
				printf("Error at file \"%s\" ; archive \"%s\"\n" , argv[i], argv[2]);
				break;
			}
		}
	}
	else if ((strcmp(argv[1], "ac") == 0) || (strcmp(argv[1], "a") == 0))
	{
		if (argc < 4)
		{
			printHelp();
			return 0;
		}

		if (strstr(argv[2], ":") != NULL)
			strcpy(fullArchName, argv[2]);
		else
			strcat(fullArchName, argv[2]);

		for (i = 3; i < argc; i++)
		{
			getcwd(fullFileName, _MAX_PATH);
			strcat(fullFileName, "\\");
			if (strstr(argv[i], ":") != NULL)
				strcpy(fullFileName, argv[i]);
			else
				strcat(fullFileName, argv[i]);
			
			result = archive_addFile(fullArchName, fullFileName, 1);
			if (result != ERR_NO)
			{
				printf("Error at file \"%s\" ; archive \"%s\"\n", argv[i], argv[2]);
				break;
			}
		}
	}
	else if (strcmp(argv[1], "x") == 0)						//<op> <arch_name> <file_name1> <file_name2>...
	{
		getcwd(fullFileName, _MAX_PATH);

		if (argc < 4)
		{
			printHelp();
			return 0;
		}

		if (strstr(argv[2], ":") != NULL)
			strcpy(fullArchName, argv[2]);
		else
			strcat(fullArchName, argv[2]);

		for (i = 3; i < argc; i++)
		{
			result = archive_extractFile(fullArchName, argv[i], fullFileName);
			if (result != ERR_NO)
			{
				printf("Error at file \"%s\" ; archive \"%s\"\n", argv[i], argv[2]);
				break;
			}
			
		}
	}
	else if (strcmp(argv[1], "l") == 0)
	{
		if (strstr(argv[2], ":") != NULL)
			strcpy(fullArchName, argv[2]);
		else
			strcat(fullArchName, argv[2]);
		
		result = archive_getFileList(fullArchName, printFileInfo, stdout);
	}
	else if (strcmp(argv[1], "d") == 0)
	{
		if (argc < 4)
		{
			printHelp();
			return 0;
		}

		if (strstr(argv[2], ":") != NULL)
			strcpy(fullArchName, argv[2]);
		else
			strcat(fullArchName, argv[2]);

		for (i = 3; i < argc; i++)
		{
			result = archive_deleteFile(fullArchName, argv[i]);
			if (result != ERR_NO)
			{
				printf("Error at file \"%s\" ; archive \"%s\"", argv[i], argv[2]);
				break;
			}
		}
	}
	else
	{
		printHelp();
		return 0;
	}

	printErrInfo(result);

	return result;
}
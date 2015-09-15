#include "list.h"


List List_Create(const ListType startVal)
{
	List result = NULL;

	if ((result = (List)malloc(sizeof(struct List))) != NULL)
	{
		result->next = NULL;
		result->value = startVal;
	}

	return result;
}


void List_Free(List *list)
{
	List temp = NULL, temp2 = NULL;
	
	if (list != NULL)
	{
		temp = *list;
		while (temp != NULL)
		{
			temp2 = temp->next;
			free(temp);
			temp = temp2;
		}

		*list = NULL;
	}
}


int List_Insert(List list, const ListType val)
{
	if (list == NULL)
		return 1;

	while (list->next != NULL)
		list = list->next;

	if ((list->next = List_Create(val)) == NULL)
		return 2;

	return 0;
}


int List_IsEmpty(List list)
{
	return (list == NULL);
}
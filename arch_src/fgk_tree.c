#include "fgk_tree.h"

FGKTree FGKTree_create()
{
	FGKTree result;

	if ((result = (FGKTree)malloc(sizeof(struct List))) != NULL)
	{
		if ((result->start = (Node *)malloc(sizeof(Node))) == NULL)
		{
			free(result);
			result = NULL;
		}
		else
		{
			result->start->byte = NULLNODEBYTE;
			result->start->weight = 0;
			result->start->left = NULL;
			result->start->right = NULL;
			result->start->next = NULL;
			result->start->prev = NULL;
			result->start->parent = NULL;

			result->end = result->start;

			if (FGKTree_addByte(result, 0) != ERR_NO)
			{
				FGKTree_destroy(result);
				result = NULL;
			}
			else
			{
				result->start->left->byte = FILEENDBYTE;
				result->start->left->weight = 1;
				result->start->weight = 1;
				
				memset(result->byteNodes, 0, 256 * sizeof(Node *));
			}
		}
	}

	return result;
}


void FGKTree_destroy(FGKTree tree)
{
	if (tree != NULL)
	{
		while (tree->start != tree->end)
		{
			tree->start = tree->start->next;
			free(tree->start->prev);
		}
		free(tree->start);
		free(tree);
	}
}


err_type FGKTree_getNullNodeSymbol(const FGKTree tree, BitArray result)
{
	err_type err;
	Node *searchPtr;
	uint64_t length, i;
	uint8_t bit1, bit2;

	if ((tree == NULL) || (result == NULL))
		return ERR_PARAM_INVALID;

	BitArray_reset(result);

	searchPtr = tree->end;
	if (searchPtr == tree->start)
	{
		if ((err = BitArray_pushBack(result, 0)) != ERR_NO)
			return err;
	}
	else
	{
		while (searchPtr != tree->start)
		{
			if (searchPtr->parent->left == searchPtr)
			{
				err = BitArray_pushBack(result, 0);
			}
			else if (searchPtr->parent->right == searchPtr)
			{
				err = BitArray_pushBack(result, 1);
			}
			else
				err = ERR_TREEINVALID;

			if (err != ERR_NO)
				return err;

			searchPtr = searchPtr->parent;
		}
	}

	length = BitArray_getLength(result);
	for (i = 0; i < length / 2; i++)
	{
		if ((err = BitArray_getBit(result, i, &bit1)) != ERR_NO)
			return err;
		if ((err = BitArray_getBit(result, length - 1 - i, &bit2)) != ERR_NO)
			return err;
		if ((err = BitArray_setBit(result, length - 1 - i, bit1)) != ERR_NO)
			return err;
		if ((err = BitArray_setBit(result, i, bit2)) != ERR_NO)
			return err;
	}

	return ERR_NO;
}


err_type FGKTree_getFileEndSymbol(const FGKTree tree, BitArray result)
{
	Node *searchPtr;
	err_type err = ERR_NO;
	uint64_t length, i;
	uint8_t bit1, bit2;

	if ((tree == NULL) || (result == NULL))
		return ERR_PARAM_INVALID;

	BitArray_reset(result);

	searchPtr = tree->end;
	while (searchPtr != NULL)
	{
		if (searchPtr->byte == FILEENDBYTE)
		{
			while (searchPtr != tree->start)
			{
				if (searchPtr->parent->left == searchPtr)
					err = BitArray_pushBack(result, 0);
				else if (searchPtr->parent->right == searchPtr)
					err = BitArray_pushBack(result, 1);
				else
					err = ERR_TREEINVALID;

				if (err != ERR_NO)
					return err;

				searchPtr = searchPtr->parent;
			}

			length = BitArray_getLength(result);
			for (i = 0; i < length / 2; i++)
			{
				if ((err = BitArray_getBit(result, i, &bit1)) != ERR_NO)
					return err;
				if ((err = BitArray_getBit(result, length - 1 - i, &bit2)) != ERR_NO)
					return err;
				if ((err = BitArray_setBit(result, length - 1 - i, bit1)) != ERR_NO)
					return err;
				if ((err = BitArray_setBit(result, i, bit2)) != ERR_NO)
					return err;
			}

			return ERR_NO;
		}

		searchPtr = searchPtr->prev;
	}

	return ERR_TREEINVALID;
}


err_type FGKTree_getByteCode(const FGKTree tree, const uint8_t byte, BitArray result)
{
	err_type err;
	Node *searchPtr;
	uint64_t length, i;
	uint8_t bit1, bit2;

	if ((tree == NULL) || (result == NULL))
		return ERR_PARAM_INVALID;

	searchPtr = tree->byteNodes[byte];
	if (searchPtr != NULL)
	{
		while (searchPtr != tree->start)
		{
			if (searchPtr->parent->left == searchPtr)
			{
				err = BitArray_pushBack(result, 0);
			}
			else if (searchPtr->parent->right == searchPtr)
			{
				err = BitArray_pushBack(result, 1);
			}
			else
				err = ERR_TREEINVALID;

			if (err != ERR_NO)
				return err;

			searchPtr = searchPtr->parent;
		}

		length = BitArray_getLength(result);
		for (i = 0; i < length / 2; i++)
		{
			if ((err = BitArray_getBit(result, i, &bit1)) != ERR_NO)
				return err;
			if ((err = BitArray_getBit(result, length - 1 - i, &bit2)) != ERR_NO)
				return err;
			if ((err = BitArray_setBit(result, length - 1 - i, bit1)) != ERR_NO)
				return err;
			if ((err = BitArray_setBit(result, i, bit2)) != ERR_NO)
				return err;
		}

		return ERR_NO;
	}

	return ERR_BYTENOTFOUND;
}


err_type FGKTree_update(FGKTree tree, const uint8_t byte)
{
	Node *byteNode, *searchPtr, swap;

	if (tree == NULL)
		return ERR_PARAM_INVALID;

	byteNode = tree->byteNodes[byte];
	if (byteNode != NULL)
	{
		do
		{
			searchPtr = byteNode;
			while (searchPtr->prev->weight <= byteNode->weight)
			{
				if (searchPtr->prev == tree->start)
					break;
				searchPtr = searchPtr->prev;
			}

			if (byteNode->parent != searchPtr)
			{
				if (byteNode->byte < 256)
					tree->byteNodes[byteNode->byte] = searchPtr;
				if (searchPtr->byte < 256)
					tree->byteNodes[searchPtr->byte] = byteNode;

				swap = *searchPtr;

				searchPtr->byte = byteNode->byte;
				searchPtr->left = byteNode->left;
				searchPtr->right = byteNode->right;
				if (searchPtr->left != NULL)
					searchPtr->left->parent = searchPtr;
				if (searchPtr->right != NULL)
					searchPtr->right->parent = searchPtr;

				byteNode->byte = swap.byte;
				byteNode->left = swap.left;
				byteNode->right = swap.right;
				if (byteNode->left != NULL)
					byteNode->left->parent = byteNode;
				if (byteNode->right != NULL)
					byteNode->right->parent = byteNode;

				byteNode = searchPtr;
			}

			byteNode->weight++;
		} 
		while ((byteNode = byteNode->parent) != (tree->start));
		tree->start->weight++;

		return ERR_NO;
	}

	return ERR_BYTENOTFOUND;
}


err_type FGKTree_addByte(FGKTree tree, const uint8_t byte)
{
	Node *additNode = NULL, *symbolNode = NULL;
	err_type result = ERR_NO;

	if (tree == NULL)
		return ERR_PARAM_INVALID;

	if ((additNode = (Node *)malloc(sizeof(Node))) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}
	if ((symbolNode = (Node *)malloc(sizeof(Node))) == NULL)
	{
		result = ERR_NOMEM;
		goto finalization;
	}

	additNode->byte = NOTBYTE;
	additNode->weight = 0;
	additNode->left = symbolNode;
	additNode->right = tree->end;
	additNode->next = symbolNode;
	additNode->prev = tree->end->prev;
	additNode->parent = tree->end->parent;

	symbolNode->byte = byte;
	symbolNode->weight = 0;
	symbolNode->left = NULL;
	symbolNode->right = NULL;
	symbolNode->next = tree->end;
	symbolNode->prev = additNode;
	symbolNode->parent = additNode;

	if (tree->end != tree->start)
	{
		tree->end->prev->next = additNode;
		if (tree->end->parent->left == tree->end)
			tree->end->parent->left = additNode;
		else if (tree->end->parent->right == tree->end)
			tree->end->parent->right = additNode;
		else
		{
			result = ERR_TREEINVALID;
			goto finalization;
		}
	}
	else
		tree->start = additNode;

	tree->end->parent = additNode;
	tree->end->prev = symbolNode;

	tree->byteNodes[byte] = symbolNode;
finalization:
	if (result != ERR_NO)
	{
		if (additNode != NULL)
			free(additNode);
		if (symbolNode != NULL)
			free(symbolNode);
	}

	return result;
}
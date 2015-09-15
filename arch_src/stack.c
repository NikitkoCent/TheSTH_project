#include "stack.h"

Stack * Stack_Create()
{
	Stack *result = NULL;
	
	if ((result = (Stack *)malloc(sizeof(Stack))) != NULL)
	{
		if ((result->arr = (Token *)calloc(STACK_BLOCKSIZE, sizeof(Token))) == NULL)
		{
			free(result);
			result = NULL;
		}
		else
		{
			result->size = STACK_BLOCKSIZE;
			result->len = 0;
		}
	}

	return result;
}


void Stack_Destroy(Stack *stack)
{
	if (stack != NULL)
	{
		if (stack->arr != NULL)
		{
			free(stack->arr);
			stack->arr = NULL;
		}

		stack->len = 0;
		stack->size = 0;
		free(stack);
	}
}


err_type Stack_Push(Stack *stack, const Token *token)
{
	Token *temp = NULL;
	
	if ((stack == NULL) || (token == NULL))
		return ERR_PARAM_INVALID;

	if (stack->len == stack->size)
	{
		temp = stack->arr;
		if ((stack->arr = (Token *)realloc(stack->arr, (stack->size + STACK_BLOCKSIZE)*sizeof(Token))) == NULL)
		{
			stack->arr = temp;
			return ERR_NOMEM;
		}

		stack->size += STACK_BLOCKSIZE;
	}

	stack->arr[stack->len++] = *token;

	return ERR_NO;
}


err_type Stack_Pop(Stack *stack, Token *result)
{
	if ((stack == NULL) || (stack->arr == NULL) || (result == NULL))
		return ERR_PARAM_INVALID;

	if (stack->len == 0)
		return ERR_INDEX_OUT;

	*result = stack->arr[--stack->len];

	return ERR_NO;
}


err_type Stack_Peek(const Stack *stack, Token *result)
{
	if ((stack == NULL) || (stack->arr == NULL) || (result == NULL))
		return ERR_PARAM_INVALID;
	
	if (stack->len == 0)
		return ERR_INDEX_OUT;

	*result = stack->arr[stack->len - 1];

	return ERR_NO;
}


char Stack_IsEmpty(const Stack* stack)
{
	return (stack->len == 0);
}
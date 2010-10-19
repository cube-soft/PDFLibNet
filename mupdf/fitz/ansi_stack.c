#include "ansi_stack.h"

struct LinkedList
{
	void *value;
	LinkedList *Address;
};

struct MyStack
{	
	LinkedList *PreviousNode;
	LinkedList **FirstNode;
	int StackPosition;
};

 void push(MyStack *s, void* obj)
{
	LinkedList *node;
	node = (LinkedList *)malloc(sizeof(LinkedList));
	
	if (s->StackPosition == -1)
	{
		node->value = obj;
		node->Address = NULL;
		s->FirstNode = node;
		s->PreviousNode = node;
		s->StackPosition +=1;
	}
	else
	{
		s->StackPosition += 1;	
		node->value = obj;
		node->Address = s->PreviousNode;
		s->PreviousNode = node;
	}
}

 void* pop(MyStack *s)
{
	void *temp;
	LinkedList *node;
	if (s->StackPosition > -1)
	{
		s->StackPosition -= 1;
		temp = s->PreviousNode->value;
		node = s->PreviousNode;
		if(s->StackPosition > -1)
			s->PreviousNode = s->PreviousNode->Address;
		else
			s->PreviousNode = NULL;
		free(node);
	}
	else
	{
		return NULL;
	}
	
	return temp;
}


 MyStack * newstack()
{
	MyStack *s;
	s=(MyStack *)malloc(sizeof(MyStack));
	s->PreviousNode = NULL;
	s->FirstNode = NULL;
	s->StackPosition = -1;
	return s;
}

 void freestack(MyStack *s)
{
	while(s->StackPosition>-1)
	{
		pop(s);
	}
	free(s);
}
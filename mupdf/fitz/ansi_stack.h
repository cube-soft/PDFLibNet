#pragma once
#include <stdio.h>

typedef struct LinkedList_s LinkedList;
typedef struct MyStack_s MyStack;
struct LinkedList_s
{
	void *value;
	LinkedList *Address;
};

struct MyStack_s
{	
	LinkedList *PreviousNode;
	LinkedList **FirstNode;
	int StackPosition;
};

void push(MyStack *s, void* obj);
void* pop(MyStack *s);
MyStack* newstack();
void freestack(MyStack *s);
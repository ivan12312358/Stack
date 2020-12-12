#include "stack.h"
#include "stack.cpp"

int main(){

	Stack stack;

	constructor(&stack);

	int x = 0;
	while(stack.size > 0)
		x = pop(&stack);

	destructor(&stack);
}
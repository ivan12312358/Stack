#include <stdio.h>
#include <stdlib.h>
enum error {
	stack_ok,
	w_value_cap,
	w_value_size,
	w_type_cap,
	size_lrg_cap,
	w_type_elem
};

typedef struct {
    int* elements;
    int size;
    int capacity;
} Stack;

int constructor(Stack* stack);
void stack_dump(Stack* stack);
int verification(Stack* stack);
int cap_change(Stack* stack, int mode);
int push(int elem, Stack* stack);
int pop(Stack* stack);
int destructor(Stack* stack);
int poison = -666;


int main() {
	Stack stack;

    constructor(&stack);
    
    destructor(&stack);

    return 0;
}

void stack_dump(Stack* stack) {
	printf("stack_dump:\n");
	printf("stack_status:");
	switch(verification(stack)) {
		case(stack_ok):
			printf("Stack_OK\n");
			break;
		case(w_value_cap):
			printf("Stack_BAD:\tInvalid value of capacity\n");
			break;
		case(w_value_size):
			printf("Stack_BAD:\tInvalid value of size\n");
			break;
		case(w_type_cap):
			printf("Stack_BAD:\tWrong type of capacity\n");		
			break;
		case(size_lrg_cap): 
			printf("Stack_BAD:\n\t\tThe number of stack elements more than stack capacity\n");
			break;
		case(w_type_elem):
			printf("Stack_BAD:\n\t\tWrong type of data\n");
			printf("stack.elem[%d] = %d",  stack->size - 1, stack->elements[stack->size - 1]);
			break;
		default:;
	}
	printf("\tSize: %d\n\tCapacity: %d\n", stack->size, stack->capacity);
	if(stack->size != 0) { 
		for (int i = 0; i < stack->capacity; i++) {
			printf("\t\tstack.elem[%d] = %d\n", i, stack->elements[i]);
		}
	}
	
}

int verification(Stack* stack) {
	
	if(stack->capacity <= 0) {
		return w_value_cap;
	} else if(stack->size < 0) {
		return w_value_size;
	} else if(sizeof(stack->capacity) != sizeof(int)) {
		return w_type_cap;
	} else if(stack->size > stack->capacity) {
		return size_lrg_cap;
	} else if(stack->size != 0 && sizeof(stack->elements[stack->size - 1]) != sizeof(int)) {
		return w_type_elem;
	} else {
		return stack_ok;	
	}
	return 0;
}

int constructor(Stack* stack) {
	
	stack->size = 0;
	scanf("%d", &stack->capacity);
	
	
	stack->elements = (int*)calloc(stack->capacity, sizeof(int));
	for(int i = 0; i < stack->capacity; i++) {
		stack->elements[i] = poison; 
	}
		stack_dump(stack);

	int elem;
	while(scanf("%d", &elem) == 1) {
		if(stack->size == stack->capacity) {
			cap_change(stack, 1);
		}
		push(elem, stack);
	}
	if(stack->size < stack->capacity) {
		cap_change(stack, -1);		
	}

	return 0;
}

int cap_change(Stack* stack, int mode) {
	Stack* old_stack = stack;
	if(mode == 1) {
		stack->capacity *= 2;
		stack->elements = (int*)realloc(stack->elements, stack->capacity*sizeof(int));
		for(int i = stack->size; i < stack->capacity; i++) {
			stack->elements[i] = poison;
		}
	} else if(mode == -1) {
		stack->capacity = stack->size;
		stack->elements = (int*)realloc(stack->elements, (stack->capacity)*sizeof(int));
	} else {
		return -1;
	}
	if(old_stack->elements != stack->elements) {
		destructor(old_stack);
	}
	return 0;
}

int push(int elem, Stack* stack) {
//	verification(stack);
	stack->elements[stack->size++] = elem;
	//verification(stack);
	return 0;
}

int pop(Stack* stack) {
	//verification(stack);
	return stack->elements[stack->size - 1];
}

int destructor(Stack* stack) {
	for(int i = 0; i < stack->size; i++) {
		stack->elements[i] = poison;
	}
	stack->size = poison;
	stack->capacity = poison;
	free(stack->elements);
	stack->elements = NULL;
	return 0;
}
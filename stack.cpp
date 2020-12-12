#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#line 6

#define ASSERT_OK(stack)				\
do {							\
	error = verification(stack);			\
							\
	if(error != 0){					\
							\
		printf("Verification failed"		\
			" on %d string\n", __LINE__);	\
							\
		stack_dump(stack, error);		\
	}						\
} while(0);						\

int verification(Stack* stack){	
	
	long long summ = 0;
    for (int i = 1; i <= stack->size; i++)
    	summ += stack->elem[i + canary_size/4]*((canary + i)%7)*i;

	if(stack->capacity <= 0 || stack->capacity >= max_value){
		return w_value_cap;

	} else if(stack->size < 0 || stack->size >= max_value){
		return w_value_size;

	} else if(stack->size >= stack->capacity){
		return size_lrg_cap;

	} else if(stack->chop1 != canary){
		return w_value_chop1;

	} else if(stack->chop2 != canary){
		return w_value_chop2;

	} else if(stack->elem == NULL){
		return null_ptr;

	} else if(((long long*) stack->elem)[0] != canary){
		return w_value_canary1;

	} else if(*((long long*)(stack->elem + stack->capacity + canary_size/2)) != canary){
		return w_value_canary2;

	} else if(summ != stack->hash_summ){
		return w_val_hsh_sm;
	}	

	return stack_ok;
}

void stack_dump(Stack* stack, int error){
	
	printf("\t\tSTACK DUMP\n");
	printf("stack_status:\n");
	
	switch(error) {

		case(stack_ok):
			printf("Stack_OK\n");
			break;

		case(w_value_cap):
			printf("Invalid value of capacity\n");
			break;

		case(w_value_size):
			printf("Invalid value of size\n");
			break;

		case(size_lrg_cap): 
			printf("The number of stack elements more than stack capacity\n");
			break;

		case(w_value_chop1):
			printf("Left canary of struct is spoiled\n");
			break;

		case(w_value_chop2):
			printf("Right canary of struct is spoiled\n");
			break;

		case(null_ptr):
			printf("Pointer to the elements equal to zero\n");
			break;

		case(w_value_canary1):
			printf("Left canary of elements is spoiled\n");
			break;

		case(w_value_canary2):
			printf("Right canary of elements is spoiled\n");
			break;

		case(w_val_hsh_sm):
			printf("Invalid value of hash summ\n");
			break;
			
		default:;
	}
	
	printf("Size: %d\nCapacity: %d\n", stack->size, stack->capacity);
	printf("Hash summ: %lld\n", stack->hash_summ);
	
	if(stack->elem != NULL){ 

		for (int i = 0; i < stack->capacity + canary_size; i++){
			printf("\tstack.elem[%d] = %d\n", i, stack->elem[i]);
		}
	}	
}

void constructor(Stack* stack){
	
	int error = 0;
	stack->chop1 = canary;
	stack->size = 0;
	stack->hash_summ = 0;
	stack->chop2 = canary;

	printf("Enter size of stack:\t");

	if(scanf("%d", &stack->capacity) == 1 && stack->capacity > 0){
		
		int* tmp = (int*)calloc(stack->capacity + canary_size, sizeof(int));
		
		if(tmp != NULL)
			stack->elem = tmp;
	} else {
		printf("Wrong value of capacity\n");
		exit(w_value_cap);
	}
	
	memcpy(stack->elem, &canary, sizeof(long long));
	memcpy(stack->elem + stack->capacity + canary_size/2, &canary, sizeof(long long));

	for(int i = 2; i < stack->capacity + canary_size/2; i++)
		stack->elem[i] = poison;

	ASSERT_OK(stack)

	int elem = 0;
	printf("Enter elements of stack:\n");

	while(scanf("%d", &elem) == 1) {

		if(stack->capacity == stack->size + canary_size/4)
			cap_change(stack);

		push(stack, elem);
	}

	cap_change(stack);
}

void cap_change(Stack* stack) {
	
	int error;
	ASSERT_OK(stack)

	int* tmp = NULL;

	if(stack->capacity == stack->size + 1){
		
		stack->capacity += decrement;

		tmp = (int*)realloc(stack->elem, (stack->capacity + canary_size)*sizeof(int));

	} else if(stack->size < stack->capacity){
		
		stack->capacity = stack->size + 1;
		
		tmp = (int*)realloc(stack->elem, (stack->capacity + canary_size)*sizeof(int));		
	
	} else {
		return;
	}

	if(tmp != NULL){

		stack->elem = tmp;
		memcpy(stack->elem + stack->capacity + canary_size/2, &canary, sizeof(long long));
		
		for(int i = stack->size + canary_size/2; i < stack->capacity + canary_size/2; i++)
			stack->elem[i] = poison;

	} else {
		printf("Failed to reallocate\n");
	}
}

void push(Stack* stack, int elem){
	
	int error = 0;
	ASSERT_OK(stack)
	
	stack->elem[stack->size + canary_size/2] = elem;
	stack->size++;

	stack->hash_summ += elem*((canary + stack->size)%7)*stack->size;
	
	ASSERT_OK(stack)  
}

int pop(Stack* stack){
	
	int error = 0;
	
	ASSERT_OK(stack)
	
	int x = stack->elem[stack->size + canary_size/4];

	stack->hash_summ -= x*((canary + stack->size)%7)*(stack->size);

	stack->size--;
	
	ASSERT_OK(stack)

	stack->elem[stack->size + canary_size/2] = poison;

	return x;
}

void destructor(Stack* stack){
	
	int error = 0;
	ASSERT_OK(stack)
	
	for(int i = 0; i < stack->capacity + canary_size; i++)
		stack->elem[i] = poison;

	stack->size = poison;
	stack->capacity = poison;
	
	free(stack->elem);
}

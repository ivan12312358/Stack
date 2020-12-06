#include <stdio.h>
#include <stdlib.h>
#line 4

#define ASSERT_OK(stack, str_num)			\
	error = verification(stack);			\
	if(error != 0){					\
		stack_dump(stack, error, str_num);	\
	}						\

int verification(Stack* stack){	
	long long summ = 0;
    for (int i = 1; i <= stack->size; i++) {
    	summ += stack->elem[i]*((canary + i)%7)*i;
    }

	if(stack->capacity <= 0 || stack->capacity >= max_value) {
		return w_value_cap;
	} else if(stack->size < 0 || stack->size >= max_value) {
		return w_value_size;
	} else if(stack->size >= stack->capacity) {
		return size_lrg_cap;
	} else if(stack->chop1 != canary) {
		return w_value_chop1;
	} else if(stack->chop2 != canary) {
		return w_value_chop2;
	} else if(stack->elem == NULL){
		return null_ptr;
	} else if(stack->elem[0] != canary) {
		return w_value_canary1;
	} else if(stack->elem[stack->capacity + 1] != canary){
		return w_value_canary2;
	} else if(summ != stack->hash_summ)
		return w_val_hsh_sm;
	return stack_ok;
}

void stack_dump(Stack* stack, int error, int str_num){
	printf("\t\tSTACK DUMP\n");
	printf("stack_status:\n");
	printf("Verification failed on %d string\n", str_num);
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
	printf("Size: %d\nCapacity: %d\nHash summ: %lld\n", stack->size, stack->capacity, stack->hash_summ);
	
	if(stack->size != 0 && stack->elem != NULL) { 
		for (int i = 0; i < stack->capacity + 2; i++) {
			printf("\tstack.elem[%d] = %lld\n", i, stack->elem[i]);
		}
	}	
}

void constructor(Stack* stack){
	stack->chop1 = canary;
	stack->size = 0;
	stack->hash_summ = 0;
	stack->chop2 = canary;

	printf("Enter size of stack:\t");

	if(scanf("%d", &stack->capacity) == 1);
		stack->elem = (long long int*)calloc(stack->capacity + 2, sizeof(long long int));{
	}
	
	stack->elem[0] = canary;
	stack->elem[stack->capacity + 1] = canary;

	for(int i = 1; i < stack->capacity + 1; i++){
		stack->elem[i] = poison;
	}

	long long elem = 0;
	printf("Enter elements of stack:\n");

	while(scanf("%lld", &elem) == 1){
		if(stack->capacity == stack->size + 1){
			cap_change(stack);
		}
		push(stack, elem);
	}
	cap_change(stack);
}

void cap_change(Stack* stack){
	int error;
	ASSERT_OK(stack, __LINE__)
	long long* tmp = NULL;

	if(stack->capacity == stack->size + 1){
		stack->capacity += 5;

		tmp = (long long*)realloc(stack->elem, (stack->capacity + 2)*sizeof(long long));

	} else if(stack->size < stack->capacity){
		stack->capacity = stack->size + 1;
		
		tmp = (long long*)realloc(stack->elem, (stack->capacity + 2)*sizeof(long long));		
	} else {
		return;
	}

	if(tmp != NULL){
		stack->elem = tmp;
		stack->elem[stack->capacity + 1] = canary;
		for(int i = stack->size + 1; i < stack->capacity + 1; i++){
			stack->elem[i] = poison;
		}
	} else {
		printf("Failed to reallocate\n");
	}
}

void push(Stack* stack, long long elem){
	int error = 0;
	ASSERT_OK(stack, __LINE__)
	
	stack->elem[++stack->size] = elem;
	
	stack->hash_summ += elem*((canary + stack->size)%7)*stack->size;
	
	ASSERT_OK(stack, __LINE__)  
}

long long pop(Stack* stack){
	int error = 0;
	ASSERT_OK(stack, __LINE__)
	
	long long x = stack->elem[stack->size--];
	
	stack->hash_summ -= x*((canary + stack->size + 1)%7)*(stack->size + 1);
	
	stack->elem[stack->size + 1] = poison;
	
	ASSERT_OK(stack, __LINE__)
	
	return x;
}

void destructor(Stack* stack){
	int error = 0;
	ASSERT_OK(stack, __LINE__)
	
	for(int i = 0; i < stack->size + 1; i++) {
		stack->elem[i] = poison;
	}

	stack->size = poison;
	stack->capacity = poison;
	
	free(stack->elem);
}

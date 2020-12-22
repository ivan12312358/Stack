#pragma once
#include "stack.h"
#line 4

const int poison = -666;
const int max_value = 2147483637;
const long long canary = 0x2237EA177;
const int canary_size = 4; 
const int decrement = 5;

#define ASSERT_OK(stack)				\
do {							\
	error = verification(stack);			\
							\
	if(error != 0){					\
							\
		printf("Verification failed"		\
			" on %d string\n", __LINE__);	\
							\
		stack_dump(stack);			\
	}						\
} while(0);						\


#define case_of_switch(enum_const) 			\
do {							\
							\
	case enum_const:				\
	return #enum_const;				\
							\
} while(0);						\


const char* test_error(int error){

	switch(error){

		case 0: return "OK";

		case_of_switch(w_value_cap)

		case_of_switch(w_value_size)

		case_of_switch(size_lrg_cap)

		case_of_switch(w_value_chop1)

		case_of_switch(w_value_chop2)

		case_of_switch(null_ptr)

		case_of_switch(w_value_canary1)

		case_of_switch(w_value_canary2)

		case_of_switch(w_val_hsh_sm)

		default: return "Unexpected case";
	}
}

int verification(Stack* stack){	
	
	long long summ = 0;

    for (int i = 1; i <= stack->size; i++){

    	summ += stack->elem[i + canary_size/4]*((canary + i)%7)*i;
    }

	if(stack->capacity <= 0 || stack->capacity >= max_value){

		printf("%s\n", test_error(w_value_cap));
		return w_value_cap;

	} else if(stack->size < 0 || stack->size >= max_value){
		
		printf("%s\n", test_error(w_value_size));
		return w_value_size;

	} else if(stack->size >= stack->capacity){

		printf("%s\n", test_error(size_lrg_cap));
		return size_lrg_cap;

	} else if(stack->chop1 != canary){

		printf("%s\n", test_error(w_value_chop1));
		return w_value_chop1;

	} else if(stack->chop2 != canary){

		printf("%s\n", test_error(w_value_chop2));
		return w_value_chop2;

	} else if(stack->elem == NULL){

		printf("%s\n", test_error(null_ptr));
		return null_ptr;

	} else if(((long long*) stack->elem)[0] != canary){
		
		printf("%s\n", test_error(w_value_canary1));
		return w_value_canary1;

	} else if(*((long long*)(stack->elem + stack->capacity + canary_size/2)) != canary){

		printf("%s\n", test_error(w_value_canary2));
		return w_value_canary2;

	} else if(summ != stack->hash_summ){

		printf("%s\n", test_error(w_val_hsh_sm));
		return w_val_hsh_sm;
	}	

	return stack_ok;
}

void stack_dump(Stack* stack){

	FILE* file = fopen("STACK_DUMP.txt", "a");
	
	fprintf(file, "\t\tSTACK DUMP\n");	
	
	fprintf(file, "Size: %d\nCapacity: %d\n", stack->size, stack->capacity);
	
	fprintf(file, "Hash summ: %lld\n", stack->hash_summ);
	
	fprintf(file, "Poison: %d\n", poison);

	fprintf(file, "Canary: %lld\n", canary);

	fprintf(file, "Stack Pointer: %p\n", stack);

	fprintf(file, "Stack elements:\n\n");

	if(stack->elem != NULL){ 

		fprintf(file, "\tstack.canary = %lld\n", *((long long*)stack->elem));

		for (int i = canary_size/2; i < stack->capacity + canary_size/2; i++){

			fprintf(file, "\tstack.elem[%d] = %d\n", i - 2, stack->elem[i]);
		}

		fprintf(file, "\tstack.canary = %lld\n", *((long long*)(stack->elem + stack->capacity + canary_size/2)));
	}	

	fclose(file);
}

void time(const char* mode){

	FILE* file = fopen("STACK_DUMP.txt", mode);

	time_t rawtime;
	
	struct tm * timeinfo;
	
	time ( &rawtime );
	
	timeinfo = localtime ( &rawtime );
	
	fprintf (file, "\n%s\n", asctime (timeinfo) );

	if(mode == "w"){

		fprintf(file, "Stack was created.\n\n");
	
	} else if(mode == "a") {

		fprintf(file, "Stack was deleted.\n");	
	}

	fclose(file);
}

void constructor(Stack* stack){

	int error = 0;
	stack->chop1 = canary;
	stack->size = 0;
	stack->hash_summ = 0;
	stack->chop2 = canary;

	printf("Enter size of stack:\t");

	if(scanf("%d", &stack->capacity) == 1 && stack->capacity > 0){
		
		stack->elem = (int*)calloc(stack->capacity + canary_size, sizeof(int));
	}

	time("w");

	memcpy(stack->elem, &canary, sizeof(long long));
	memcpy(stack->elem + stack->capacity + canary_size/2, &canary, sizeof(long long));

	for(int i = 2; i < stack->capacity + canary_size/2; i++){
		
		stack->elem[i] = poison;
	}

	ASSERT_OK(stack)

	int elem = 0;
	printf("Enter elements of stack:\n");

	while(scanf("%d", &elem) == 1){

		if(stack->capacity == stack->size + canary_size/4){

			cap_change(stack);
		}
			
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

	time("a");
}

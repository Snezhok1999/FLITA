#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char **stack;
static int stack_size = 0;

void stack_print(){
    if(stack_size == 0){
        printf("stack is empty\n");
    }
    for(int i=0; i<stack_size; i++){
        printf("%d: '%s'\n", i+1, stack[i]);
    }
}

void stack_push(char *n){
    char **newstack = calloc(++stack_size, sizeof(char*));

    newstack[0] = n;
    for(int i=0; i<stack_size; i++){
        newstack[i+1] = stack[i];
    }
    free(stack);
    stack = newstack;
}

char *stack_pop(){
    if (stack_size > 0) {
        char *return_value = stack[0];

        char **newstack = calloc(--stack_size, sizeof(char*));

        for(int i=0; i<stack_size; i++){
            newstack[i] = stack[i+1];
        }
        free(stack);
        stack = newstack;

        return return_value;
    } else {
        printf("stack is empty\n");
        return NULL;
    }
}


void main(){
    stack = calloc(1, sizeof(char*));

    int menu;

    while(1){
        printf("1: push; 2: pop; 3: print;\n> ");
        scanf("%d", &menu);

        if(menu == 1){
            char *element = calloc(32, sizeof(char));
            printf("enter element > ");
            scanf("%s", element);
            stack_push(element);
        } else if(menu == 2){
            char *element;

            if((element = stack_pop()) != NULL){
                printf("'%s'\n", element);
            }
            free(element);
        } else if(menu == 3){
            stack_print();
        } else {
            printf("unknown command\n");
        }
        printf("\n");
    }
}

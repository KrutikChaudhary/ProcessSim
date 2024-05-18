#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char type[10];
    int ticks;
}primitive;

typedef struct{
    int index;
    int remainingIterations;
}loopReference;

typedef struct {
    loopReference items[100];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int size(Stack *s){
    return s->top+1;
}
int isEmpty(Stack *s) {
    return s->top == -1;
}
int isFull(Stack *s) {
    return s->top == 100 - 1;
}
void push(Stack *s, loopReference item) {
    if (isFull(s)) {
        printf("Stack is full. Cannot push item.\n");
        return;
    }
    s->items[++s->top] = item;
}
loopReference pop(Stack *s) {
    if (isEmpty(s)) {
        printf("Stack is empty. Cannot pop item.\n");
        exit(EXIT_FAILURE);
    }
    return s->items[s->top--];
}

int main() {
    char programName[50];
    int lines;
    static int time = 0;
    scanf("%s %d", programName, &lines);

    //printf("%s %d", programName, lines);

    primitive primitives[lines];

    for (int i = 0; i<lines; i++){
        scanf("%s", primitives[i].type);
        //printf("%s",primitives[i].type);
        if(strcmp(primitives[i].type, "END")==0 || strcmp(primitives[i].type, "HALT")==0 ){
            primitives[i].ticks=0;
        } else{
            scanf("%d", &primitives[i].ticks);
        }
    }

//    for (int i = 0; i<lines; i++){
//        printf("%s %d yoo\n", primitives[i].type, primitives[i].ticks);
//    }
//    printf("*******\n");

    int doopCount = 0,doopTime =0 , blockCount = 0, blockTime = 0;
    int i=0;
    Stack s;
    initStack(&s);
    while(i<lines){
        if(strcmp(primitives[i].type, "DOOP")==0){
            printf("%05d: DOOP\n", time);
            time += primitives[i].ticks;
            doopTime +=  primitives[i].ticks;
            doopCount++;
        } else if(strcmp(primitives[i].type, "BLOCK")==0){
            printf("%05d: BLOCK\n", time);
            time += primitives[i].ticks;
            blockTime +=  primitives[i].ticks;
            blockCount++;
        } else if(strcmp(primitives[i].type, "HALT")==0){
            printf("%05d: HALT\n", time);
        } else if(strcmp(primitives[i].type, "LOOP")==0){
            loopReference reference;
            reference.index = i;
            reference.remainingIterations=primitives[i].ticks;
            push(&s,reference);
            printf("Size %d\n",size(&s));
            int j = s.items[s.top].index + 1;
            while(!isEmpty(&s)){
                if(strcmp(primitives[j].type, "DOOP")==0){
                    printf("%05d: DOOP\n", time);
                    time += primitives[j].ticks;
                    doopTime +=  primitives[j].ticks;
                    doopCount++;
                    j++;

                } else if(strcmp(primitives[j].type, "BLOCK")==0){
                    printf("%05d: BLOCK\n", time);
                    time += primitives[j].ticks;
                    blockTime +=  primitives[j].ticks;
                    blockCount++;
                    j++;
                } else if(strcmp(primitives[j].type, "END")==0){
                    s.items[s.top].remainingIterations--;
                    if(s.items[s.top].remainingIterations==0){
                        pop(&s);
                       // printf("%dHJKHFWRK", isEmpty(&s));
                        i=j;
//                        printf("ohoy %d", i);
                        continue;
                    } else {
                        j=s.items[s.top].index + 1;
                    }
                }
            }
        }
        i++;
    }

    printf("\n");

    printf("DOOP count : %d\n"
           "DOOP time  : %d\n"
           "BLOCK count: %d\n"
           "BLOCK time : %d\n", doopCount, doopTime, blockCount, blockTime);
    return 0;
}

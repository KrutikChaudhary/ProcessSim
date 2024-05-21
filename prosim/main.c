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
    int currDeviation;
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
            reference.currDeviation=1;
            push(&s,reference);
            //printf("Size %d\n",size(&s));
            while(!isEmpty(&s)){
                int j = s.items[s.top].currDeviation+s.items[s.top].index;
                //printf("%d\n",j);
                if(strcmp(primitives[j].type, "DOOP")==0){
                    //printf("yoouuuuuu");
                    printf("%05d: DOOP\n", time);
                    //printf("yoRajjjouuuuuu");
                    time += primitives[j].ticks;
                    //printf("yoRajjjouuuuuu");
                    doopTime +=  primitives[j].ticks;
                    doopCount++;
                    s.items[s.top].currDeviation++;
//                    printf("yoo");

                } else if(strcmp(primitives[j].type, "BLOCK")==0){
                    printf("%05d: BLOCK\n", time);
//                    printf("%d yuy\n",j);
                    time += primitives[j].ticks;
                    blockTime +=  primitives[j].ticks;
                    blockCount++;
                    s.items[s.top].currDeviation++;
                } else if(strcmp(primitives[j].type, "END")==0){
                    printf("ahoyyy\n");
                    s.items[s.top].remainingIterations--;
                    if(s.items[s.top].remainingIterations==0){
                        if(size(&s)>1){
//                            printf("UTL\n");
                            //for(int k=size(&s); k>0; k--){

                           j++;
                           s.items[s.top-1].currDeviation=j;
                            //}


                        }
//                        printf("true\n");
                        pop(&s);
                       // printf("%dHJKHFWRK", isEmpty(&s));
                        i=j;
//                        printf("ohoy %d", i);
                        continue;
                    } else {//printf("\n*****%d y\n",j);

                        s.items[s.top].currDeviation=1;
                    }
                }
                else if(strcmp(primitives[j].type, "LOOP")==0){
                    loopReference temp;
                    temp.index = j;
                    temp.remainingIterations=primitives[j].ticks;
                    temp.currDeviation=1;
                    push(&s,temp);
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

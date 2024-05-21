/*
* File: main.c
* Author: Krutik Chaudhary
* Date: May 21, 2024
* Purpose: Assignment 1 Submission
* Simple Simulator to simulate the program execution
*/

#include <stdio.h>
#include <string.h>

//struct to store each primitive
typedef struct{
    char type[10]; // type or name of primitive
    int ticks; // duration of clock ticks
}primitive;

//struct to store information of a loop primitive
typedef struct{
    int index; //start index of loop
    int remainingIterations; //remaining iterations of the loop (initialized based on user input)

    //current reference to keep track of the current statement we are processing inside loop
    // (incremented after processing each statement of the loop.
    // if end of loop is reached and iterations are still remaining, reset to start of loop again i.e index position of loop)
    int currReference;
}loopReference;

//struct to perform stack and process normal and nested loops
typedef struct {
    loopReference items[100];//declare an array of 100, to keep track of which loop to process at which time
    int top;//keep track of top of stack
} Stack;

//this method initializes stack top to -1 (Empty initially)
void initializeStack(Stack *s) {
    s->top = -1;
}

//this method returns size of the stack
int size(Stack *s){
    return s->top+1;
}

//this method returns 1 if stack is empty. 0 otherwise
int isEmpty(Stack *s) {
    return s->top == -1;
}

//this method returns 1 if stack is full. 0 otherwise
int isFull(Stack *s) {
    return s->top == 100 - 1;
}

//this method perform push operation on stack
void push(Stack *s, loopReference item) {
    if (isFull(s)) {
        printf("Stack is full.\n");
        return;
    } else{
        s->top++;
        s->items[s->top] = item;
    }
}

//this method performs pop operation on stack and decrements the top
loopReference pop(Stack *s) {
    return s->items[s->top--];
}

//main program
int main() {
    char programName[50]; //char array/string to store the program name
    int lines; //to store the number of statements of the upcoming input

    //input the program name and number of lines in upcoming/statements input
    scanf("%s %d", programName, &lines);

    //initialize primitive array to store all the statements of the input
    primitive primitives[lines];

    //take input of the input program primitives
    for (int i = 0; i<lines; i++){
        //scan name/type
        scanf("%s", primitives[i].type);

        //if the primitive name/type is END or HALT initialize the clock ticks to 0 (as they don't hold any time consumption to execute)
        if(strcmp(primitives[i].type, "END")==0 || strcmp(primitives[i].type, "HALT")==0 ){
            primitives[i].ticks=0;
        } else{
            //else store the clock ticks of each statement, if type is LOOP, it will store number of iterations instead of clock ticks
            scanf("%d", &primitives[i].ticks);
        }
    }

    //initialize clock time to 0
    static int time = 0;

    //initialize doop, block: time and count to zero, to count the output stats
    int doopCount = 0,doopTime =0 , blockCount = 0, blockTime = 0;

    //initialize counter to keep track of position in primitive array
    int i=0;

    //declare and initialize the stack for loops
    Stack s;
    initializeStack(&s);

    //while i is less than number of statements
    while(i<lines){
        //if the statement type is doop
        if(strcmp(primitives[i].type, "DOOP")==0){
            //output the primitive processed and clock time
            printf("%05d: DOOP\n", time);

            //add primitive's time to clock and doop time.
            time += primitives[i].ticks;
            doopTime +=  primitives[i].ticks;

            //increment the doop counter
            doopCount++;

        } else if(strcmp(primitives[i].type, "BLOCK")==0){  //if the statement type is block
            //output the primitive processed and clock time
            printf("%05d: BLOCK\n", time);

            //add primitive's time to clock and block time.
            time += primitives[i].ticks;
            blockTime +=  primitives[i].ticks;

            //increment the block counter
            blockCount++;

        } else if(strcmp(primitives[i].type, "HALT")==0){//if the statement type is halt means end of input program
            //output the primitive processed and final clock time
            printf("%05d: HALT\n", time);

        } else if(strcmp(primitives[i].type, "LOOP")==0){//if the statement type is loop
            //initialize a loop reference
            loopReference reference;

            //set the index as the loop statement position in the primitive array
            reference.index = i;
            //set the remaining iterations, which we stored in ticks for loop case
            reference.remainingIterations=primitives[i].ticks;

            //set current reference from the index as 1 i.e shift form index of loop,
            // so the position of statement being executed inside loop will be at index + currReference
            reference.currReference=1;

            //push loop on stack
            push(&s,reference);

            //while stack is not empty
            while(!isEmpty(&s)){
                //get and store current position of statement to execute inside loop i.e loop statement index + currReference
                int j = s.items[s.top].currReference + s.items[s.top].index;

                //if the statement type is doop
                if(strcmp(primitives[j].type, "DOOP")==0){
                    //output the primitive processed and clock time
                    printf("%05d: DOOP\n", time);

                    //add primitive's time to clock and doop time.
                    time += primitives[j].ticks;
                    doopTime +=  primitives[j].ticks;

                    //increment the doop counter
                    doopCount++;

                    //increment the current reference to execute the next statement inside loop
                    s.items[s.top].currReference++;

                } else if(strcmp(primitives[j].type, "BLOCK")==0){ //if the statement type is block
                    //output the primitive processed and clock time
                    printf("%05d: BLOCK\n", time);

                    //add primitive's time to clock and block time.
                    time += primitives[j].ticks;
                    blockTime +=  primitives[j].ticks;

                    //increment the block counter
                    blockCount++;

                    //increment the current reference to execute the next statement inside loop
                    s.items[s.top].currReference++;

                } else if(strcmp(primitives[j].type, "END")==0){//if the statement type is end
                    //decrement the iterations counter
                    s.items[s.top].remainingIterations--;

                    //if all iterations are completed
                    if(s.items[s.top].remainingIterations==0){

                        //if stack size is already more than one, i.e nested loops
                        if(size(&s)>1){

                            //if the next statement type is also end, which means two loops are ending,
                            // assign first(outer) loop the current reference of the end of second loop so any statement is not missed
                            if(strcmp(primitives[j+1].type, "END")==0){
                                s.items[s.top-1].currReference=j;
                            }
                            else{//if the second loop is ended and first(outer) loop still have statements to execute,
                                //assign first(outer) loop the current reference of position after the second loop,
                                // so first loop can execute the remaining statements
                                s.items[s.top-1].currReference= j + 1;
                            }

                        }
                        //pop the finished loop from stack
                        pop(&s);

                        //reset i to position after loop
                        i=j;

                        //continue until stack is empty
                        continue;

                    } else {
                        //iterations are remaining, go back to starting position of loop, to proceed with next iteration
                        s.items[s.top].currReference=1;
                    }
                }
                else if(strcmp(primitives[j].type, "LOOP")==0){//if the statement type is loop

                    //initialize a loop reference
                    loopReference temp;
                    //set the index as the loop statement position in the primitive array
                    temp.index = j;
                    //set the remaining iterations, which we stored in ticks for loop case
                    temp.remainingIterations=primitives[j].ticks;
                    //set current reference from the index as 1
                    temp.currReference=1;

                    //push loop on stack
                    push(&s,temp);
                }
            }
        }
        //increment index
        i++;
    }

    //final stats output
    printf("\n");
    printf("DOOP count : %d\n"
           "DOOP time  : %d\n"
           "BLOCK count: %d\n"
           "BLOCK time : %d\n", doopCount, doopTime, blockCount, blockTime);

    //end main return zero
    return 0;
}

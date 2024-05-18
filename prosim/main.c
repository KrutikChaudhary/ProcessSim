#include <stdio.h>
#include <string.h>

typedef struct{
    char type[10];
    int ticks;
}primitive;

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
    for(int i=0; i<lines; i++){
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
        }
    }

    printf("\n");

    printf("DOOP count : %d\n"
           "DOOP time  : %d\n"
           "BLOCK count: %d\n"
           "BLOCK time : %d\n", doopCount, doopTime, blockCount, blockTime);
    return 0;
}

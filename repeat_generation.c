#include "generate.h"

int WANT  = 100;

int main(){
    MODE = 1;
    int create = 0;
    int flag;
    while(create < WANT){
        flag = generate_sudoku();
        if (flag == 1)
            create ++;
        printf("%d\n", time(0));
    }
}
#include "generate.h"

int WANT  = 10;

int main(){
    MODE = 2;
    int create = 0;
    int flag;
    while(create < WANT){
        flag = generate_sudoku();
        if (flag == 1)
            create ++;
        printf("%d\n", time(0));
    }
}
#include "generate.h"

int WANT  = 500;

int main(){
    MODE = 2;
    int create = 0;
    int count =0;
    int flag;
    while(create < WANT){
        srand((unsigned int)time(NULL)+count);
        flag = generate_sudoku();
        if (flag == 1)
            create ++;
        printf("%d\n", time(0));
        count ++;
    }
}
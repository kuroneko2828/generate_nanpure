#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"
#include <time.h>
int MODE = 2; //1:易しい、2:難しい
int pos_g[9][9][9];
int GIVEN_NUM = 10;
int REPEAT_COUNT = 5;
int SYMMETRY = 1; //1:点対称
int ERROR_COUNT = 5;

//マスが埋まるたびに関連する候補の削除を行う
void delete_pos_g(int i,int j){
    int k,l,x,y;
    int N=num[i][j];
    //その行、列での候補削除
    for(k=0;k<9;k++){
        pos_g[i][k][N-1]=0;
        pos_g[k][j][N-1]=0;
    }
    //そのブロックでの候補削除
    x=ul(i,j,'i');
    y=ul(i,j,'j');
    for(k=x;k<x+3;k++){
        for(l=y;l<y+3;l++){
            pos_g[k][l][N-1]=0;
        }
    }
    //自分自身の候補削除
    for(k=0;k<9;k++)
    pos_g[i][j][k]=0;
}

void generate_1(){
    for (int i = 0; i <  9; i++){
        for (int j = 0; j < 9; j++){
            for (int k = 0; k < 9; k++){
                pos_g[i][j][k] = 1;
            }
        }
    }
    int n = 0;
    int r1;
    int r2;
    int r3;
    while(1){
        while(1){
            r1 = rand()%9;
            r2 = rand()%9;
            r3 = rand()%9;
            if (pos_g[r1][r2][r3] == 1){
                break;
            }
        }
        num[r1][r2] = r3+1;
        delete_pos_g(r1,r2);
        n ++;
        r1=8-r1;
        r2=8-r2;
        int t = 0;
        for (int k = 0; k < 9; k ++){
            t += pos_g[r1][r2][k];
        }
        if(t == 0){
            continue;
        }
        //printf("OK\n");
        while(1){
            r3 = rand()%9;
            if (pos_g[r1][r2][r3] == 1){
                break;
            }
        }
        num[r1][r2] = r3+1;
        delete_pos_g(r1,r2);
        n++;

        if (n >= GIVEN_NUM){
            break;
        }
    }
}

int generate_2(){
    int r1;
    int r2;
    int r3;
    int r_f;
    r_f = 0;
    for(int i = 0; i < 3; i ++){
        //printf("%d\n", i);
        r1 = rand()%9;
        r2 = rand()%9;
        if (num[r1][r2] == 0){
            for(int j = 0; j < 10; j ++){
                r3 = rand()%9;
                if (pos_g[r1][r2][r3] == 1){
                    r_f = 1;
                    break;
                }
            }
        }
        if (r_f == 1)
            break;
        //printf("%d\n", r1);
    }
    if (r_f == 0)
        return 0;
    num[r1][r2] = r3+1;
    delete_pos_g(r1,r2);
    r1=8-r1;
    r2=8-r2;
    int t = 0;
    for (int k = 0; k < 9; k ++){
        t += pos_g[r1][r2][k];
    }
    if(t == 0 && (r1!=4 || r2!=4)){
        if (SYMMETRY)
            return 0;
        else
            return 1;
    }else if (t==0){
        return 1;
    }
    //printf("OK\n");
    while(1){
        //printf("-%d\n", r3);
        r3 = rand()%9;
        if (pos_g[r1][r2][r3] == 1){
            break;
        }
    }
    num[r1][r2] = r3+1;
    delete_pos_g(r1,r2);
    return 1;
}

void array_copy_9x9(int tar[9][9], int src[9][9]){
    for (int i = 0; i < 9; i ++){
        for (int j = 0; j < 9; j ++){
            tar[i][j] = src[i][j];
        }
    }
}

void array_copy_9x9x9(int tar[9][9][9], int src[9][9][9]){
    for (int i = 0; i < 9; i ++){
        for (int j = 0; j < 9; j ++){
            for (int k = 0; k < 9; k ++){
                tar[i][j][k] = src[i][j][k];
            }
        }
    }
}

int generate(){
    int tmp2[9][9];
    //int tmp2_2[9][9];
    int tmp2_2[9][9];
    int tmp3[9][9][9];
    int flag;
    int ok_f = 0;

    array_copy_9x9(tmp2, num);
    array_copy_9x9x9(tmp3, pos_g);
    //array_copy_9x9(now, num);
    for (int i = 0; i < REPEAT_COUNT; i ++){
        flag = generate_2();
        if (flag == 0){
            array_copy_9x9(num, tmp2);
            array_copy_9x9x9(pos_g, tmp3);
            continue;
        }
        array_copy_9x9(tmp2_2, num);
        flag = answer();
        if (flag == -2)
            return -2;
        if (flag == MODE){
            //printf("ok");
            array_copy_9x9(num, tmp2_2);
            return 1;
        }else if((MODE == 1 && flag == 2) || flag == 0){
            ok_f = 1;
            array_copy_9x9(num, tmp2_2);
            return 0;
        }else{
            array_copy_9x9(num, tmp2);
            array_copy_9x9x9(pos_g, tmp3);
        }
    }
    return -1;
}

int g(){
    int flag;
    int tmp2[9][9];
    int tmp3[9][9][9];
    array_copy_9x9(tmp2,num);
    array_copy_9x9x9(tmp3, pos_g);
    flag = generate();
    if (flag == -2)
        return -2;
    if (flag == 1)
        return 1;
    if (flag == -1)
        return -1;
    for (int i = 0; i <ERROR_COUNT; i ++){
        flag = g();
        if (flag == -2)
            return -2;
        if (flag != -1)
            break;
    }
    if (flag == -1){
        array_copy_9x9(num, tmp2);
        array_copy_9x9x9(pos_g, tmp3);
        return -1;
    }
    if (flag == 1)
        return 1;
}

void append_data(){
    FILE *fp;
    char json_str[10000][500];
    char str[500];
    char new_sudoku[500] = "";
    int count = 0;
    char tmp[2];
    char id[6];
    if (MODE == 1)
        fp = fopen("json/nanpure_easy.json", "r");
    else if (MODE == 2)
        fp = fopen("json/nanpure_hard.json", "r");
    while(fgets(str, 500, fp) != NULL) {
		strcpy(json_str[count], str);
        count ++;
	}
    //printf("%d\n", count);
    //printf("%s", json_str[count-1]);
    fclose(fp);
    if (count > 2){
        json_str[count-2][strlen(json_str[count-2])-1] = '\0';
        //printf("%s", json_str[count-2]);
        strcat(json_str[count-2], ",\n");
    }
    sprintf(id, "%d", count-1);
    strcat(new_sudoku, "{\"id\":\"");
    strcat(new_sudoku, id);
    strcat(new_sudoku, "\",\"mondai\":[");
    for (int i = 0; i < 9; i ++){
        strcat(new_sudoku, "[");
        for (int j = 0; j < 9; j ++){
            tmp[0] = (char)(num[i][j]+48);
            strcat(new_sudoku, tmp);
            if (j != 8){
                strcat(new_sudoku, ",");
            }
        }
        strcat(new_sudoku, "]");
        if (i != 8){
            strcat(new_sudoku, ",");
        }
    }
    strcat(new_sudoku, "],\"kaitou\":[");
    answer();
    for (int i = 0; i < 9; i ++){
        strcat(new_sudoku, "[");
        for (int j = 0; j < 9; j ++){
            tmp[0] = (char)(num[i][j]+48);
            strcat(new_sudoku, tmp);
            if (j != 8){
                strcat(new_sudoku, ",");
            }
        }
        strcat(new_sudoku, "]");
        if (i != 8){
            strcat(new_sudoku, ",");
        }
    }
    strcat(new_sudoku, "]}\n");
    strcpy(json_str[count-1], new_sudoku);
    count ++;
    if (MODE == 1)
        fp = fopen("json/nanpure_easy.json", "w");
    else if (MODE == 2)
        fp = fopen("json/nanpure_hard.json", "w");
    for(int i = 0; i < count; i ++){
        fprintf(fp, "%s", json_str[i]);
    }
    fprintf(fp, "]\n");
    fclose(fp);
}

int main(){
    int flag;
    int tmp2[9][9];
    int tmp3[9][9][9];
    FILE *fp;
    start_time = time(NULL);
    TIMEOUT = 10;
    if (MODE == 2){
        ERROR_COUNT = 1;
    }
    srand((unsigned int)time(NULL));
    while(1){
        for(int i = 0; i < 9; i ++){
            for (int j = 0; j < 9; j ++){
                num[i][j] = 0;
            }
        }
        while(1){
            generate_1();
            array_copy_9x9(tmp2, num);
            flag = answer();
            if (flag == -2)
                return -1;
            //printf("%d\n", flag);
            if (flag == MODE){
                break;
            }else if (flag == 0){
                array_copy_9x9(num, tmp2);
                break;
            }
        }
        //printf("OK\n");
        //printf("begin%d\n", time(0));
        flag = g();
        if (flag == -2)
            return -1;
        if (flag == 1)
            break;
        //printf("end\n");
    }
    /*
    printf("問題\n");
    show();
    printf("\n");
    printf("解答\n");
    answer();
    show();
    */
   append_data();
    return 1;
}
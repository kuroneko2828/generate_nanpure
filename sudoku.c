#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#define CLOCK

void input();  //問題の入力
void show();  //表の表示
void error(int x);  //エラーのチェック（開発中に使った）
int ul(int i,int j,char ij);  //そのブロックの左上の座標を求める
int set(int i,int j);  //method1の関数の一部
int pos_num();  //候補数の総和を求める
int pos_num_ij(int i, int j); //i,jの候補数を求める
int equal_check(int parent[9],int child[9]);  //childがparentの候補をすべてっ持っているのかのチェック
int link_check(int i1,int j1,int i2,int j2);  //つながりがあるかを調べる
int strong_link(int N,int i1,int j1,int i2,int j2);
void impos1(); //初期の候補削除
void impos2(int i,int j);  //適宜候補削除
//impos3~はテクニックを用いた候補削除
void impos3();
void impos4();
void impos4_2(int N1,int dest[9],int b,int N2);
void impos4_3(int dest[9]);
void impos5_1();
void impos5_2(int equal_n,int n[9],int i,int j,int b,int N);
void impos5_3(int equal_n,int n[9],int i,int j);
void impos6();
void impos7();
void impos7_2(int i1,int j1,int i2,int j2,int a);
void impos8();
void impos9();
void impos10();
void impos11();
void impos11_2(int N, int i, int j, int num11[81][2], int size);
void impos12();
void impos12_2(int start, int si, int sj, int pi, int pj, int prev, int num12[81][2], int size);
void impos13();
void impos13_2(int N, int pi, int pj, int pg, int num13[81][2][81][2], int size[81][2],int gszie);
void impos13_3(int N,int num13[81][2][81][2],int size[81][2],int gsize);
void impos14();
void impos15();
void impos15_2(int x,int y, int pi, int pj,int pg,int num15[][81][2],int size[]);
void impos15_3(int x,int y,int num15[][81][2],int size[]);
void method1();  //候補が一つの時入る数が決定する
void method2();  //ブロック等である数が入る場所が一つならそこに決定する
void line(int n);  //method2の一部
void row(int n);  //method2の一部
void block(int n);  //method2の一部
void give_up();  //最終手段として網羅的探索を行う

//マスに入っている数字(入っていない場合は0)
int num[9][9];

//各マスにおける候補
int pos[9][9][9];

//give_up関数で用いる終了用の関数
int finish=0;

int main(void){
    int i,j,complete,before,before_pos,num_pos,f;
#ifdef CLOCK
    clock_t start,end;
#endif
    input();
    impos1();
    before=complete=0;
    before_pos=num_pos=0;
#ifdef CLOCK
    start=clock();
#endif
    //完成する、もしくは入った数字の数や候補の数が変化しなくなるまで繰り返す
    do{
        before=complete;
        complete=0;
        before_pos=num_pos;
        num_pos=0;
        method1();
        method2();
        impos3();
        impos4();
        impos5_1();
        impos6();
        impos7();
        impos8();
        impos9();
        impos10();
        impos11();
        impos12();
        impos13();
        impos14();
        impos15();
        complete=0;
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                if(num[i][j]==0){
                    complete++;
                }
            }
        }
        num_pos=pos_num();
        
    }while(complete&&(before!=complete||before_pos!=num_pos));
    

    //結果を示す
    if(!complete)
    printf("完成しました\n");
    else{
        show();
        printf("失敗しました。網羅的探索を実行します。\n");
        give_up();
    }

    show();
    f=0;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(num[i][j]==0) f=1;
        }
    }
    if(f) printf("エラー：解は存在しません\n");

#ifdef CLOCK
    end=clock();
#endif

#ifdef CLOCK
    printf("所用時間：%.3f秒",(double)(end-start)/CLOCKS_PER_SEC);
#endif

    return 0;
}

//問題入力
void input(){
    int i,j;
    char check;
    printf("問題を入力してください。空欄の位置には0を入れてください。\n");
    for (i=0;i<9;i++){
        printf("第%d行目の数字を一つずつ入力してください\n",i+1);
        
        for(j=0;j<9;j++){
            scanf("%1d",&num[i][j]);
        }
        
        if(i==8){
            printf("これでいいですか？y/n\n");
            show();
            scanf("\n%c",&check);
            if(check=='n')
            i=-1;
        }
    }
}

//図の表示
void show(){
    int i,j;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            printf("%d",num[i][j]);
            if(j==2||j==5)
            printf("|");
        }
        printf("\n");
        if(i==2||i==5)
        printf("-----------\n");
    }
}

void error(int x){
    int i,j,N,f;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            f=1;
            for(N=0;N<9;N++){
                if(pos[i][j][N]) f=0;
            }
            if(f&&!num[i][j]){
                printf("error by %d",x);
                exit(1);
            }
        }
    }
}

//(i,j)があるブロックの左上の座標を求める
//第三引数がiなら何行目かを、jなら何列目かを返す
int ul(int i,int j,char ij){
    int x,y;
    switch(i%3){
        case 0:
        x=0;
        break;
        case 1:
        x=-1;
        break;
        case 2:
        x=-2;
        break;
    }
    switch(j%3){
        case 0:
        y=0;
        break;
        case 1:
        y=-1;
        break;
        case 2:
        y=-2;
        break;
    }
    if(ij=='i') return i+x;
    else if(ij=='j') return j+y;
    else return 0;
}

//method1の続き
int set(int i,int j){
    int k;
    int only=0;
    for(k=0;k<9;k++){
        if(pos[i][j][k]!=0){
            only++;
            if(only==1){
                num[i][j]=pos[i][j][k];
            }
            else{
                num[i][j]=0;
                return 0;
            }
        }
    }
    if(only==1)
    impos2(i,j);
    return only;
}

//候補の総数を求める
int pos_num(){
    int i,j,k,sum=0;
    for(i=0;i<9;i++)
    for(j=0;j<9;j++)
    for(k=0;k<9;k++)
    sum+=pos[i][j][k];
    return sum;
}

//i,jにおける候補の数を元める。
int pos_num_ij(int i, int j){
    int k,n=0;
    for(k=0;k<9;k++){
        if(pos[i][j][k]!=0)
        n++;
    }

    return n;
}

//childがparentの候補をすべて持っているかのチェック
//childはparentが持っていない候補を持っていてもいけない
int equal_check(int parent[9],int child[9]){
    int i;
    for(i=0;i<9;i++){
        if(child[i]&&parent[i]!=child[i])
        return 0;
    }
    return 1;
}


//リンクがあれば1を、なければ0を返す。
int link_check(int i1,int j1,int i2,int j2){
    int x1,y1,x2,y2;
    //iが等しい
    if(i1==i2){
        return 1;
    }
    //jが等しい
    if(j1==j2){
        return 1;
    }
    //ブロックが等しい
    x1=ul(i1,j1,'i');
    y1=ul(i1,j1,'j');
    x2=ul(i2,j2,'i');
    y2=ul(i2,j2,'j');
    if(x1==x2&&y1==y2){
        return 1;
    }
    return 0;
}

//強いリンクかどうかのチェック
int strong_link(int N,int i1,int j1,int i2,int j2){
    int x1,y1,x2,y2;
    int i,j,f;

    //iが等しい
    if(i1==i2){
        f=1;
        for(i=0;i<9;i++){
            if(i==j1||i==j2) continue;
            if(pos[i1][i][N-1]==N){
                f=0;
                break;
            }
        }
        return f;
    }

    //jが等しい
    if(j1==j2){
        f=1;
        for(i=0;i<9;i++){
            if(i==i1||i==i2) continue;
            if(pos[i][j1][N-1]==N){
                f=0;
                break;
            }
        }
        return f;
    }

    //ブロックが等しい
    x1=ul(i1,j1,'i');
    y1=ul(i1,j1,'j');
    x2=ul(i2,j2,'i');
    y2=ul(i2,j2,'j');
    if(x1==x2&&y1==y2){
        f=1;
        for(i=x1;i<x1+3;i++){
            for(j=y1;j<y1+3;j++){
                if((i==i1&&j==j1)||(i==i2&&j==j2)) continue;
                if(pos[i][j][N-1]==N){
                    f=0;
                    break;
                }
            }
        }
        return f;
    }
    return 0;
}

//最初に各マスの候補をチェックする
void impos1(){
    int i,j,k,x,y,l,m;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(num[i][j]==0){
                for(k=0;k<9;k++){
                    pos[i][j][k]=k+1;
                }
            }else{
                for(k=0;k<9;k++){
                    pos[i][j][k]=0;
                }
            }
        }
    }
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(num[i][j]==0){
                for(k=0;k<9;k++){
                    if(num[i][k]!=0)  //行をみて候補削除
                    pos[i][j][num[i][k]-1]=0;
                    if(num[k][j]!=0)  //列を見て候補削除
                    pos[i][j][num[k][j]-1]=0;
                }
                x=ul(i,j,'i');
                y=ul(i,j,'j');
                for(l=x;l<x+3;l++){
                    for(m=y;m<y+3;m++){
                        if(num[l][m]!=0)  //ブロックでの候補削除
                        pos[i][j][num[l][m]-1]=0;
                    }
                }
            }
        }
    }
}

//マスが埋まるたびに関連する候補の削除を行う
void impos2(int i,int j){
    int k,l,x,y;
    int N=num[i][j];
    //その行、列での候補削除
    for(k=0;k<9;k++){
        pos[i][k][N-1]=0;
        pos[k][j][N-1]=0;
    }
    //そのブロックでの候補削除
    x=ul(i,j,'i');
    y=ul(i,j,'j');
    for(k=x;k<x+3;k++){
        for(l=y;l<y+3;l++){
            pos[k][l][N-1]=0;
        }
    }
    //自分自身の候補削除
    for(k=0;k<9;k++)
    pos[i][j][k]=0;
}

//同ブロック中においてある数の候補が同行、列にある場合...(1)
//同行、列中においてある数の候補が同ブロックにある場合...(2)
void impos3(){
    int i,j,k,l,N,line,row,x,y,f;
    int a[9];
    for(N=1;N<=9;N++){
        //(1)の場合
        for(i=0;i<9;i+=3){
            for(j=0;j<9;j+=3){
                line=-1;
                row=-1;
                for(k=0;k<3;k++){
                    for(l=0;l<3;l++){
                        if(num[i+k][j+l]==0&&pos[i+k][j+l][N-1]==N){
                            a[3*k+l]=1;
                        }else{
                            a[3*k+l]=0;
                        }
                    }
                }
                for(k=0;k<9;k++){
                    if(a[k]==1){
                        if((k>=0&&k<3)&&(line==-1||line==0)) line=0;
                        else if((k>=3&&k<6)&&(line==-1||line==1)) line=1;
                        else if((k>=6&&k<9)&&(line==-1||line==2)) line=2;
                        else line=-2;

                        if(k%3==0&&(row==-1||row==0)) row=0;
                        else if(k%3==1&&(row==-1||row==1)) row=1;
                        else if(k%3==2&&(row==-1||row==2)) row=2;
                        else row=-2;
                    }
                }
                if(line>=0){
                    for(k=0;k<9;k++){
                        if(!(k>=j&&k<j+3))
                        pos[i+line][k][N-1]=0;
                    }
                }
                if(row>=0){
                    for(k=0;k<9;k++){
                        if(!(k>=i&&k<i+3))
                        pos[k][j+row][N-1]=0;
                    }
                }
            }
        }

        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                if(!num[i][j]&&pos[i][j][N-1]){
                    x=ul(i,j,'i');
                    y=ul(i,j,'j');
                    //(2)(行)の場合
                    for(k=0;k<9;k++)
                    a[k]=0;
                    f=1;
                    for(k=0;k<9;k++){
                        if(!num[i][k]&&pos[i][k][N-1]){
                            if(y==ul(i,k,'j')){
                                a[k]=1;
                            }else{
                                f=0;
                                break;
                            }
                        }
                    }
                    if(f){
                        for(k=x;k<x+3;k++){
                            for(l=y;l<y+3;l++){
                                if(!(a[l]&&k==i))
                                pos[k][l][N-1]=0;
                            }
                        }
                    }
                    //(2)(列)の場合
                    for(k=0;k<9;k++)
                    a[k]=0;
                    f=1;
                    for(k=0;k<9;k++){
                        if(!num[k][j]&&pos[k][j][N-1]){
                            if(x==ul(k,j,'i')){
                                a[k]=1;
                            }else{
                                f=0;
                                break;
                            }
                        }
                    }
                    if(f){
                        for(k=x;k<x+3;k++){
                            for(l=y;l<y+3;l++){
                                if(!(a[k]&&l==j))
                                pos[k][l][N-1]=0;
                            }
                        }
                    }
                }
            }
        }
    }
}

//ある行、列、ブロックに注目して、そこに(3,6,9),(3,9),(3,6)のような候補の組み合わせがあれば、
//その他のマスのその候補は消える。
//数字の種類の数と、その数字の候補があるマス数は等しくなければならない
void impos4(){
    int i,j,k,l,m,n,o,pos_n,N,x,y;
    int a[9],equal[9];

    for(i=2;i<9;i++){
        for(j=0;j<9;j++){
            a[j]=0;
        }
        impos4_2(i,a,0,1);
    }
}

void impos4_2(int N1,int dest[9],int b,int N2){
    int a;
    for(a=b+1;a<=9;a++){
        dest[a-1]=a;
        if(N2==N1){
            impos4_3(dest);
        }else{
            impos4_2(N1,dest,a,N2+1);
        }
        dest[a-1]=0;
    }
    N2--;
}

void impos4_3(int dest[]){
    int i,j,k,l,m,f,n;
    int a[9][9];
    int N=0;

    for(i=0;i<9;i++){
        if(dest[i]) N++;
    }

    //列、行方向
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(k=0;k<9;k++){
                a[j][k]=0;
            }
        }
        n=0;
        for(j=0;j<9;j++){
            if(num[i][j]) continue;
            f=equal_check(dest,pos[i][j]);
            if(!f) continue;
            n++;
            a[i][j]=1;
        }
        if(N==n){
            for(j=0;j<9;j++){
                if(a[i][j]==1) continue;
                for(k=1;k<=9;k++){
                    if(dest[k-1]){
                        pos[i][j][k-1]=0;
                    }
                }
            }
        }

        for(j=0;j<9;j++){
            for(k=0;k<9;k++){
                a[j][k]=0;
            }
        }
        n=0;
        for(j=0;j<9;j++){
            if(num[j][i]) continue;
            f=equal_check(dest,pos[j][i]);
            if(!f) continue;
            n++;
            a[j][i]=1;
        }
        if(N==n){
            for(j=0;j<9;j++){
                if(a[j][i]==1) continue;
                for(k=1;k<=9;k++){
                    if(dest[k-1]){
                        pos[j][i][k-1]=0;
                    }
                }
            }
        }
    }

    
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            for(k=0;k<9;k++){
                for(l=0;l<9;l++){
                    a[k][l]=0;
                }
            }
            n=0;
            for(k=0;k<3;k++){
                for(l=0;l<3;l++){
                    if(num[i*3+k][j*3+l]) continue;
                    f=equal_check(dest,pos[i*3+k][j*3+l]);
                    if(!f) continue;
                    n++;
                    a[i*3+k][j*3+l]=1;
                }
            }
            if(N==n){
                for(k=0;k<3;k++){
                    for(l=0;l<3;l++){
                        if(a[i*3+k][j*3+l]==1) continue;
                        for(m=1;m<=9;m++){
                            if(dest[m-1]){
                                pos[i*3+k][j*3+l][m-1]=0;
                            }
                        }
                    }
                }
            }
        }
    }
}

//ある行、列、ブロックに注目して(2,4,8),(2,4,6)のような組み合わせがあり、
//その他のマスに2,4の候補がなければこの2つのマスの候補は2,4のみになる
void impos5_1(){
    int i,j,k,N,equal_n;
    int n[9]={0,0,0,0,0,0,0,0,0};
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(num[i][j]==0){
                N=0;
                for(k=0;k<9;k++)
                if(pos[i][j][k]) N++;
                for(equal_n=2;equal_n<=N;equal_n++){
                    impos5_2(equal_n,n,i,j,0,1);
                }
            }
        }
    }
}

//候補の組み合わせをすべて試すために再帰を使用
void impos5_2(int equal_n,int n[9],int i,int j,int b,int N){ //最初はb=0,N=1;
    int a;
    for(a=b+1;a<=9;a++){
        if(!pos[i][j][a-1]) continue;
        if(N<=equal_n)
        n[a-1]=a;
        if(N==equal_n){  
            impos5_3(equal_n,n,i,j);
            
        }else{
            impos5_2(equal_n,n,i,j,a,N+1);
        }
        n[a-1]=0;
    }
    N--;
}

//5_2で出した候補の組み合わせを用いて条件を満たしているかを確かめる
//満たしていれば候補を削除する
void impos5_3(int equal_n,int n[9],int i,int j){
    int k,l,m,N,x,y,f;
    int equal[9];
    //fが0ならnの要素を全て持っている。
    //fが1以上なら、その分だけnの要素を持っていない。

    //行に注目
    N=0;
    for(k=0;k<9;k++)
    equal[k]=0;
    for (k=0;k<9;k++){
        f=0;
        if(num[i][k])
        continue;
        for(l=0;l<9;l++){
            if(n[l]&&n[l]!=pos[i][k][l]){
                f++;
            }
        }
        if(f==0){
            equal[k]=1;
            N++;
        }
        if(f>0&&f<equal_n){
            N=0;
            break;
        }
    }
    if(N==equal_n){
        for(k=0;k<9;k++){
            if(equal[k]){
                for(l=0;l<9;l++){
                    if(!n[l])
                    pos[i][k][l]=0;
                }
            }
        }
    }
    //列に注目
    N=0;
    for(k=0;k<9;k++)
    equal[k]=0;
    for (k=0;k<9;k++){
        f=0;
        if(num[k][j])
        continue;
        for(l=0;l<9;l++){
            if(n[l]&&n[l]!=pos[k][j][l]){
                f++;
            }
        }
        if(f==0){
            equal[k]=1;
            N++;
        }
        if(f>0&&f<equal_n){
            N=0;
            break;
        }
    }
    if(N==equal_n){
        for(k=0;k<9;k++){
            if(equal[k]){
                for(l=0;l<9;l++){
                    if(!n[l])
                    pos[k][j][l]=0;
                }
            }
        }
    }
    //ブロックに注目
    N=0;
    for(k=0;k<9;k++)
    equal[k]=0;
    x=ul(i,j,'i');
    y=ul(i,j,'j');
    for(k=x;k<x+3;k++){
        for(l=y;l<y+3;l++){
            f=0;
            if(num[k][l])
            continue;
            for(m=0;m<9;m++){
                if(n[m]&&n[m]!=pos[k][l][m]){
                    f++;
                }
            }
            if(f==0){
                equal[3*(k-x)+l%3]=1;
                N++;
            }
            if(f>0&&f<equal_n){
                N=-1;
                break;
            }
        }
        if(N==-1)
            break;
    }
    if(N==equal_n){
        for(k=x;k<x+3;k++){
            for(l=y;l<y+3;l++){
                if(equal[3*(k-x)+l%3]){
                    for(m=0;m<9;m++){
                        if(!n[m])
                        pos[k][l][m]=0;
                    }
                }
            }
        }
    }
}

//n本の行(列)に注目して、ある数の候補があるマスがn個以下同じ列(行)にあったら、
//その列の(行)のほかのマスの候補からはその数は消える
void impos6(){
    int i,j,k,l,m,N,f,f2,equal_n;
    int equal[9][9];
    
    for(N=1;N<=9;N++){
        //行に注目
        for(k=0;k<9;k++){
            equal_n=0;
            l=0;
            for(i=0;i<9;i++){
                for(j=0;j<9;j++)
                equal[i][j]=0;
            }
            for(j=0;j<9;j++){
                if(!num[k][j]&&pos[k][j][N-1]){
                    equal[k][j]=1;
                    equal_n++;
                }
            }
            if(!equal_n)
            continue;
            for(i=0;i<9;i++){
                f=1;   //元の行の中の候補のある列以外の列に候補がある場合0になる
                f2=0;   //その行に候補があるなら1になる
                for(j=0;j<9;j++){
                    if(!num[i][j]&&pos[i][j][N-1]){
                        if(equal[k][j]){
                            equal[i][j]=1;
                            f2=1;
                        }
                        else
                        f=0;
                        if(!f)
                        break;
                    }
                }
                if(f&&f2)
                l++;
                else{
                    for(j=0;j<9;j++){
                        equal[i][j]=0;
                    }
                }
            }
            if(l>=equal_n){ //l>=equal_n ? equal_n==l
                for(i=0;i<9;i++){
                    for(j=0;j<9;j++){
                        if(equal[i][j]){
                            for(m=0;m<9;m++){
                                if(!equal[i][m])
                                pos[i][m][N-1]=0;
                                if(!equal[m][j])
                                pos[m][j][N-1]=0;
                            }
                        }
                    }
                }
            }
        }

        //列に注目
        for(k=0;k<9;k++){
            equal_n=0;
            l=0;
            for(i=0;i<9;i++){
                for(j=0;j<9;j++)
                equal[i][j]=0;
            }
            for(i=0;i<9;i++){
                if(!num[i][k]&&pos[i][k][N-1]){
                    equal[i][k]=1;
                    equal_n++;
                }
            }
            if(!equal_n)
            continue;
            for(j=0;j<9;j++){
                f=1;   //元の列の中の候補のある行以外の行に候補がある場合0になる
                f2=0;   //その行に候補があるなら1になる
                for(i=0;i<9;i++){
                    if(!num[i][j]&&pos[i][j][N-1]){
                        if(equal[i][k]){
                            equal[i][j]=1;
                            f2=1;
                        }
                        else
                        f=0;
                        if(!f)
                        break;
                    }
                }
                if(f&&f2)
                l++;
                else{
                    for(i=0;i<9;i++){
                        equal[i][j]=0;
                    }
                }
            }
            if(l>=equal_n){
                for(i=0;i<9;i++){
                    for(j=0;j<9;j++){
                        if(equal[i][j]){
                            for(m=0;m<9;m++){
                                if(!equal[i][m])
                                pos[i][m][N-1]=0;
                                if(!equal[m][j])
                                pos[m][j][N-1]=0;
                            }
                        }
                    }
                }
            }
        }
    }
}

//xy_wing
void impos7(){
    int i,j,k,l,m,n,o,N,a,x,y,x1,y1,x2,y2,x3,y3,f;
    int xy[9];
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(num[i][j]==0){
                N=0;
                for(k=0;k<9;k++){
                    if(pos[i][j][k]!=0){
                        xy[N]=pos[i][j][k];
                        N++;
                    }
                }
                if(N!=2) continue;
                x=ul(i,j,'i');
                y=ul(i,j,'j');
                
                //横ラインを見る
                for(k=0;k<9;k++){
                    f=-1;
                    if(k==j) continue; //同じセルは無視
                    if(pos_num_ij(i,k)!=2) continue;
                    
                    for(l=0;l<9;l++){
                        if(pos[i][k][l]==xy[0]){
                            for(m=0;m<9;m++){
                                if(pos[i][k][m]!=0&&l!=m&&pos[i][k][m]!=xy[1]){
                                    f=1;
                                    a=pos[i][k][m];
                                }
                            }
                        }
                    }
                    
                    if(f==-1){
                        for(l=0;l<9;l++){
                            if(pos[i][k][l]==xy[1]){
                                for(m=0;m<9;m++){
                                    if(pos[i][k][m]!=0&&l!=m&&pos[i][k][m]!=xy[0]){
                                        f=0;
                                        a=pos[i][k][m];
                                    }
                                }
                            }
                        }
                    }
                    
                    if(f==-1) continue;
                    
                    //横ラインを見る(2つめのマス)
                    for(l=0;l<9;l++){
                        if(l==j||l==k) continue;
                        if(pos_num_ij(i,l)!=2) continue;
                        for(m=0;m<9;m++){
                            if(pos[i][l][m]==xy[f]&&pos[i][l][a-1]==a){
                                impos7_2(i,k,i,l,a);
                            }
                        }
                    }
                    
                    //縦ラインを見る(2つめのマス)
                    for(l=0;l<9;l++){
                        if(l==i) continue;
                        if(pos_num_ij(l,j)!=2) continue;
                        for(m=0;m<9;m++){
                            if(pos[l][j][m]==xy[f]&&pos[l][j][a-1]==a){
                               impos7_2(i,k,l,j,a);
                            }
                        }
                    }
                    
                    //ブロックを見る(2つめのマス)
                    for(l=x;l<x+3;l++){
                        for(m=y;m<y+3;m++){
                            if(l==i) continue;
                            if(pos_num_ij(l,m)!=2) continue;
                            for(n=0;n<9;n++){
                                if(pos[l][m][n]==xy[f]&&pos[l][m][a-1]==a){
                                    impos7_2(i,k,l,m,a);
                                }
                            }
                        }
                    }
                    
                }

                
                //縦ラインを見る
                for(k=0;k<9;k++){
                    f=-1;
                    if(k==i) continue; //同じセルは無視
                    if(pos_num_ij(k,j)!=2) continue;
                    for(l=0;l<9;l++){
                        if(pos[k][j][l]==xy[0]){
                            for(m=0;m<9;m++){
                                if(pos[k][j][m]!=0&&l!=m&&pos[k][j][m]!=xy[1]){
                                    f=1;
                                    a=pos[k][j][m];
                                }
                            }
                        }
                    }
                    if(f==-1){
                        for(l=0;l<9;l++){
                            if(pos[k][j][l]==xy[1]){
                                for(m=0;m<9;m++){
                                    if(pos[k][j][m]!=0&&l!=m&&pos[k][j][m]!=xy[0]){
                                        f=0;
                                        a=pos[k][j][m];
                                    }
                                }
                            }
                        }
                    }
                    if(f==-1) continue;
                    //縦を見る(2つめのマス)
                    for(l=0;l<9;l++){
                        if(l==i||l==k) continue;
                        if(pos_num_ij(l,j)!=2) continue;
                        for(m=0;m<9;m++){
                            if(pos[l][j][m]==xy[f]&&pos[l][j][a-1]==a){
                                impos7_2(k,j,l,j,a);
                            }
                        }
                    }
                    //ブロックを見る(2つめのマス)
                    for(l=x;l<x+3;l++){
                        for(m=y;m<y+3;m++){
                            if(m==j) continue;
                            if(pos_num_ij(l,m)!=2) continue;
                            for(n=0;n<9;n++){
                                if(pos[l][m][n]==xy[f]&&pos[l][m][a-1]==a){
                                    impos7_2(k,j,l,m,a);
                                }
                            }
                        }
                    }
                }
                //2つとも同ブロック
                for(k=x;k<x+3;k++){
                    for(l=y;l<y+3;l++){
                        f=-1;
                        if(k==i&&l==j) continue; //同じセルは無視
                        if(pos_num_ij(k,l)!=2) continue;
                        for(m=0;m<9;m++){
                            if(pos[k][l][m]==xy[0]){
                                for(n=0;n<9;n++){
                                    if(pos[k][l][n]!=0&&m!=n&&pos[k][l][n]!=xy[1]){
                                        f=1;
                                        a=pos[k][l][n];
                                    }
                                }
                            }
                        }
                        if(f==-1){
                            for(m=0;m<9;m++){
                                if(pos[k][l][m]==xy[1]){
                                    for(n=0;n<9;n++){
                                        if(pos[k][l][n]!=0&&m!=n&&pos[k][l][n]!=xy[0]){
                                            f=0;
                                            a=pos[k][l][n];
                                        }
                                    }
                                }
                            }
                        }
                        if(f==-1) continue;
                        for(m=x;m<x+3;m++){
                            for(n=y;n<y+3;n++){
                                if((m==i&&n==j)||(m==k&&n==l)) continue;
                                if(pos_num_ij(m,n)!=2) continue;
                                for(o=0;o<9;o++){
                                    if(pos[m][n][o]==xy[f]&&pos[m][n][a-1]==a){
                                        impos7_2(k,l,m,n,a);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//同じブロックになったとき
void impos7_2(int i1,int j1,int i2,int j2,int a){
    int i,j,x1,y1,x2,y2;
    x1=ul(i1,j1,'i');
    y1=ul(i1,j1,'j');
    x2=ul(i2,j2,'i');
    y2=ul(i2,j2,'j');

    if(x1==x2&&y1==y2){
        for(i=x1;i<x1+3;i++){
            for(j=y1;j<y1+3;j++){
                if((i==i1&&j==j1)||(i==i2&&j==j2)) continue;
                pos[i][j][a-1]=0;
            }
        }
    }
    else if(y1==y2){
        for(i=x1;i<x1+3;i++){
            if(i!=i1||j1!=j2)
            pos[i][j2][a-1]=0;
        }
        for(i=x2;i<x2+3;i++){
            if(i!=i2||j1!=j2)
            pos[i][j1][a-1]=0;
        }
    }
    else if(x1==x2){
        for(i=y1;i<y1+3;i++){
            if(i!=j1||i1!=i2)
            pos[i2][i][a-1]=0;
        }
        for(i=y2;i<y2+3;i++){
            if(i!=j2||i1!=i2)
            pos[i1][i][a-1]=0;
        }
    }else{
        pos[i1][j2][a-1]=0;
        pos[i2][j1][a-1]=0;
    }
    
}

//LockedCandidate
void impos8(){
    int N,h,i,j,k,l,f,L,R;
    int a[9];
    for(N=1;N<=9;N++){
        for(h=0;h<3;h++){
            L=R=1;
            for(i=0;i<9;i++){
                for(j=0;j<3;j++){
                    if(num[h*3+j][i]==N)
                    L=0;
                    if(num[i][h*3+j]==N)
                    R=0;
                }
            }
            for(i=0;i<3;i++){
                
                if(L){
                    for(k=0;k<9;k++){
                        a[k]=1;
                    }
                    f=0;
                    for(j=0;j<3;j++){
                        if(j==i) continue;
                        for(k=h*3;k<h*3+3;k++){
                            for(l=j*3;l<j*3+3;l++){
                                if(pos[k][l][N-1]){
                                    a[k]=0;
                                }
                            }
                        }
                    }
                    
                    for(k=0;k<9;k++){
                        if(!a[k])
                        f++;
                    }
                    if(f==2){
                        for(k=0;k<9;k++){
                        }
                        for(k=h*3;k<h*3+3;k++){
                            if(a[k]==0){
                                for(l=i*3;l<i*3+3;l++){
                                    pos[k][l][N-1]=0;
                                }
                            }
                        }
                    }
                }
                
                if(R){
                    for(k=0;k<3;k++){
                        a[k]=1;
                    }
                    f=0;
                    for(j=0;j<3;j++){
                        if(i==j) continue;
                        for(k=h*3;k<h*3+3;k++){
                            for(l=j*3;l<j*3+3;l++){
                                if(pos[l][k][N-1]==N){
                                    a[k%3]=0;
                                }
                            }
                        }
                    }
                    for(k=0;k<3;k++){
                        if(a[k])
                        f++;
                    }
                    if(f==1)
                    for(k=0;k<3;k++){
                        if(!a[k]){
                            for(l=i*3;l<i*3+3;l++){
                                pos[l][h*3+k][N-1]=0;
                            }
                        }
                    }
                }
            }
        }
    }
}

//Skyscraper
void impos9(){
    int N,i,j,k,l,m,n,x1,y1,x2,y2,data=0,f;
    int link[1000][2][2];
    int d[2][2];
    for(N=1;N<=9;N++){
        data=0;
        for(i=0;i<9;i++){
            f=0;
            for(j=0;j<9;j++){
                if(pos[i][j][N-1]){
                    if(f==2){f++; break;}
                    d[f][0]=i;
                    d[f][1]=j;
                    f++;
                }
            }
            if(f==2){
                for(j=0;j<2;j++){
                    link[data][0][j]=d[0][j];
                    link[data][1][j]=d[1][j];
                    link[data+1][0][j]=d[1][j];
                    link[data+1][1][j]=d[0][j];
                }
                data+=2;
            }

            f=0;
            for(j=0;j<9;j++){
                if(pos[j][i][N-1]){
                    if(f==2){f++; break;}
                    d[f][0]=j;
                    d[f][1]=i;
                    f++;
                }
            }
            if(f==2){
                for(j=0;j<2;j++){
                    link[data][0][j]=d[0][j];
                    link[data][1][j]=d[1][j];
                    link[data+1][0][j]=d[1][j];
                    link[data+1][1][j]=d[0][j];
                }
                data+=2;
            }
        }
        f=0;
        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                for(k=i*3;k<i*3+3;k++){
                    for(l=j*3;l<j*3+3;l++){
                        if(pos[k][l][N-1]){
                            if(f==2){f++; break;}
                            d[f][0]=k;
                            d[f][1]=l;
                            f++;
                        }
                    }
                }
                if(f==2){
                    for(j=0;j<2;j++){
                        link[data][0][j]=d[0][j];
                        link[data][1][j]=d[1][j];
                        link[data+1][0][j]=d[1][j];
                        link[data+1][1][j]=d[0][j];
                    }
                    data+=2;
                }
            }
        }

        
        for(i=0;i<data-1;i++){
            for(j=i+1;j<data;j++){
                
                if(j==i+1&&i%2==0) continue;
                
                if((link[i][0][0]==link[j][0][0])&&(link[i][0][1]==link[j][0][1])) continue;
                if((link[i][0][0]==link[j][1][0])&&(link[i][0][1]==link[j][1][1])) continue;
                if((link[i][1][0]==link[j][0][0])&&(link[i][1][1]==link[j][0][1])) continue;
                if((link[i][1][0]==link[j][1][0])&&(link[i][1][1]==link[j][1][1])) continue;

                f=link_check(link[i][0][0],link[i][0][1],link[j][0][0],link[j][0][1]);

                if(f){
                    f=0;
                    f=link_check(link[i][0][0],link[i][0][1],link[j][1][0],link[j][1][1]);
                    f=link_check(link[i][1][0],link[i][1][1],link[j][0][0],link[j][0][1]);
                    f=link_check(link[i][0][0],link[i][0][1],link[j][0][0],link[j][0][1]);
                    if(f) continue;

                    impos7_2(link[i][0][0],link[i][0][1],link[j][0][0],link[j][0][1],N);
                }
                
            }
        }
    }
}

//EmptyRectangle
void impos10(){
    int N,i,j,k,l,m,f1,f2,f3,J,K;
    int x;
    for(N=1;N<=9;N++){
        for(i=0;i<9;i++){  //ブロックの選択
            J=3*((int)(i/3));
            K=3*((int)(i%3));
            f1=1;
            for(j=0;j<3;j++){
                for(k=0;k<3;k++){
                    if(num[J+j][K+k]==N)
                    f1=0;
                }
            }
            if(!f1) continue;

            for(j=0;j<3;j++){//軸の選択（行）
                for(k=0;k<3;k++){//軸の選択（列）
                    
                    f3=1;
                    for(l=0;l<3;l++){
                        for(m=0;m<3;m++){
                            
                            if((pos[J+l][K+m][N-1]==N)&&((l!=j)&&(m!=k))){
                                f3=0;
                            }
                        }
                    }
                    if(f3==0) continue;
                    
                    for(l=0;l<9;l++){
                        f2=0;
                        if(l>=J&&l<J+3) continue;
                        if(pos[l][K+k][N-1]==N){
                            for(m=0;m<9;m++){
                                if(m>=K&&m<K+3){
                                    if(m==K+k) continue;
                                    if(pos[l][m][N-1]==N){
                                        f2=0;
                                        break;
                                    }
                                }
                                if(pos[l][m][N-1]==N){
                                    if(f2==1){
                                        f2=0;
                                        break;
                                    }
                                    f2=1;
                                    x=m;
                                }
                            }
                            if(f2&&pos[J+j][x][N-1]){
                                pos[J+j][x][N-1]=0;
                            }
                        }
                    }
                    
                    for(l=0;l<9;l++){
                        f2=0;
                        if(l>=K&&l<K+3) continue;
                        if(pos[J+j][l][N-1]==N){
                            for(m=0;m<9;m++){
                                if(m>=J&&m<J+3){
                                    if(m==J+j) continue;
                                    if(pos[m][l][N-1]==N){
                                        f2=0;
                                        break;
                                    }
                                }
                                if(pos[m][l][N-1]==N){
                                    if(f2==1){
                                        f2=0;
                                        break;
                                    }
                                    f2=1;
                                    x=m;
                                }
                            }
                            if(f2&&pos[x][K+k][N-1]){
                                pos[x][K+k][N-1]=0;
                            
                            }
                        }
                    }
                    
                }
            }
        }
    }
}

//Xchain
void impos11(){
    int N,i,j;
    int num11[81][2],size=0;
    for(N=1;N<=9;N++){
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                if(pos[i][j][N-1]==N){
                    num11[size][0]=i;
                    num11[size][1]=j;
                    size++;
                    impos11_2(N,i,j,num11,size);
                    size--;
                }
            }
        }
    }
}

void impos11_2(int N, int i, int j, int num11[][2], int size){
    int k,l,m,n,f1,f2,i0,j0;
    
    for(k=0;k<9;k++){
        for(l=0;l<9;l++){
            if(pos[k][l][N-1]==0) continue;
            f1=0;
            for(m=0;m<size;m++){
                if(k==num11[m][0]&&l==num11[m][1]){
                    f1=1;
                    break;
                }
            }
            if(f1) continue;
            if(size%2==1){
                f1=strong_link(N,i,j,k,l);
                if(!f1) continue;
                num11[size][0]=k;
                num11[size][1]=l;
                size++;
                impos11_2(N,k,l,num11,size);
                size--;
            }else{
                f1=link_check(i,j,k,l);
                if(!f1) continue;
                i0=num11[0][0];
                j0=num11[0][1];
                f1=link_check(i0,j0,k,l);
                if(f1){
                    pos[k][l][N-1]=0;
                    continue;
                }
                num11[size][0]=k;
                num11[size][1]=l;
                size++;
                impos11_2(N,k,l,num11,size);
                size--;
            }
        }
    }
}

//XYchian
void impos12(){
    int N,i,j,k,l;
    int num12[81][2];
    int size=0;
    for(N=1;N<=9;N++){
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                if(pos_num_ij(i,j)==2&&pos[i][j][N-1]==N){
                    for(k=1;k<=9;k++){
                        if(k==N) continue;
                        if(pos[i][j][k-1]==k){
                            num12[size][0]=i;
                            num12[size][1]=j;
                            size++;
                            impos12_2(N,i,j,i,j,k,num12,size);
                            size--;
                        }
                    }
                }
            }
        }
    }
}

void impos12_2(int start, int si, int sj, int pi, int pj, int prev, int num12[][2], int size){
    int i,j,k,f;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            f=0;
            for(k=0;k<size;k++){
                if(i==num12[k][0]&&j==num12[k][1]){
                    f=1;
                    break;
                }
            }
            if(f) continue;
            if(pos[i][j][prev-1]==prev&&pos_num_ij(i,j)==2){
                f=strong_link(prev,i,j,pi,pj);
                if(!f) continue;
                for(k=1;k<=9;k++){
                    if(k==prev) continue;
                    if(pos[i][j][k-1]==k){
                        if(k==start){
                            impos7_2(i,j,si,sj,start);
                        }
                        num12[size][0]=i;
                        num12[size][1]=j;
                        size++;
                        impos12_2(start,si,sj,i,j,k,num12,size);
                        size--;
                    }
                }
            }
        }
    }
}

//Coloring
void impos13(){
    int N,i,j,k,l,m,n,o,p,f,pi,pj,ei,ej,ef;
    int num13[81][2][81][2]; //グループ、カラー、何個目のマスか、座標
    int size[81][2];
    int gsize=0;
    ef=0;
    pi=pj=0;
    for(N=1;N<=9;N++){
        for(i=0;i<81;i++){
            size[i][0]=0;
            size[i][1]=0;
        }
        gsize=0;
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                f=0;
                for(k=0;k<gsize;k++){
                    for(l=0;l<2;l++){
                        for(m=0;m<size[k][l];m++){
                            if(i==num13[k][l][m][0]&&j==num13[k][l][m][1]){
                                f=1;
                                break;
                            }
                            if(i==num13[0][0][0][0]&&j==num13[0][0][0][0]) f=1;
                        }
                        if(f) break;
                    }
                    if(f) break;
                }
                if(f) continue;

                if(pos[i][j][N-1]==0) continue;
                num13[gsize][0][size[gsize][0]][0]=i;
                num13[gsize][0][size[gsize][0]][1]=j;
                size[gsize][0]++;
                impos13_2(N,i,j,0,num13,size,gsize);
                gsize++;
            }
        }
        impos13_3(N,num13,size,gsize);
    }
}

void impos13_2(int N, int pi, int pj, int pg, int num13[][2][81][2],int size[][2],int gsize){
    int i,j,k,l,f;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(pos[i][j][N-1]==0) continue;
            f=0;
            for(k=0;k<2;k++){
                for(l=0;l<size[gsize][k];l++){
                    if(i==num13[gsize][k][l][0]&&j==num13[gsize][k][l][1]){
                        f=1;
                        break;
                    }
                }
            }
            if(f) continue;
            f=strong_link(N,i,j,pi,pj);
            if(!f) continue;
            if(pg==0){
                num13[gsize][1][size[gsize][1]][0]=i;
                num13[gsize][1][size[gsize][1]][1]=j;
                size[gsize][1]++;
                impos13_2(N,i,j,1,num13,size,gsize);
            }else{
                num13[gsize][0][size[gsize][0]][0]=i;
                num13[gsize][0][size[gsize][0]][1]=j;
                size[gsize][0]++;
                impos13_2(N,i,j,0,num13,size,gsize);
            }
        }
    }
}

void impos13_3(int N,int num13[][2][81][2],int size[][2],int gsize){
    int i,j,k,l,m,n,f;
    for(i=0;i<gsize;i++){
        //Trap
        for(j=0;j<size[i][0];j++){
            for(k=0;k<size[i][1];k++){
                impos7_2(num13[i][0][j][0],num13[i][0][j][1],num13[i][1][k][0],num13[i][1][k][1],N);
            }
        }
        //Wrap
        for(j=0;j<size[i][0]-1;j++){
            for(k=j+1;k<size[i][0];k++){
                f=link_check(num13[i][0][j][0],num13[i][0][j][1],num13[i][0][k][0],num13[i][0][k][1]);
                if(f){
                    for(l=0;l<size[i][0];l++){
                        pos[num13[i][0][l][0]][num13[i][0][l][1]][N-1]=0;
                    }
                    break;
                }
            }
            if(f) break;
        }
        for(j=0;j<size[i][1]-1;j++){
            for(k=j+1;k<size[i][1];k++){
                f=link_check(num13[i][1][j][0],num13[i][1][j][1],num13[i][1][k][0],num13[i][1][k][1]);
                if(f){
                    for(l=0;l<size[i][1];l++){
                        pos[num13[i][1][l][0]][num13[i][1][l][1]][N-1]=0;
                    }
                    break;
                }
            }
            if(f) break;
        }
    }

    for(i=0;i<gsize-1;i++){
        for(j=i+1;j<gsize;j++){
            //type1
            for(k=0;k<size[i][0];k++){
                for(l=0;l<size[j][0];j++){
                    f=link_check(num13[i][0][k][0],num13[i][0][k][1],num13[j][0][l][0],num13[j][0][l][1]);
                    if(f){
                        for(m=0;m<size[i][1];m++){
                            for(n=0;n<size[j][1];n++){
                                impos7_2(num13[i][1][m][0],num13[i][1][m][1],num13[j][1][n][0],num13[j][1][n][1],N);
                            }
                        }
                    }
                }
            }
            for(k=0;k<size[i][0];k++){
                for(l=0;l<size[j][1];j++){
                    f=link_check(num13[i][0][k][0],num13[i][0][k][1],num13[j][1][l][0],num13[j][1][l][1]);
                    if(f){
                        for(m=0;m<size[i][1];m++){
                            for(n=0;n<size[j][0];n++){
                                impos7_2(num13[i][1][m][0],num13[i][1][m][1],num13[j][0][n][0],num13[j][0][n][1],N);
                            }
                        }
                    }
                }
            }
            for(k=0;k<size[i][1];k++){
                for(l=0;l<size[j][0];j++){
                    f=link_check(num13[i][1][k][0],num13[i][1][k][1],num13[j][0][l][0],num13[j][0][l][1]);
                    if(f){
                        for(m=0;m<size[i][0];m++){
                            for(n=0;n<size[j][1];n++){
                                impos7_2(num13[i][0][m][0],num13[i][0][m][1],num13[j][1][n][0],num13[j][1][n][1],N);
                            }
                        }
                    }
                }
            }
            for(k=0;k<size[i][1];k++){
                for(l=0;l<size[j][1];j++){
                    f=link_check(num13[i][1][k][0],num13[i][1][k][1],num13[j][1][l][0],num13[j][1][l][1]);
                    if(f){
                        for(m=0;m<size[i][0];m++){
                            for(n=0;n<size[j][0];n++){
                                impos7_2(num13[i][0][m][0],num13[i][0][m][1],num13[j][0][n][0],num13[j][0][n][1],N);
                            }
                        }
                    }
                }
            }

            //type2
            f=0;
            for(k=0;k<size[i][0];k++){
                for(l=0;l<size[j][0];l++){
                    f=link_check(num13[i][0][k][0],num13[i][0][k][1],num13[j][0][l][0],num13[j][0][l][1]);
                    if(f) break;
                }
                if(f) break;
            }
            if(f){
                f=0;
                for(k=0;k<size[i][0];k++){
                    for(l=0;l<size[j][1];l++){
                        f=link_check(num13[i][0][k][0],num13[i][0][k][1],num13[j][1][l][0],num13[j][1][l][1]);
                        if(f) break;
                    }
                    if(f) break;
                }
                if(f){
                    for(k=0;k<size[i][1];k++){
                        pos[num13[i][1][k][0]][num13[i][1][k][1]][N-1]=0;
                    }
                }
            }
            f=0;
            for(k=0;k<size[i][1];k++){
                for(l=0;l<size[j][0];l++){
                    f=link_check(num13[i][1][k][0],num13[i][1][k][1],num13[j][0][l][0],num13[j][0][l][1]);
                    if(f) break;
                }
                if(f) break;
            }
            if(f){
                f=0;
                for(k=0;k<size[i][1];k++){
                    for(l=0;l<size[j][1];l++){
                        f=link_check(num13[i][1][k][0],num13[i][1][k][1],num13[j][1][l][0],num13[j][1][l][1]);
                        if(f) break;
                    }
                    if(f) break;
                }
                if(f){
                    for(k=0;k<size[i][0];k++){
                        pos[num13[i][0][k][0]][num13[i][0][k][1]][N-1]=0;
                    }
                }
            }
            f=0;
            for(k=0;k<size[j][0];k++){
                for(l=0;l<size[i][0];l++){
                    f=link_check(num13[j][0][k][0],num13[j][0][k][1],num13[i][0][l][0],num13[i][0][l][1]);
                    if(f) break;
                }
                if(f) break;
            }
            if(f){
                f=0;
                for(k=0;k<size[j][0];k++){
                    for(l=0;l<size[i][1];l++){
                        f=link_check(num13[j][0][k][0],num13[j][0][k][1],num13[i][1][l][0],num13[i][1][l][1]);
                        if(f) break;
                    }
                    if(f) break;
                }
                if(f){
                    for(k=0;k<size[j][1];k++){
                        pos[num13[j][1][k][0]][num13[j][1][k][1]][N-1]=0;
                    }
                }
            }
            f=0;
            for(k=0;k<size[j][1];k++){
                for(l=0;l<size[i][0];l++){
                    f=link_check(num13[j][1][k][0],num13[j][1][k][1],num13[i][0][l][0],num13[i][0][l][1]);
                    if(f) break;
                }
                if(f) break;
            }
            if(f){
                f=0;
                for(k=0;k<size[j][1];k++){
                    for(l=0;l<size[i][1];l++){
                        f=link_check(num13[j][1][k][0],num13[j][1][k][1],num13[i][1][l][0],num13[i][1][l][1]);
                        if(f) break;
                    }
                    if(f) break;
                }
                if(f){
                    for(k=0;k<size[j][0];k++){
                        pos[num13[j][0][k][0]][num13[j][0][k][1]][N-1]=0;
                    }
                }
            }
        }
    }
}

//W-Wing
void impos14(){
    int N,y,i,j,k,l,m,n,o,p,f;
    int num14[4][2];
    for(N=1;N<=9;N++){
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                if(pos[i][j][N-1]==0||pos_num_ij(i,j)!=2) continue;
                for(k=1;k<=9;k++){
                    if(k==N) continue;
                    if(pos[i][j][k-1]==k){
                        y=k;
                        break;
                    }
                }
                for(k=0;k<9;k++){
                    for(l=0;l<9;l++){
                        if(k==i&&l==j) continue;
                        if(pos[k][l][N-1]==0) continue;
                        f=link_check(i,j,k,l);
                        if(!f) continue;
                        for(m=0;m<9;m++){
                            for(n=0;n<9;n++){
                                if((m==i&&n==j)||(m==k&&n==l)) continue;
                                if(pos[k][l][N-1]==0) continue;
                                f=strong_link(N,k,l,m,n);
                                if(!f) continue;
                                for(o=0;o<9;o++){
                                    for(p=0;p<9;p++){
                                        if((o==i&&p==j)||(o==k&&p==l)||(o==m&&p==n)) continue;
                                        if(pos[o][p][N-1]==0||pos[o][p][y-1]==0||pos_num_ij(o,p)!=2) continue;
                                        f=link_check(m,n,o,p);
                                        if(!f) continue;
                                        impos7_2(i,j,o,p,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//Remote Pair
void impos15(){
    int x,y,i,j,k,f;
    int num15[2][81][2];
    int size[2];
    for(x=1;x<=9;x++){
        size[0]=size[1]=0;
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                f=0;
                for(k=0;k<size[0];k++){
                    if(i==num15[0][k][0]&&j==num15[0][k][1]){
                        f=1;
                        break;
                    }
                }
                if(f) continue;
                f=0;
                for(k=0;k<size[1];k++){
                    if(i==num15[1][k][0]&&j==num15[1][k][1]){
                        f=1;
                        break;
                    }
                }
                if(f) continue;
                size[0]=size[1]=0;
                if(pos[i][j][x-1]==0||pos_num_ij(i,j)!=2) continue;
                for(k=1;k<=9;k++){
                    if(k==x) continue;
                    if(pos[i][j][k-1]==k){
                        y=k;
                        break;
                    }
                }
                num15[0][size[0]][0]=i;
                num15[0][size[0]][1]=j;
                size[0]++;
                impos15_2(x,y,i,j,0,num15,size);
                impos15_3(x,y,num15,size);
            }
        }
    }
}

void impos15_2(int x,int y, int pi, int pj,int pg,int num15[][81][2],int size[]){
    int i,j,k,l,f;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(pos[i][j][x-1]==0||pos[i][j][y-1]==0||pos_num_ij(i,j)!=2) continue;
            /*
            f=strong_link(x,i,j,pi,pj);
            if(!f) continue;
            f=strong_link(y,i,j,pi,pj);
            if(!f) continue;
            */
            f=link_check(i,j,pi,pj);
            if(!f) continue;
            f=0;
            for(k=0;k<size[0];k++){
                if(i==num15[0][k][0]&&j==num15[0][k][1]){
                    f=1;
                    break;
                }
            }
            if(f) continue;
            f=0;
            for(k=0;k<size[1];k++){
                if(i==num15[1][k][0]&&j==num15[1][k][1]){
                    f=1;
                    break;
                }
            }
            if(f) continue;
            if(pg==0){
                num15[1][size[1]][0]=i;
                num15[1][size[1]][1]=j;
                size[1]++;
                impos15_2(x,y,i,j,1,num15,size);
            }else{
                num15[0][size[0]][0]=i;
                num15[0][size[0]][1]=j;
                size[0]++;
                impos15_2(x,y,i,j,0,num15,size);
            }
        }
    }
}

void impos15_3(int x,int y,int num15[][81][2],int size[]){
    int i,j,k,l,m,n,o,p,f,f1;
    for(i=0;i<size[0];i++){
        for(j=0;j<size[1];j++){
            impos7_2(num15[0][i][0],num15[0][i][1],num15[1][j][0],num15[1][j][1],x);
            impos7_2(num15[0][i][0],num15[0][i][1],num15[1][j][0],num15[1][j][1],y);
        }
    }
}

//候補が一つの時、その数に決定
void method1(){
    int i,j,k,l,m,x,y;
    int only=0;
    int finish=1;
    do{
        finish=1;
        for(i=0;i<9;i++){
            for(j=0;j<9;j++){
                if(num[i][j]==0){
                    only=0;
                    
                    only=set(i,j);
                    if(only==1){ 
                    finish=0;
                    }
                    else
                    num[i][j]=0;
                }
            }
        }
    }while(finish==0);  //この方法で進まなくなるまで繰り返す
}

//行、列、ブロックに注目してある数の候補が特定の1マスにしかないときその数に決定する
void method2(){
    int i;
    
    for(i=0;i<9;i++){
        line(i);
        row(i);
        block(i);
    }
}

//行を見て、ある数字が入れるマスが一つならそこに入れる。
void line(int n){
    int i,N,check,finish;
    for(N=1;N<=9;N++){
        check=-1;
        finish=0;
        for(i=0;i<9;i++){
            if(num[n][i]==0&&pos[n][i][N-1]==N){
                if(check!=-1) {
                    finish=1;
                    break;
                }
                check=i;
            }
        }
        if(!finish&&check>=0){
            num[n][check]=N;
            impos2(n,check);
        }
    }
}

//列を見て、ある数字が入れるマスが一つならそこに入れる。
void row(int n){
    int i,N,check,finish;
    for(N=1;N<=9;N++){
        check=-1;
        finish=0;
        for(i=0;i<9;i++){
            if(num[i][n]==0&&pos[i][n][N-1]==N){
                if(check!=-1) {
                    finish=1;
                    break;
                }
                check=i;
            }
        }
        if(!finish&&check>=0){
            num[check][n]=N;
            impos2(check,n);
        }
    }
}

/*
ブロックを見て、ある数字が入れるマスが一つならそこに入れる。
0|1|2
-----
3|4|5
-----
6|7|8
というようにブロックの番号を付けた。
*/
void block(int n){
    int i,j,l,r,N,checki,checkj,finish;

    switch(n%3){
        case 0:
        l=n;
        r=0;
        break;
        case 1:
        l=n-1;
        r=3;
        break;
        case 2:
        l=n-2;
        r=6;
        break;
    }
    for(N=1;N<=9;N++){
        checki=-1;
        checkj=-1;
        finish=0;
        
        for(i=l;i<l+3;i++){
            for(j=r;j<r+3;j++){
                if(num[i][j]==0&&pos[i][j][N-1]==N){
                    if(checki!=-1) {
                        finish=1;
                        break;
                    }
                    checki=i;
                    checkj=j;
                }
            }
            if(finish) break;
        }
        if(!finish&&checki>=0){
            num[checki][checkj]=N;
            impos2(checki,checkj);
        }
    }
}

//網羅的探索
void give_up(){
    int i,j,N,k,l,x,y,f,complete;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(!num[i][j]){
                for(N=1;N<=9;N++){
                    if(pos[i][j][N-1]){
                        f=1;
                        for(k=0;k<9;k++){
                            if(num[i][k]==N||num[k][j]==N)
                            f=0;
                        }
                        x=ul(i,j,'i');
                        y=ul(i,j,'j');
                        for(k=x;k<x+3;k++){
                            for(l=y;l<y+3;l++){
                                if(num[k][l]==N)
                                f=0;
                            }
                        }
                        if(f){
                            complete=1;
                            num[i][j]=N;
                            for(k=0;k<9;k++){
                                for(l=0;l<9;l++){
                                    if(!num[k][l])
                                    complete=0;
                                }
                            }
                            if(complete){
                                finish=1;
                                return;
                            }
                            give_up();
                            if(finish)
                            return;
                            num[i][j]=0;
                        }
                    }
                    if(N==9){
                        return;
                    }
                }
            }
        }
    }
}
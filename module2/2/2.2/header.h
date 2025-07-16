#include <CUnit/CUnit.h>
#include <math.h>
int sum(int a, int b){
    return a+b;
}

int neg(int a, int b){
    return a-b;
}

int mult(int a, int b){
    return a*b;
}

double div(double a, double b){
    return b==0?NAN:a/b;
}

int mod(int  a, int  b){
    return b==0?NAN:a%b;
}

void test_sum(void){
    for(int i=-100;i<100;i++){
        for(int j=100;j>-100;j--){
            CU_ASSERT(sum(i, j) == i+j);
        }
    }
}
void test_neg(void){
    for(int i=-100;i<100;i++){
        for(int j=100;j>-100;j--){
            CU_ASSERT(neg(i, j) == i-j);
        }
    }
}
void test_mult(void){
    for(int i=-100;i<100;i++){
        for(int j=100;j>-100;j--){
            CU_ASSERT(mult(i, j) == i*j);
        }
    }
}
void test_div(void){
    CU_ASSERT(div(2, 3) == 2.0/3.0);
    CU_ASSERT(div(-1, 1) == -1);
    CU_ASSERT(div(-1, -1) == 1);
    CU_ASSERT(div(6, -3) == -2);
    //CU_ASSERT(div(10, 0) == NAN);
}
void test_mod(void){
    CU_ASSERT(mod(5, 3) == 2);
    CU_ASSERT(mod(7, 2) == 1);
    CU_ASSERT(mod(8, 2) == 0);
    CU_ASSERT(mod(-8, 3) == -2);
    CU_ASSERT(mod(-8, -3) == -2);
    CU_ASSERT(mod(8, -3) == 2);
}
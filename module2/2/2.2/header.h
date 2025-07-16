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
    for(double i=-100;i<100;i++){
        for(double j=100;j>-100;j--){
            if(j==0){CU_ASSERT(isnan(div(i, j)));continue;}
            CU_ASSERT(div(i, j) == i/j);
        }
    }
}
void test_mod(void){
    for(int i=-100;i<100;i++){
        for(int j=100;j>-100;j--){
            if(j==0){CU_ASSERT(mod(i, j)==0);continue;}
            CU_ASSERT(mod(i, j) == i%j);
        }
    }
}
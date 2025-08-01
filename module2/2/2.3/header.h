#include <CUnit/CUnit.h>

int sum(int a, int b){
    return a+b;
}

int neg(int a, int b){
    return a-b;
}

int mult(int a, int b){
    return a*b;
}

int div(int a, int b){
    return b==0?-1:a/b;
}

int mod(int  a, int  b){
    return b==0?-1:a%b;
}

int (*select(int choice)) (int, int) {
    switch (choice) {
    case 1: return sum;
    case 2: return neg;
    case 3: return mult;
    case 4: return div;
    case 5: return mod;
    default: return sum;
    }
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
    for(int i=-100;i<100;i++){
        for(int j=100;j>-100;j--){
            if(j==0){CU_ASSERT(div(i, j)==-1);continue;}
            CU_ASSERT(div(i, j) == i/j);
        }
    }
}
void test_mod(void){
    for(int i=-100;i<100;i++){
        for(int j=100;j>-100;j--){
            if(j==0){CU_ASSERT(mod(i, j)==-1);continue;}
            CU_ASSERT(mod(i, j) == i%j);
        }
    }
}

void test_select(void){
    CU_ASSERT(select(5) == mod);
    CU_ASSERT(select(1) == sum);
    CU_ASSERT(select(2) == neg);
    CU_ASSERT(select(3) == mult);
    CU_ASSERT(select(4) == div);
    CU_ASSERT(select(99) == sum);
}
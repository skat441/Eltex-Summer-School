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
    return a%b;
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
    CU_ASSERT(sum(2, 3) == 5);
    CU_ASSERT(sum(-1, 1) == 0);
    CU_ASSERT(sum(-1, -1) == -2);
    CU_ASSERT(sum(0, 0) == 0);
}
void test_neg(void){
    CU_ASSERT(neg(2, 3) == -1);
    CU_ASSERT(neg(-1, 1) == -2);
    CU_ASSERT(neg(-1, -1) == 0);
    CU_ASSERT(neg(2, 1) == 1);
    CU_ASSERT(neg(0, 0) == 0);
}
void test_mult(void){
    CU_ASSERT(mult(2, 3) == 6);
    CU_ASSERT(mult(-1, 1) == -1);
    CU_ASSERT(mult(-1, -1) == 1);
    CU_ASSERT(mult(0, 0) == 0);
}
void test_div(void){
    CU_ASSERT(div(2, 3) == 2/3);
    CU_ASSERT(div(-1, 1) == -1);
    CU_ASSERT(div(-1, -1) == 1);
    CU_ASSERT(div(6, -3) == -2);
    CU_ASSERT(div(10, 0) == -1);
}
void test_mod(void){
    CU_ASSERT(mod(5, 3) == 2);
    CU_ASSERT(mod(7, 2) == 1);
    CU_ASSERT(mod(8, 2) == 0);
    CU_ASSERT(mod(-8, 2) == 0);
    CU_ASSERT(mod(-8, -3) == -2);
    CU_ASSERT(mod(8, -3) == 2);
}
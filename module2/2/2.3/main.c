#include <stdio.h>
#include "header.h"
int main(){

    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("MathTestSuite", 0, 0);

    CU_add_test(suite, "test of sum()", test_sum);
    CU_add_test(suite, "test of neg()", test_neg);
    CU_add_test(suite, "test of mult()", test_mult);
    CU_add_test(suite, "test of div()", test_div);
    CU_add_test(suite, "test of mod()", test_mod);

    CU_basic_run_tests();
    CU_cleanup_registry();

    int answer=0;
    int a,b;
    int result;
    int (*operation)(int, int );
    while(1){
        printf("\n1-sum\n2-neg\n3-mult\n4-div\n5-mod\n-1-exit\nEnter option:");
        scanf("%d",&answer);
        fseek(stdin,0,SEEK_END);
        if(answer==-1)break;
        printf("Enter first operand:");
        scanf("%d",&a);
        printf("Enter second operand:");
        scanf("%d",&b);
        operation=select(answer);
        result=operation(a,b);
        printf("Result is:%d",result);
    }
    return 0;
}

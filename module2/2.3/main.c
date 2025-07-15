#include <stdio.h>
#include "header.h"
int main(){
    char answer=0;
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

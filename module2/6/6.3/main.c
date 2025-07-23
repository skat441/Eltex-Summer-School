#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
int main(){

    void *select_handle;
    void *op_handle;
    int answer=0;
    int a,b;
    int result;

    int (*(*select)(void**,int))(int, int);
    char *error;
    select_handle = dlopen ("libselect.so", RTLD_NOW);
    if (!select_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    
    select = dlsym(select_handle, "selected");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    int (*operation)(int,int);
    while(1){
        printf("\n1-sum\n2-neg\n3-mult\n4-div\n5-mod\n-1-exit\nEnter option:");
        scanf("%d",&answer);
        fseek(stdin,0,SEEK_END);
        if(answer==-1)break;
        printf("Enter first operand:");
        scanf("%d",&a);
        printf("Enter second operand:");
        scanf("%d",&b);
        operation=select(&op_handle, answer);
        result=operation(a,b);
        printf("Result is:%d",result);
    }

     dlclose(select_handle);
     dlclose(op_handle);
    return 0;
}

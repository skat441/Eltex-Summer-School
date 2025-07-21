#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
int main(){
    // void *sum_handle;
    // void *neg_handle;
    // void *mult_handle;
    // void *div_handle;
    // void *mod_handle;
    void *select_handle;
    int answer=0;
    int a,b;
    int result;
    // int (*sum)(int, int );
    // int (*neg)(int, int );
    // int (*mult)(int, int );
    // int (*div)(int, int );
    // int (*mod)(int, int );
    int (*(*select)(int))(int, int);
    char *error;
    // sum_handle = dlopen ("libsum.so", RTLD_NOW);
    // if (!sum_handle) {
    //     fputs (dlerror(), stderr);
    //     exit(1);
    // }
    // neg_handle = dlopen ("libneg.so", RTLD_NOW);
    // if (!neg_handle) {
    //     fputs (dlerror(), stderr);
    //     exit(1);
    // }
    // mult_handle = dlopen ("libmult.so", RTLD_NOW);
    // if (!mult_handle) {
    //     fputs (dlerror(), stderr);
    //     exit(1);
    // }
    // div_handle = dlopen ("libdiv.so", RTLD_NOW);
    // if (!div_handle) {
    //     fputs (dlerror(), stderr);
    //     exit(1);
    // }
    // mod_handle = dlopen ("libmod.so", RTLD_NOW);
    // if (!mod_handle) {
    //     fputs (dlerror(), stderr);
    //     exit(1);
    // }
    select_handle = dlopen ("libselect.so", RTLD_NOW);
    if (!select_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    // sum = dlsym(sum_handle, "sum");
    // if ((error = dlerror()) != NULL) {
    //     fprintf (stderr, "%s\n", error);
    //     exit(1);
    // }
    // neg = dlsym(neg_handle, "neg");
    // if ((error = dlerror()) != NULL) {
    //     fprintf (stderr, "%s\n", error);
    //     exit(1);
    // }
    // mult = dlsym(mult_handle, "mult");
    // if ((error = dlerror()) != NULL) {
    //     fprintf (stderr, "%s\n", error);
    //     exit(1);
    // }
    // div = dlsym(div_handle, "div");
    // if ((error = dlerror()) != NULL) {
    //     fprintf (stderr, "%s\n", error);
    //     exit(1);
    // }
    // mod = dlsym(mod_handle, "mod");
    // if ((error = dlerror()) != NULL) {
    //     fprintf (stderr, "%s\n", error);
    //     exit(1);
    // }
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
        operation=select(answer);
        result=operation(a,b);
        printf("Result is:%d",result);
    }
    // dlclose(sum_handle);
    // dlclose(neg_handle);
    // dlclose(mult_handle);
    // dlclose(div_handle);
    // dlclose(mod_handle);
    // dlclose(select_handle);
    return 0;
}

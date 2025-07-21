#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int (*selected(int choice)) (int, int) {
    void *sum_handle;
    void *neg_handle;
    void *mult_handle;
    void *div_handle;
    void *mod_handle;
    int (*sum)(int, int );
    int (*neg)(int, int );
    int (*mult)(int, int );
    int (*div)(int, int );
    int (*mod)(int, int );
    char *error;
    sum_handle = dlopen ("libsum.so", RTLD_NOW);
    if (!sum_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    neg_handle = dlopen ("libneg.so", RTLD_NOW);
    if (!neg_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    mult_handle = dlopen ("libmult.so", RTLD_NOW);
    if (!mult_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    div_handle = dlopen ("libdiv.so", RTLD_NOW);
    if (!div_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    mod_handle = dlopen ("libmod.so", RTLD_NOW);
    if (!mod_handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    sum = dlsym(sum_handle, "sum");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    neg = dlsym(neg_handle, "neg");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    mult = dlsym(mult_handle, "mult");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    div = dlsym(div_handle, "div");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    mod = dlsym(mod_handle, "mod");
    if ((error = dlerror()) != NULL) {
        fprintf (stderr, "%s\n", error);
        exit(1);
    }
    switch (choice) {
    case 1: return sum;
    case 2: return neg;
    case 3: return mult;
    case 4: return div;
    case 5: return mod;
    default: return sum;
    }
    dlclose(sum_handle);
    dlclose(neg_handle);
    dlclose(mult_handle);
    dlclose(div_handle);
    dlclose(mod_handle);
}
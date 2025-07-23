#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int (*selected(void**handle, int choice)) (int, int) {
    int (*sum)(int, int );
    int (*neg)(int, int );
    int (*mult)(int, int );
    int (*div)(int, int );
    int (*mod)(int, int );
    char *error;
    switch (choice) {
    case 1: 
        *handle = dlopen ("libsum.so", RTLD_NOW);
        if (!*handle) {
            fputs (dlerror(), stderr);
            exit(1);
        }
        sum = dlsym(*handle, "sum");
        if ((error = dlerror()) != NULL) {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        return sum;
    case 2: 
        *handle = dlopen ("libneg.so", RTLD_NOW);
        if (!*handle) {
            fputs (dlerror(), stderr);
            exit(1);
        }
        neg = dlsym(*handle, "neg");
        if ((error = dlerror()) != NULL) {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        return neg;
    case 3: 
        *handle = dlopen ("libmult.so", RTLD_NOW);
        if (!*handle) {
            fputs (dlerror(), stderr);
            exit(1);
        }
        mult = dlsym(*handle, "mult");
        if ((error = dlerror()) != NULL) {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        return mult;
    case 4: 
        *handle = dlopen ("libdiv.so", RTLD_NOW);
        if (!*handle) {
            fputs (dlerror(), stderr);
            exit(1);
        }
        div = dlsym(*handle, "div");
        if ((error = dlerror()) != NULL) {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        return div;
    case 5: 
        *handle = dlopen ("libmod.so", RTLD_NOW);
        if (!*handle) {
            fputs (dlerror(), stderr);
            exit(1);
        }
        mod = dlsym(*handle, "mod");
        if ((error = dlerror()) != NULL) {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        return mod;
    default: 
        *handle = dlopen ("libsum.so", RTLD_NOW);
        if (!*handle) {
            fputs (dlerror(), stderr);
            exit(1);
        }
        sum = dlsym(*handle, "sum");
        if ((error = dlerror()) != NULL) {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        return sum;
    }
}
#include "sum.c"
#include "neg.c"
#include "mult.c"
#include "div.c"
#include "mod.c"

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
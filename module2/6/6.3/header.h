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
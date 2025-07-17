#include <stdio.h>
#include "header.h"

int main(){
    List* queue=createList();
    srand ( time(NULL) );
    for(int i=0;i<100;i++)addValue(queue,i);
    viewList(queue);
    printf("\n\n%d\n",getFirstValue(queue));
    printf("%d\n",getValueWithPriority(queue,26));
    printf("%d\n\n",getValueWithMorePriority(queue,15));
    viewList(queue);
    return 0;
}
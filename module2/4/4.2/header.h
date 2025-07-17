#include <stdlib.h>
#include <time.h>

typedef struct Node{
    unsigned int value;
    int priority;
    struct Node* nextP;
}Node;
typedef struct List{
    Node* head;
    Node* last;
}List;

Node* createNode(unsigned int val){
    Node* newNode=malloc(sizeof(Node));
    newNode->value=val;
    //srand ( time(NULL) ); 
    newNode->priority=rand() % 256;
    newNode->nextP=NULL;
    return newNode;
}
List* createList(){
    List* newList=malloc(sizeof(List));
    newList->head=NULL;
    newList->last=NULL;
}

void addValue(List* list, int val){
    if(list->head==NULL){//no elements in list
        Node* newNode=createNode(val);
        list->head=newNode;
        list->last=newNode;
    }
    else if(list->head==list->last){//one element in list
        Node* newNode=createNode(val);
        if(list->head->priority<newNode->priority){//priorty less 
            list->head->nextP=newNode;
            list->last=newNode;
        }
        else{
            list->head=newNode;
            newNode->nextP=list->last;
        }
    }
    else{
        Node* newNode=createNode(val);
        Node* iNode=list->head;
        Node* tmpNode=NULL;
        for(;iNode!=NULL && iNode->priority < newNode->priority;){tmpNode=iNode;iNode=iNode->nextP;}
        if(iNode==NULL){//new el is last
            list->last->nextP=newNode;
            list->last=newNode;
        }
        else if(iNode==list->head){//new element is first
            newNode->nextP=list->head;
            list->head=newNode;
        }
        else{//new el is between two el
            tmpNode->nextP=newNode;
            newNode->nextP=iNode;
        }
    }
}

unsigned int getFirstValue(List* list){
    if(list->head==NULL)return -1;
    else if(list->head->nextP==NULL){//one element in list
        unsigned int val=list->head->value;
        free(list->head);
        list->head=NULL;
        list->last=NULL;
        return val;
    }
    else{//more then one element in list
        unsigned int val=list->head->value;
        Node* tmpNode=list->head->nextP;
        free(list->head);
        list->head=tmpNode;
        if(list->head->nextP==NULL)list->last=list->head;//if only one element remain after release
        return val;
    }
}

void deleteValueFromQueue(List* list,unsigned int val,int priority){
    Node* iNode=list->head;
    for(;iNode!=NULL && iNode->priority!=priority && iNode->value!=val;iNode=iNode->nextP);
    if(iNode==NULL)return;//no such element
    else if(list->head==list->last){//only one element
        free(iNode);
        list->head=NULL;
        list->last=NULL;
    }
    else if(list->head==iNode){
        list->head=iNode->nextP;
        free(iNode);
    }
    else if(list->last==iNode){
        Node* jNode=list->head;
        for(;jNode!=NULL && jNode->nextP!=iNode;jNode=jNode->nextP);
        list->last=jNode;
        jNode->nextP=NULL;
        free(iNode);
    }
    else{
        Node* jNode=list->head;
        for(;jNode!=NULL && jNode->nextP!=iNode;jNode=jNode->nextP);
        jNode->nextP=iNode->nextP;
        free(iNode);
    }
}

unsigned int getValueWithPriority(List* list, unsigned int priority){
    Node* iNode=list->head;
    for(;iNode!=NULL && iNode->priority!=priority;iNode=iNode->nextP);
    if(iNode==NULL)return -1;
    unsigned int val=iNode->value;
    deleteValueFromQueue(list,val,iNode->priority);
    return val;
}

unsigned int getValueWithMorePriority(List* list, int priority){
    Node* iNode=list->head;
    Node* resNode=NULL;
    int priorityDiff=-255;
    for(;iNode!=NULL;iNode=iNode->nextP){
        if((iNode->priority-priority)>priorityDiff && (iNode->priority-priority<=0)){
            priorityDiff=iNode->priority-priority;
        }
        if(priorityDiff==0)break;
    }
    if(priorityDiff==-255)return -1;
    unsigned int val=getValueWithPriority(list,priorityDiff+priority);
    return val;
}

void viewList(List* list){
    if(list->head==NULL){printf("Empty queue");return;}
    for(Node* iNode=list->head;iNode!=NULL;iNode=iNode->nextP){printf("val:%d priority:%d\n",iNode->value,iNode->priority);}
}
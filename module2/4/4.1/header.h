#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <CUnit/CUnit.h>
typedef struct Date
{
    char day;
    char month;
    short year;
}Date;

typedef struct Person
{
    int id;
    char* FirstName;
    char* LastName;
    char ExtraInformation;
    long long PhoneNumber;
    char* Email;
    Date BirthDate;
}Person;

typedef struct Node{
    Person* p;
    struct Node* nextP;
    struct Node* prevP;
}Node;

Person* PersonInit(int ID, char* firstname,char* lastname,char isExtraInf, ...){
    Person* newp=malloc(sizeof(Person));
    int fsize=strlen(firstname);
    int lsize=strlen(lastname);
    char* newFname=malloc(fsize+1);
    char* newLname=malloc(lsize+1);
    strncpy(newFname,firstname,fsize);
    strncpy(newLname,lastname,lsize);
    newFname[fsize]=0;
    newLname[lsize]=0;
    newp->id=ID;
    newp->FirstName=newFname;
    newp->LastName=newLname;
    if(isExtraInf){
        va_list args;
        va_start(args,isExtraInf);
        long long phonenumber=va_arg(args,long long);
        char* email=va_arg(args, char*);
        Date date=va_arg(args,Date);
        newp->BirthDate=date;
        newp->PhoneNumber=phonenumber;
        if(email!=NULL){
            int esize=strlen(email);
            char* newEmail=malloc(esize);
            strncpy(newEmail,email,esize);
            newp->Email=newEmail;
        }
        else{
            newp->Email=NULL;
        }
        va_end(args);
    }
    else{
        newp->ExtraInformation=0;
        newp->PhoneNumber=0;
        newp->Email=NULL;
        newp->BirthDate.day=0;
        newp->BirthDate.month=0;
        newp->BirthDate.year=0;
    }
    return newp;
}

void* PersonFree(Person* p){
    free(p->FirstName);
    free(p->LastName);
    if(p->Email!=NULL)free(p->Email);
    free(p);
    return NULL;
}

// Person** AddContact(Person** phoneBook, int* size, Person* newP){
//     (*size)++;
//     Person** newPhoneBook=malloc(sizeof(Person*) * (*size));
//     for(int i=0;i<((*size)-1);i++){
//         newPhoneBook[i]=phoneBook[i];
//     }
//     newPhoneBook[((*size)-1)]=newP;
//     free(phoneBook);
//     return newPhoneBook;
// }

Node* getINodeByIndex(Node* head, int index){
    Node* iNode=head;
    for(int i=0;i<index && iNode!=NULL;i++){
        iNode=iNode->nextP;
    }
    return iNode;
}

Node* getINodeById(Node* head, int id){
    Node* iNode=head;
    for(;iNode!=NULL && iNode->p->id!=id;){
        iNode=iNode->nextP;
    }
    return iNode;
}


int insertPerson(Node** head, Person* newP){
    if(newP==NULL)return 0;
    if((*head)==NULL){//empty list
        Node* newEl=malloc(sizeof(Node));
        newEl->p=newP;
        newEl->nextP=NULL;
        newEl->prevP=NULL;
        (*head)=newEl;
        return 1;
    }
    else if((*head)->nextP==NULL){//One element in list
        Node* newEl=malloc(sizeof(Node));
        newEl->p=newP;
        if(strcmp(newP->FirstName,(*head)->p->FirstName) > 0){//add to end of list
            newEl->prevP=(*head);
            newEl->nextP=NULL;
            (*head)->nextP=newEl;
            return 1;
        }
        else{//add to begining of list
            newEl->nextP=(*head);
            newEl->prevP=NULL;
            (*head)->prevP=newEl;
            (*head)->nextP=NULL;
            (*head)=newEl;
            return 1;
        }
    }
    else{//elements more then one
        Node* iNode=(*head);
        Node* tmpNode=NULL;
        for(;iNode!=NULL && (strcmp(newP->FirstName,iNode->p->FirstName) > 0);){tmpNode=iNode;iNode=iNode->nextP;}
        if(iNode==NULL){//new Element is last, last element is tmpNode
            Node* newEl=malloc(sizeof(Node));
            newEl->p=newP;
            newEl->nextP=NULL;
            newEl->prevP=tmpNode;
            tmpNode->nextP=newEl;
            return 1;
        }
        else if(iNode==(*head)){//new element is first
            Node* newEl=malloc(sizeof(Node));
            newEl->p=newP;
            newEl->prevP=NULL;
            newEl->nextP=(*head);
            (*head)->prevP=newEl;
            (*head)=newEl;
            return 1;
        }
        else{//new element is between elements, left element is tmpNode, right element is iNode
            Node* newEl=malloc(sizeof(Node));
            newEl->p=newP;
            newEl->nextP=iNode;
            newEl->prevP=tmpNode;
            tmpNode->nextP=newEl;
            iNode->prevP=newEl;
            return 1;
        }
    }
    return 0;
}

void viewList(Node* head){
    if(head==NULL){printf("Phonebook is empty\n");return;}
    for(Node* iNode=head;iNode!=NULL;iNode=iNode->nextP){
        if(iNode->p->ExtraInformation==0){
            printf("id:%d\tfirstname:%s lastname:%s\n",iNode->p->id,iNode->p->FirstName,iNode->p->LastName);
        }
        else{
            printf("id:%d\tfirstname:%s lastname:%s phonenumber:%llu E-mail:%s bithdate:%d.%d.%d\n",
            iNode->p->id,iNode->p->FirstName,iNode->p->LastName, iNode->p->PhoneNumber,iNode->p->Email,
            iNode->p->BirthDate.day, iNode->p->BirthDate.month, iNode->p->BirthDate.year);
        }
    }
}

// void viewPhoneBook(Person** phoneBook, int size){
//     if((size)==0){printf("Phonebook is empty\n");return;}
//     for(int i=0;i<size;i++){
//         if(phoneBook[i]->ExtraInformation==0){
//             printf("id:%d\tfirstname:%s lastname:%s\n",phoneBook[i]->id,phoneBook[i]->FirstName,phoneBook[i]->LastName);
//         }
//         else{
//             printf("id:%d\tfirstname:%s lastname:%s phonenumber:%llu E-mail:%s bithdate:%d.%d.%d\n",
//             phoneBook[i]->id,phoneBook[i]->FirstName,phoneBook[i]->LastName, phoneBook[i]->PhoneNumber,phoneBook[i]->Email,
//              phoneBook[i]->BirthDate.day, phoneBook[i]->BirthDate.month, phoneBook[i]->BirthDate.year);
//         }
//     }
// }

// Person** deleteContact(Person** phoneBook,int* size,int ID){
//     (*size)--;
//     Person** newPhoneBook=malloc(sizeof(Person*) * (*size));
//     int j=0;
//     for(int i=0;i<((*size)+1);i++){
//         if(phoneBook[i]->id!=ID){newPhoneBook[j]=phoneBook[i];j++;}
//         else{phoneBook[i]=PersonFree(phoneBook[i]);}
//     }
//     free(phoneBook);
//     return newPhoneBook;
// }
// int isContain(Person** phoneBook,int* size,int ID){
//     int isfound=0;
//     for(int i=0;i<(*size);i++){
//         if(phoneBook[i]->id==ID){
//             isfound=1;
//         }
//     }
//     return isfound;
// }
// void changePerson(Person** phoneBook,int size, int ID, char fieldToChange, ...){
//     int pos;
//     for(int i=0;i<(size);i++){
//         if(phoneBook[i]->id==ID){
//             pos=i;
//         }
//     }
//     va_list args;
//     va_start(args,fieldToChange);
//     switch (fieldToChange)
//     {
//     case 1:
//         char* Fname=va_arg(args,char*);
//         char* newFname=malloc(strlen(Fname)+1);
//         strncpy(newFname,Fname,strlen(Fname)+1);
//         newFname[strlen(Fname)]=0;
//         free(phoneBook[pos]->FirstName);
//         phoneBook[pos]->FirstName=newFname;
//         break;
//     case 2:
//         char* Lname=va_arg(args,char*);
//         char* newLname=malloc(strlen(Lname)+1);
//         strncpy(newLname,Lname,strlen(Lname)+1);
//         newLname[strlen(Lname)]=0;
//         free(phoneBook[pos]->LastName);
//         phoneBook[pos]->LastName=newLname;
//         break;
//     case 3:
//         int ShowExtraInfo=va_arg(args,int);
//         phoneBook[pos]->ExtraInformation=ShowExtraInfo;
//         break;
//     case 4:
//         long long newphonenumber=va_arg(args,long long);
//         phoneBook[pos]->PhoneNumber=newphonenumber;
//         break;
//     case 5:
//         char* email=va_arg(args,char*);
//         char* newemail=malloc(strlen(email)+1);
//         strncpy(newemail,email,strlen(email)+1);
//         newemail[strlen(email)]=0;
//         free(phoneBook[pos]->Email);
//         phoneBook[pos]->Email=newemail;
//         break;
//     case 6:
//         Date newDate=va_arg(args,Date);
//         phoneBook[pos]->BirthDate.day=newDate.day;
//         phoneBook[pos]->BirthDate.month=newDate.month;
//         phoneBook[pos]->BirthDate.year=newDate.year;
//         break;
//     default:
//         break;
//     }
// }

void* clearPhoneBook(Person** phoneBook,int size){
    for(int i=0;i<(size);i++){
        PersonFree(phoneBook[i]);
    }
    free(phoneBook);
    return NULL;
}

Node* clearList(Node* head){
    if(head==NULL)return NULL;
    for(Node* iNode=head;iNode!=NULL;){
        PersonFree(iNode->p);
        Node* tmp=iNode->nextP;
        free(iNode);
        iNode=tmp;
    }
    return NULL;
}

int deletePerson(Node** head, int id){
    if((*head)==NULL)return 0;
    Node* delNode=getINodeById((*head), id);
    if(delNode==NULL)return 0;
    else{
        Node* rightNode=delNode->nextP;
        Node* leftNode=delNode->prevP;
        if(rightNode==leftNode){//only one element in list
            PersonFree(delNode->p);
            free (delNode);
            (*head)=NULL;
            return 1;
        }
        else if(delNode==(*head)){//del element is head(first)
            PersonFree((*head)->p);
            (*head)->nextP->prevP=NULL;
            (*head)=(*head)->nextP;
            free(delNode);
            return 1;
        }
        else if(delNode->nextP==NULL){//del element is last
            delNode->prevP->nextP=NULL;
            PersonFree(delNode->p);
            free(delNode);
            return 1;
        }
        else{//del element is between left and right el
            leftNode->nextP=rightNode;
            rightNode->prevP=leftNode;
            PersonFree(delNode->p);
            free(delNode);
            return 1;
        }
    }
    return 0;
}

int isContainList(Node* head, int id){
    for(Node* iNode=head;iNode!=NULL;iNode=iNode->nextP){
        if(iNode->p->id==id)return 1;
    }
    return 0;
}

int changePersonList(Node* head, int ID, char fieldToChange, ...){
    int pos;
    Node* changeNode=getINodeById(head,ID);
    if (changeNode==NULL)return 0;
    va_list args;
    va_start(args,fieldToChange);
    switch (fieldToChange)
    {
    case 1:
        char* Fname=va_arg(args,char*);
        char* newFname=malloc(strlen(Fname)+1);
        strncpy(newFname,Fname,strlen(Fname)+1);
        newFname[strlen(Fname)]=0;
        free(changeNode->p->FirstName);
        changeNode->p->FirstName=newFname;
        return 1;
        break;
    case 2:
        char* Lname=va_arg(args,char*);
        char* newLname=malloc(strlen(Lname)+1);
        strncpy(newLname,Lname,strlen(Lname)+1);
        newLname[strlen(Lname)]=0;
        free(changeNode->p->LastName);
        changeNode->p->LastName=newLname;
        return 1;
        break;
    case 3:
        int ShowExtraInfo=va_arg(args,int);
        changeNode->p->ExtraInformation=ShowExtraInfo;
        return 1;
        break;
    case 4:
        long long newphonenumber=va_arg(args,long long);
        changeNode->p->PhoneNumber=newphonenumber;
        return 1;
        break;
    case 5:
        char* email=va_arg(args,char*);
        char* newemail=malloc(strlen(email)+1);
        strncpy(newemail,email,strlen(email)+1);
        newemail[strlen(email)]=0;
        free(changeNode->p->Email);
        changeNode->p->Email=newemail;
        return 1;
        break;
    case 6:
        Date newDate=va_arg(args,Date);
        changeNode->p->BirthDate.day=newDate.day;
        changeNode->p->BirthDate.month=newDate.month;
        changeNode->p->BirthDate.year=newDate.year;
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

//Testing part
char *rndstr(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    char *randomString = NULL;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (int n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

Node* test_phoneBook=NULL;
int test_phoneBookSize=0;

void test_add_contact(void){
    printf("Test adding to phonebook:\n");
    for(int i=1;i<200;i++){
        Person* newP = PersonInit(i,rndstr(10),rndstr(10),0);
        CU_ASSERT(insertPerson(&test_phoneBook,newP)==1);printf("added id %d\n",i);
    }
    viewList(test_phoneBook);
}
void test_chg_contact(void){
    int id_to_change=0;
    int change_option=0;
    for(int i=0;i<600;i++){
        id_to_change=rand()%199+1;
        change_option=rand()%6+1;
        switch (change_option)
        {
        case 1:
            CU_ASSERT(changePersonList(test_phoneBook,id_to_change,change_option,rndstr(10))==1);
            break;
        case 2:
            CU_ASSERT(changePersonList(test_phoneBook,id_to_change,change_option,rndstr(10))==1);
            break;
        case 3:
            CU_ASSERT(changePersonList(test_phoneBook,id_to_change,change_option,rand()%2)==1);
            break;
        case 4:
            CU_ASSERT(changePersonList(test_phoneBook,id_to_change,change_option,rand())==1);
            break;
        case 5:
            CU_ASSERT(changePersonList(test_phoneBook,id_to_change,change_option,rndstr(20))==1);
            break;
        case 6:
            Date newDate;
            newDate.day=rand()%30+1;
            newDate.month=rand()%12+1;
            newDate.year=rand()%40+1980;
            CU_ASSERT(changePersonList(test_phoneBook,id_to_change,change_option,newDate)==1);
            break;
        default:
            break;
        }
    }
    viewList(test_phoneBook);
}
void test_del_contact(void){
    printf("Test delete operation to phonebook:\n");
    int id_to_del=rand()%10;
    for(int i=0;i<300;i++){
        id_to_del=rand()%200+1;
        printf("id to del is:%d\n",id_to_del);
        if(isContainList(test_phoneBook,id_to_del)){//if id contains in phonebook, delete operation will send 1
            printf("\tdelete id:%d\n",id_to_del);
            CU_ASSERT(deletePerson(&test_phoneBook,id_to_del)==1);
        }
        else{
            CU_ASSERT(deletePerson(&test_phoneBook,id_to_del)==0);
        }
    }
    test_phoneBook=clearList(test_phoneBook);
}
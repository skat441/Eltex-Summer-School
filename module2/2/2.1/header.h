#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <CUnit/CUnit.h>
#include <time.h>
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

int isContain(Person** phoneBook,int* size,int ID){
    int isfound=0;
    for(int i=0;i<(*size);i++){
        if(phoneBook[i]->id==ID){
            isfound=1;
        }
    }
    return isfound;
}

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

int AddContact(Person*** phoneBook, int* size, Person* newP){
    if(isContain(*phoneBook,size,newP->id))return 0;
    (*size)++;
    Person** newPhoneBook=malloc(sizeof(Person*) * (*size));
    for(int i=0;i<((*size)-1);i++){
        newPhoneBook[i]=(*phoneBook)[i];
    }
    newPhoneBook[((*size)-1)]=newP;
    //free(phoneBook);
    *phoneBook=newPhoneBook;
    //printf("%p\n",phoneBook);
    return 1;
}

void viewPhoneBook(Person** phoneBook, int size){
    if((size)==0){printf("Phonebook is empty\n");return;}
    //printf("%p",phoneBook);
    for(int i=0;i<size;i++){
        if(phoneBook[i]->ExtraInformation==0){
            printf("id:%d\tfirstname:%s lastname:%s\n",phoneBook[i]->id,phoneBook[i]->FirstName,phoneBook[i]->LastName);
        }
        else{
            printf("id:%d\tfirstname:%s lastname:%s phonenumber:%llu E-mail:%s bithdate:%d.%d.%d\n",
            phoneBook[i]->id,phoneBook[i]->FirstName,phoneBook[i]->LastName, phoneBook[i]->PhoneNumber,phoneBook[i]->Email,
             phoneBook[i]->BirthDate.day, phoneBook[i]->BirthDate.month, phoneBook[i]->BirthDate.year);
        }
    }
}

int deleteContact(Person*** phoneBook,int* size,int ID){
    if(!isContain(*phoneBook,size,ID))return 0;
    (*size)--;
    Person** newPhoneBook=malloc(sizeof(Person*) * (*size));
    int j=0;
    for(int i=0;i<((*size)+1);i++){
        if(((*phoneBook)[i])->id!=ID){newPhoneBook[j]=(*phoneBook)[i];j++;}
        else{(*phoneBook)[i]=PersonFree((*phoneBook)[i]);}
    }
    //free(*phoneBook);
    *phoneBook=newPhoneBook;
    return 1;
}
int changePerson(Person** phoneBook,int size, int ID, char fieldToChange, ...){
    int pos=-1;
    for(int i=0;i<(size);i++){
        if(phoneBook[i]->id==ID){
            pos=i;
        }
    }
    if (pos==-1)return 0;
    va_list args;
    va_start(args,fieldToChange);
    switch (fieldToChange)
    {
    case 1:
        char* Fname=va_arg(args,char*);
        char* newFname=malloc(strlen(Fname)+1);
        strncpy(newFname,Fname,strlen(Fname)+1);
        newFname[strlen(Fname)]=0;
        free(phoneBook[pos]->FirstName);
        phoneBook[pos]->FirstName=newFname;
        return 1;
        break;
    case 2:
        char* Lname=va_arg(args,char*);
        char* newLname=malloc(strlen(Lname)+1);
        strncpy(newLname,Lname,strlen(Lname)+1);
        newLname[strlen(Lname)]=0;
        free(phoneBook[pos]->LastName);
        phoneBook[pos]->LastName=newLname;
        return 1;
        break;
    case 3:
        int ShowExtraInfo=va_arg(args,int);
        phoneBook[pos]->ExtraInformation=ShowExtraInfo;
        return 1;
        break;
    case 4:
        long long newphonenumber=va_arg(args,long long);
        phoneBook[pos]->PhoneNumber=newphonenumber;
        return 1;
        break;
    case 5:
        char* email=va_arg(args,char*);
        char* newemail=malloc(strlen(email)+1);
        strncpy(newemail,email,strlen(email)+1);
        newemail[strlen(email)]=0;
        free(phoneBook[pos]->Email);
        phoneBook[pos]->Email=newemail;
        return 1;
        break;
    case 6:
        Date newDate=va_arg(args,Date);
        phoneBook[pos]->BirthDate.day=newDate.day;
        phoneBook[pos]->BirthDate.month=newDate.month;
        phoneBook[pos]->BirthDate.year=newDate.year;
        return 1;
        break;
    default:
        return 0;
        break;
    }
    // Person* newP=PersonInit(ID,firstname,lastname,0);
    // phoneBook[pos]=PersonFree(phoneBook[pos]);
    // phoneBook[pos]=newP;
}
void* clearPhoneBook(Person** phoneBook,int size){
    for(int i=0;i<(size);i++){
        PersonFree(phoneBook[i]);
    }
    free(phoneBook);
    return NULL;
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

Person** test_phoneBook=NULL;
int test_phoneBookSize=0;

void test_add_contact(void){
    printf("Test adding to phonebook:\n");
    for(int i=1;i<30;i++){
        Person* newP = PersonInit(i,rndstr(10),rndstr(10),0);
        CU_ASSERT(AddContact(&test_phoneBook,&test_phoneBookSize,newP)==1);printf("added id %d\n",i);
    }
    viewPhoneBook(test_phoneBook,test_phoneBookSize);
}
void test_del_contact(void){
    printf("Test delete operation to phonebook:\n");
    int id_to_del=rand()%10;
    for(int i=0;i<300;i++){
        id_to_del=rand()%29+1;
        printf("id to del is:%d\n",id_to_del);
        if(isContain(test_phoneBook,&test_phoneBookSize,id_to_del)){//if id contains in phonebook, delete operation will send 1
            printf("\tdelete id:%d\n",id_to_del);
            CU_ASSERT(deleteContact(&test_phoneBook,&test_phoneBookSize,id_to_del)==1);
        }
        else{
            CU_ASSERT(deleteContact(&test_phoneBook,&test_phoneBookSize,id_to_del)==0);
        }
    }
    clearPhoneBook(test_phoneBook,test_phoneBookSize);
}
void test_chg_contact(void){
    int id_to_change=0;
    int change_option=0;
    for(int i=0;i<300;i++){
        id_to_change=rand()%29+1;
        change_option=rand()%6+1;
        switch (change_option)
        {
        case 1:
            CU_ASSERT(changePerson(test_phoneBook,test_phoneBookSize,id_to_change,change_option,rndstr(10))==1);
            break;
        case 2:
            CU_ASSERT(changePerson(test_phoneBook,test_phoneBookSize,id_to_change,change_option,rndstr(10))==1);
            break;
        case 3:
            CU_ASSERT(changePerson(test_phoneBook,test_phoneBookSize,id_to_change,change_option,rand()%2)==1);
            break;
        case 4:
            CU_ASSERT(changePerson(test_phoneBook,test_phoneBookSize,id_to_change,change_option,rand())==1);
            break;
        case 5:
            CU_ASSERT(changePerson(test_phoneBook,test_phoneBookSize,id_to_change,change_option,rndstr(20))==1);
            break;
        case 6:
            Date newDate;
            newDate.day=rand()%30+1;
            newDate.month=rand()%12+1;
            newDate.year=rand()%40+1980;
            CU_ASSERT(changePerson(test_phoneBook,test_phoneBookSize,id_to_change,change_option,newDate)==1);
            break;
        default:
            break;
        }
    }
    viewPhoneBook(test_phoneBook,test_phoneBookSize);
}
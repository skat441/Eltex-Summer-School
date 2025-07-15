#include <stdio.h>
#include <CUnit/CUnit.h>
#include "header.h"

int main(){
    Person** phoneBook=malloc(sizeof(Person*));
    int id=1;
    int ID;
    char isfound;
    char answer=0;
    char buffer[100]={0};
    char fname[100]={0};
    char lname[100]={0};
    char nul[100]={0};
    int phoneBookSize=0;
    while(answer!=-1){
        strncpy(buffer,nul,100);
        strncpy(fname,nul,100);
        strncpy(lname,nul,100);
        printf("\n0-Add Contact to Phonebook\n1-Delete Contact by id\n2-Change Person by id\n3-View Phonebook\n-1-exit\nEnter option:");
        scanf("%d",&answer);
        fseek(stdin,0,SEEK_END);
        switch (answer)
        {
        case 0:
            Person* newP;
            printf("Enter Person Firstname:");
            scanf("%s",buffer);
            strncpy(fname,buffer,strlen(buffer));
            strncpy(buffer,nul,100);
            printf("Enter Person Lastname:");
            scanf("%s",buffer);
            strncpy(lname,buffer,strlen(buffer));
            printf("ExtraInfo?[y/n]:");
            scanf("%s",buffer);
            if(buffer[0]=='y'){
                long long phonenumber=0;
                char email[100]={0};
                Date date;
                printf("Enter Person phonenumber:");
                scanf("%llu",&phonenumber);

                printf("Enter Person email:");
                scanf("%s",buffer);
                strncpy(email,buffer,strlen(buffer));
                strncpy(buffer,nul,100);

                printf("Enter Person birth day:");
                scanf("%d",&date.day);
                printf("Enter Person birth month:");
                scanf("%d",&date.month);
                printf("Enter Person birth year:");
                scanf("%d",&date.year);
                newP = PersonInit(id,fname,lname,1,phonenumber,email,date);
            }
            else{
            newP = PersonInit(id,fname,lname,0);
            }
            id++;
            //printf("%d",phoneBookSize);
            phoneBook=AddContact(phoneBook,&phoneBookSize,newP);
            //printf("%d",phoneBookSize);
            break;
        case 1:
            ID=0;
            isfound=0;
            printf("Enter person id:");
            scanf("%d",&ID);
            fseek(stdin,0,SEEK_END);
            //
            isfound=isContain(phoneBook,&phoneBookSize,ID);
            //
            if(!isfound){
                printf("\nPerson with id=%d not found!!!\n",ID);
            }
            else{
                //
                phoneBook=deleteContact(phoneBook,&phoneBookSize,ID);
                //
                printf("\nPerson with id=%d deleted!!!\n",ID);
            }
            break;
        case 2:
            ID=0;
            isfound=0;
            printf("Enter person id:");
            scanf("%d",&ID);
            fseek(stdin,0,SEEK_END);
            //
            isfound=isContain(phoneBook,&phoneBookSize,ID);
            //
            if(!isfound){
                printf("\nPerson with id=%d not found!!!\n",ID);
            }
            else{
                int feildToChange=0;
                printf("1-Firstname\n2-Lastname\n3-ShowExtraInfo\n4-Phonenumber\n5-Email\n6-Birthdate\nEnter field number to change:");
                scanf("%d",&feildToChange);
                fseek(stdin,0,SEEK_END);
                switch (feildToChange)
                {
                case 1://change FirstName
                    printf("Enter Person Firstname:");
                    scanf("%s",buffer);
                    strncpy(fname,buffer,strlen(buffer));
                    changePerson(phoneBook,phoneBookSize,ID,1,fname);
                    break;
                case 2:
                    printf("Enter Person Lastname:");
                    scanf("%s",buffer);
                    strncpy(lname,buffer,strlen(buffer));
                    changePerson(phoneBook,phoneBookSize,ID,2,lname);
                    break;
                case 3:
                    int newvalue;
                    printf("ExtraInfo?[1/0]:");
                    scanf("%d",&newvalue);
                    changePerson(phoneBook,phoneBookSize,ID,3,newvalue);
                    break;
                case 4:
                    long long newphone=0;
                    printf("newphonenumber:");
                    scanf("%llu",&newphone);
                    changePerson(phoneBook,phoneBookSize,ID,4,newphone);
                    break;
                case 5:
                    char newEmail[100];
                    printf("Enter Person email:");
                    scanf("%s",buffer);
                    strncpy(newEmail,buffer,strlen(buffer));
                    changePerson(phoneBook,phoneBookSize,ID,5,newEmail);
                    break;
                case 6:
                    Date date;
                    printf("Enter Person birth day:");
                    scanf("%d",&date.day);
                    printf("Enter Person birth month:");
                    scanf("%d",&date.month);
                    printf("Enter Person birth year:");
                    scanf("%d",&date.year);
                    changePerson(phoneBook,phoneBookSize,ID,6,date);
                    break;
                default:
                    break;
                }
                printf("\nPerson with id=%d changed!!!\n",ID);
            }
            break;
        case 3:
            //
            viewPhoneBook(phoneBook,phoneBookSize);
            //
            break;
        default:
            answer=-1;
            //
            phoneBook=clearPhoneBook(phoneBook,phoneBookSize);
            //
        }
    }
    return 1;
}

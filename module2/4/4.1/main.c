#include <stdio.h>
#include "header.h"
#include <time.h>

int main(){
    // Person** phoneBook=malloc(sizeof(Person*));
    // Person* newPerson1 = PersonInit(1,"Ford","Harrison",0);
    // Person* newPerson2 = PersonInit(0,"Gleg","Harrison",0);
    // Person* newPerson3 = PersonInit(2,"Blob","Harrison",0);   
    Node* head=NULL;
    // head=insertPerson(head,newPerson1);
    // head=insertPerson(head,newPerson2);
    // head=insertPerson(head,newPerson3);
    // viewList(head);
    // head=deletePerson(head,2);
    // head=deletePerson(head,0);
    // changePersonList(head,1,1,"Greg");
    // viewList(head);3
    // head=clearList(head);
    // viewList(head);
    int id=1;

    srand(time(NULL));
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("ListTestSuite", 0, 0);

    CU_add_test(suite, "test of add()", test_add_contact);
    CU_add_test(suite, "test of chg()", test_chg_contact);
    CU_add_test(suite, "test of del()", test_del_contact);

    CU_basic_run_tests();
    CU_cleanup_registry();

    Person* newPerson1 = PersonInit(id,"Ford","Harrison",0);
    id++;
    Person* newPerson2 = PersonInit(id,"Harry","Potter",0);
    id++;
    Person* newPerson3 = PersonInit(id,"Josh","Woods",0);
    id++;
    insertPerson(&head,newPerson1);
    insertPerson(&head,newPerson2);
    insertPerson(&head,newPerson3);
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
            printf("%d\n",insertPerson(&head,newP));
            //printf("%d",phoneBookSize);
            break;
        case 1:
            ID=0;
            isfound=0;
            printf("Enter person id:");
            scanf("%d",&ID);
            fseek(stdin,0,SEEK_END);
            //
            isfound=isContainList(head,ID);
            //
            if(!isfound){
                printf("\nPerson with id=%d not found!!!\n",ID);
            }
            else{
                //
                printf("%d",deletePerson(&head,ID));
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
            isfound=isContainList(head,ID);
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
                    printf("%d",changePersonList(head,ID,1,fname));
                    break;
                case 2:
                    printf("Enter Person Lastname:");
                    scanf("%s",buffer);
                    strncpy(lname,buffer,strlen(buffer));
                    printf("%d",changePersonList(head,ID,2,lname));
                    break;
                case 3:
                    int newvalue;
                    printf("ExtraInfo?[1/0]:");
                    scanf("%d",&newvalue);
                    printf("%d",changePersonList(head,ID,3,newvalue));
                    break;
                case 4:
                    long long newphone=0;
                    printf("newphonenumber:");
                    scanf("%llu",&newphone);
                    printf("%d",changePersonList(head,ID,4,newphone));
                    break;
                case 5:
                    char newEmail[100];
                    printf("Enter Person email:");
                    scanf("%s",buffer);
                    strncpy(newEmail,buffer,strlen(buffer));
                    printf("%d",changePersonList(head,ID,5,newEmail));
                    break;
                case 6:
                    Date date;
                    printf("Enter Person birth day:");
                    scanf("%d",&date.day);
                    printf("Enter Person birth month:");
                    scanf("%d",&date.month);
                    printf("Enter Person birth year:");
                    scanf("%d",&date.year);
                    printf("%d",changePersonList(head,ID,6,date));
                    break;
                default:
                    break;
                }
                //printf("\nPerson with id=%d changed!!!\n",ID);
            }
            break;
        case 3:
            //
            viewList(head);
            //
            break;
        default:
            answer=-1;
            //
            head=clearList(head);
            //
        }
    }
    return 1;
}
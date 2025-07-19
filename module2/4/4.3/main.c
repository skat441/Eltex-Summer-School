#include <stdio.h>
#include "header.h"

int main(){
    int id=1;
    Tree* tree=createTree();
    // Person* newPerson1 = PersonInit(0,"Ford","Harrison",0);
    // Person* newPerson2 = PersonInit(1,"Gleg","Harrison",0);
    // Person* newPerson3 = PersonInit(2,"Blob","Harrison",0);
    // Person* newPerson4 = PersonInit(3,"Alex","Harrison",0);
    // Person* newPerson5 = PersonInit(4,"Nord","Harrison",0);
    // Person* newPerson6 = PersonInit(5,"Josh","Harrison",0);
    // Person* newPerson7 = PersonInit(6,"Willy","Harrison",0);
    // Person* newPerson8 = PersonInit(7,"Billy","Harrison",0);
    for(int i=0;i<15;i++){
         Person* newPers = PersonInit(i,"Alex","Harrison",0);
         insertPerson(tree,newPers);
    }
    // insertPerson(tree,newPerson1);
    // insertPerson(tree,newPerson2);
    // insertPerson(tree,newPerson3);
    // printTreeIDs(tree);
    // insertPerson(tree,newPerson4);
    // printTreeIDs(tree);
    // insertPerson(tree,newPerson5);
    // printTreeIDs(tree);
    // insertPerson(tree,newPerson6);
    // printTreeIDs(tree);
    // insertPerson(tree,newPerson7);
    // printTreeIDs(tree);
    // insertPerson(tree,newPerson8);
    // printTreeIDs(tree);
    // deletePerson(tree,3);
    // printTreeIDs(tree);
    // insertPerson(tree,newPerson8);
    printTreeIDs(tree);
    printf("\n\n%d %d",tree->leftHeight, tree->rightHeight);
    int ID;
    id=15;
    char answer=0;
    char buffer[100]={0};
    char fname[100]={0};
    char lname[100]={0};
    char nul[100]={0};
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
            printf("%d",insertPerson(tree,newP));
            //printf("%d",phoneBookSize);
            break;
        case 1:
            ID=0;
            printf("Enter person id:");
            scanf("%d",&ID);
            fseek(stdin,0,SEEK_END);
            //
            // isfound=getNodeWithId(tree->root,ID);
            // //
            // if(!isfound){
            //     printf("\nPerson with id=%d not found!!!\n",ID);
            // }
            // else{
                //
                printf("%d",deletePerson(tree,ID));
                //
                // printf("\nPerson with id=%d deleted!!!\n",ID);
            // }
            break;
        case 2:
            ID=0;
            printf("Enter person id:");
            scanf("%d",&ID);
            fseek(stdin,0,SEEK_END);
            //
            // isfound=getNodeWithId(tree->root,ID);
            // //
            // if(!isfound){
            //     printf("\nPerson with id=%d not found!!!\n",ID);
            // }
            // else{
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
                    printf("%d",changePersonList(tree->root,ID,1,fname));
                    break;
                case 2:
                    printf("Enter Person Lastname:");
                    scanf("%s",buffer);
                    strncpy(lname,buffer,strlen(buffer));
                    printf("%d",changePersonList(tree->root,ID,2,lname));
                    break;
                case 3:
                    int newvalue;
                    printf("ExtraInfo?[1/0]:");
                    scanf("%d",&newvalue);
                    printf("%d",changePersonList(tree->root,ID,3,newvalue));
                    break;
                case 4:
                    long long newphone=0;
                    printf("newphonenumber:");
                    scanf("%llu",&newphone);
                    printf("%d",changePersonList(tree->root,ID,4,newphone));
                    break;
                case 5:
                    char newEmail[100];
                    printf("Enter Person email:");
                    scanf("%s",buffer);
                    strncpy(newEmail,buffer,strlen(buffer));
                    printf("%d",changePersonList(tree->root,ID,5,newEmail));
                    break;
                case 6:
                    Date date;
                    printf("Enter Person birth day:");
                    scanf("%d",&date.day);
                    printf("Enter Person birth month:");
                    scanf("%d",&date.month);
                    printf("Enter Person birth year:");
                    scanf("%d",&date.year);
                    printf("%d",changePersonList(tree->root,ID,6,date));
                    break;
                default:
                    break;
                }
                //printf("\nPerson with id=%d changed!!!\n",ID);
            //}
            break;
        case 3:
            //
            printTreeIDs(tree);
            //
            break;
        default:
            answer=-1;
            //
            //head=clearList(head);
            //
        }
    }
    return 1;
}
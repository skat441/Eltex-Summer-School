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
    struct Node* left;
    struct Node* right;
    int height;
}Node;

typedef struct Tree{
    int leftHeight;
    int rightHeight;
    Node* root;
}Tree;

Tree* createTree(){
    Tree* newTree=malloc(sizeof(Tree));
    newTree->leftHeight=0;
    newTree->rightHeight=0;
    newTree->root=NULL;
    return newTree;
}


int maxI(int a,int b) {return ((a)>(b)) ? a : b;}

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

Node* createNewNode(Person* newP){
    Node* newNode=malloc(sizeof(Node));
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->p=newP;
    return newNode;
}

void recursiveInsert(Node* node, Person* newP){
    if(node->p->id < newP->id){
        if(node->right!=NULL){recursiveInsert(node->right,newP);return;}
        Node* newNode=createNewNode(newP);
        newNode->height=node->height+1;
        node->right=newNode;
    }
    else if(node->p->id > newP->id){
        if(node->left!=NULL){recursiveInsert(node->left,newP);return;}
        Node* newNode=createNewNode(newP);
        newNode->height=node->height+1;
        node->left=newNode;
    }
}

Node* getNodeWithId(Node* node,int id){
    if(node==NULL)return NULL;
    Node* leftNode=getNodeWithId(node->left,id);
    Node* rightNode=getNodeWithId(node->right,id);
    if(id==node->p->id)return node;
    if(leftNode!=NULL)return leftNode;
    if(rightNode!=NULL)return rightNode;
    else{return NULL;}
}


int calculateMaxHeight(Node* node) {
    if (node == NULL) {
        return 0;
    }
    int leftHeight = calculateMaxHeight(node->left);
    int rightHeight = calculateMaxHeight(node->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// int calculateMinHeight(Node* node) {
//     if (node == NULL) {
//         return 0;
//     }
//     int leftHeight = calculateMinHeight(node->left);
//     int rightHeight = calculateMinHeight(node->right);
//     return (leftHeight < rightHeight ? leftHeight : rightHeight) + 1;
// }

int checkBalance(Node* node){
    if (node==NULL)return 1;
    if(abs(calculateMaxHeight(node->left)-calculateMaxHeight(node->right))>1)return 0;
    int leftBalance=checkBalance(node->left);
    int rightBalance=checkBalance(node->right);
    if(leftBalance==0 || rightBalance==0)return 0;
    return 1;
}

void recalcHeight(Node* node, int height){
    node->height=height;
    if(node->left!=NULL)recalcHeight(node->left,height+1);
    if(node->right!=NULL)recalcHeight(node->right,height+1);
}

Node* rebalanceTree(Node* node){
    if(node==NULL || (node->right==NULL && node->left==NULL))return node;
    node->left=rebalanceTree(node->left);
    node->right=rebalanceTree(node->right);
    int max_balance=calculateMaxHeight(node->left)-calculateMaxHeight(node->right);
    // int min_l_balance=calculateMinHeight(node->left)-calculateMaxHeight(node->right);
    // int min_r_balance=calculateMaxHeight(node->left)-calculateMinHeight(node->right);
    // printf("__________________\n");
    // show(node,0);
    if(max_balance>1){
        
        if( calculateMaxHeight(node->left->left)<calculateMaxHeight(node->left->right)){
            printf("LR\n");
            Node* tmp=node;
            Node* tmpbottom=tmp->left->right;
            Node* tmpmid=tmp->left;
            tmp->left=tmpbottom->right;
            tmpmid->right=tmpbottom->left;
            tmpbottom->left=tmpmid;
            tmpbottom->right=tmp;
            node=tmpbottom;
        }
        else if(calculateMaxHeight(node->left->left)>=calculateMaxHeight(node->left->right)){
            printf("RR\n");
            Node* tmp=node;
            Node* tmpmid=node->left;
            tmp->left=tmpmid->right;
            tmpmid->right=tmp;
            node=tmpmid;
        }
    }
    else if(max_balance<-1){

         if(calculateMaxHeight(node->right->left)>calculateMaxHeight(node->right->right)){
            printf("RL\n");
            Node* tmp=node;
            Node* tmpbottom=tmp->right->left;
            Node* tmpmid=tmp->right;
            tmp->right=tmpbottom->left;
            tmpmid->left=tmpbottom->right;
            tmpbottom->left=tmp;
            tmpbottom->right=tmpmid;
            node=tmpbottom;
        }
        else if(calculateMaxHeight(node->right->left)<=calculateMaxHeight(node->right->right)){
            printf("LL\n");
            Node* tmp=node;
            Node* tmpmid=node->right;
            tmp->right=tmpmid->left;
            tmpmid->left=tmp;
            node=tmpmid;
        }
    }
    return node;
}

int insertPerson(Tree* tree, Person* newP){
    //printf("\n\nInsert person with id:%d\n", newP->id);
    if(getNodeWithId(tree->root,newP->id)!=NULL)return 0;
    if(tree->root==NULL){
        Node* newNode=createNewNode(newP);
        newNode->height=0;
        tree->root=newNode;
        return 1;
    }
    recursiveInsert(tree->root,newP);
    tree->leftHeight = calculateMaxHeight(tree->root->left);
    tree->rightHeight = calculateMaxHeight(tree->root->right);
    if(!checkBalance(tree->root))tree->root=rebalanceTree(tree->root);
    recalcHeight(tree->root,0);
    return 1;
}


void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("ID: %d Height: %d   firstname:%s lastname:%s\n", root->p->id, root->height,root->p->FirstName, root->p->LastName);
        inOrderTraversal(root->right);
    }
}

// Pre-order traversal to print IDs
void preOrderTraversal(Node* root) {
    if (root != NULL) {
        printf("ID: %d Height: %d   firstname:%s lastname:%s\n", root->p->id, root->height,root->p->FirstName, root->p->LastName);
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
}

// Post-order traversal to print IDs
void postOrderTraversal(Node* root) {
    if (root != NULL) {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        printf("ID: %d Height: %d   firstname:%s lastname:%s\n", root->p->id, root->height,root->p->FirstName, root->p->LastName);
    }
}

// Function to print the IDs of all persons in the tree
void printTreeIDs(Tree* tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("The tree is empty.\n");
        return;
    }

    printf("\nPre-order Traversal:\n");
    preOrderTraversal(tree->root);

    printf("\nIn-order Traversal:\n");
    inOrderTraversal(tree->root);

    printf("\nPost-order Traversal:\n");
    postOrderTraversal(tree->root);
}

void show(Node* node, int level){
    if (node == NULL)return;
    show(node->right, level + 1);
        for (int i = 0; i < level * 3; i++)printf(" ");
        printf("%d\n", node->p->id);
        show(node->left, level + 1);
}

void Show(Tree* tree){
    show(tree->root,0);
}

Node* getParent(Node* node,int id){
    if(node==NULL || (node->right==NULL && node->left==NULL))return NULL;
    Node* leftNode=getParent(node->left,id);
    Node* rightNode=getParent(node->right,id);
    if(node->left!=NULL)if(id==node->left->p->id)return node;
    if(node->right!=NULL)if(id==node->right->p->id)return node;
    if(leftNode!=NULL)return leftNode;
    if(rightNode!=NULL)return rightNode;
    else{return NULL;}
}

int deletePerson(Tree* tree, int id){
    //printf("\n\nDelete person with id:%d\n",id);
    int isDeleted=0;
    printf("check root and existence\n");
    if(tree->root==NULL || getNodeWithId(tree->root,id)==NULL){
        return 0;
    }
    else if(tree->root->right==NULL && tree->root->left==NULL && id==tree->root->p->id){//delete root 0 child
        printf("delete root 0 child\n");
        free(tree->root);
        tree->root=NULL;
        tree->leftHeight=0;
        tree->rightHeight=0;
        return 1;
    }
    else if(getNodeWithId(tree->root, id)==tree->root){//delete root with change
        printf("delete root with change\n");
        if(tree->root->right!=NULL){//2 child
            Node* tmpRoot = tree->root;
            tree->root=tree->root->right;
            tree->root->left=tmpRoot->left;
            free(tmpRoot);
            isDeleted=1;
        }
        else if(tree->root->left!=NULL){//1 child
            Node* tmpRoot = tree->root;
            tree->root=tree->root->left;
            free(tmpRoot);
            isDeleted=1;
        }
        tree->root->height=0;
    }
    else{
        printf("else\n");
        Node* delNode=getNodeWithId(tree->root, id);
        if(delNode==NULL)return 0;
        printf("get delNode:%p id:%d",delNode,delNode->p->id);
        if(delNode!=NULL){
            if(delNode->left==NULL && delNode->right==NULL){//leaf delete
                Node* tmpParent=getParent(tree->root,id);
                printf("get tmpParent:%p id:%d",tmpParent,tmpParent->p->id);
                if(tmpParent->left==delNode){//leaf is left 
                    free(delNode);
                    tmpParent->left=NULL;
                }
                else if(tmpParent->right==delNode){//leaf is right 
                    free(delNode);
                    tmpParent->right=NULL;
                }
                isDeleted=1;
            }
            else if( (delNode->right==NULL && delNode->left!=NULL)||(delNode->right!=NULL && delNode->left==NULL) ){//only 1 child
                Node* tmpParent=getParent(tree->root,id);
                if(tmpParent->left==delNode){//leaf is left 
                    if(delNode->right!=NULL)tmpParent->left=delNode->right;
                    if(delNode->left!=NULL)tmpParent->left=delNode->left;
                    free(delNode);
                }
                else if(tmpParent->right==delNode){//leaf is left 
                    if(delNode->right!=NULL)tmpParent->right=delNode->right;
                    if(delNode->left!=NULL)tmpParent->right=delNode->left;
                    free(delNode);
                }
                isDeleted=1;
            }
            else if(delNode->right!=NULL && delNode->left!=NULL){//delNode has 2 child
                Node* tmpParent=getParent(tree->root,id);
                //printf("\n\n%d",tmpParent);
                if(tmpParent->left==delNode){//delNode is left
                    tmpParent->left=delNode->right;
                    tmpParent->left->left=delNode->left;
                    free(delNode);
                }
                else if(tmpParent->right==delNode){//delNode is right
                    tmpParent->right=delNode->right;
                    tmpParent->right->left=delNode->left;
                    free(delNode);
                }
                isDeleted=1;
            }
        }
    }
    tree->leftHeight = calculateMaxHeight(tree->root->left);
    tree->rightHeight = calculateMaxHeight(tree->root->right);
    if(!checkBalance(tree->root))tree->root=rebalanceTree(tree->root);
    recalcHeight(tree->root,0);
    return isDeleted;
}

int changePersonList(Node* head, int ID, char fieldToChange, ...){
    Node* changeNode=getNodeWithId(head,ID);
    if(changeNode==NULL)return 0;
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

void clearTree(Node* node){
    if(node==NULL)return;
    clearTree(node->left);
    clearTree(node->right);
    PersonFree(node->p);
    free(node);
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

void test_add_contact(void){
    Tree* tree=createTree();
    printf("Test adding to phonebook:\n");
    for(int i=1;i<200;i++){
        Person* newP = PersonInit(i,rndstr(10),rndstr(10),0);
        CU_ASSERT(insertPerson(tree,newP)==1);printf("added id %d\n",i);
    }
    printTreeIDs(tree);
    clearTree(tree->root);
}
void test_chg_contact(void){
    Tree* tree=createTree();
    printf("Test adding to phonebook:\n");
    for(int i=1;i<200;i++){
        Person* newP = PersonInit(i,rndstr(10),rndstr(10),0);
        CU_ASSERT(insertPerson(tree,newP)==1);printf("added id %d\n",i);
    }
    printTreeIDs(tree);
    int id_to_change=0;
    int change_option=0;
    for(int i=0;i<600;i++){
        id_to_change=rand()%199+1;
        change_option=rand()%6+1;
        switch (change_option)
        {
        case 1:
            CU_ASSERT(changePersonList(tree->root,id_to_change,change_option,rndstr(10))==1);
            break;
        case 2:
            CU_ASSERT(changePersonList(tree->root,id_to_change,change_option,rndstr(10))==1);
            break;
        case 3:
            CU_ASSERT(changePersonList(tree->root,id_to_change,change_option,rand()%2)==1);
            break;
        case 4:
            CU_ASSERT(changePersonList(tree->root,id_to_change,change_option,rand())==1);
            break;
        case 5:
            CU_ASSERT(changePersonList(tree->root,id_to_change,change_option,rndstr(20))==1);
            break;
        case 6:
            Date newDate;
            newDate.day=rand()%30+1;
            newDate.month=rand()%12+1;
            newDate.year=rand()%40+1980;
            CU_ASSERT(changePersonList(tree->root,id_to_change,change_option,newDate)==1);
            break;
        default:
            break;
        }
    }
    printTreeIDs(tree);
    clearTree(tree->root);
}
void test_del_contact(void){
    printf("Test delete operation to phonebook:\n");
    Tree* tree=createTree();
    printf("Test adding to phonebook:\n");
    for(int i=1;i<200;i++){
        Person* newP = PersonInit(i,rndstr(10),rndstr(10),0);
        CU_ASSERT(insertPerson(tree,newP)==1);printf("added id %d\n",i);
    }
    printTreeIDs(tree);
    int id_to_del=rand()%10;
    for(int i=0;i<300;i++){
        id_to_del=rand()%200+1;
        printf("id to del is:%d\n",id_to_del);
        if(getNodeWithId(tree->root,id_to_del)){//if id contains in phonebook, delete operation will send 1
            printf("\tdelete id:%d\n",id_to_del);
            CU_ASSERT(deletePerson(tree,id_to_del)==1);
        }
        else{
            CU_ASSERT(deletePerson(tree,id_to_del)==0);
        }
    }
    clearTree(tree->root);
}
void test_add_n_del(void){
    Tree* tree=createTree();
    for(int i=1;i<2000;i++){
        if(rand()%2==0){//try to add
            int id_to_add=rand()%1000;
            if(getNodeWithId(tree->root,id_to_add)==NULL){//added contact isn't exist
                Person* newP = PersonInit(id_to_add,rndstr(10),rndstr(10),0);
                CU_ASSERT(insertPerson(tree,newP)==1);printf("added id %d\n",i);
            }
            else{//try to add already exist id
                Person* newP = PersonInit(id_to_add,rndstr(10),rndstr(10),0);
                CU_ASSERT(insertPerson(tree,newP)==0);printf("failed to add id %d\n",i);
            }
        }
        else{//try to del
            int id_to_del=rand()%1000;
            if(getNodeWithId(tree->root,id_to_del)!=NULL){//if id contains in phonebook, delete operation will send 1
                printf("\tdelete id:%d\n",id_to_del);
                CU_ASSERT(deletePerson(tree,id_to_del)==1);
            }
            else{
                printf("\tfailed to delete id:%d\n",id_to_del);
                CU_ASSERT(deletePerson(tree,id_to_del)==0);
            }
        }
    }
    Show(tree);
    clearTree(tree->root);
    //test_phoneBook=clearList(test_phoneBook);
}
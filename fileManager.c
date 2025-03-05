#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
  int element;
  struct Node *left;
  struct Node *right;
  int height;
}Node;

int getHeight(Node *root){
  if(root==NULL)
    return 0;
  return root->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

Node *createNode(int element){
  Node *newNode = (Node *) malloc (sizeof(Node));
  newNode->element=element;
  newNode->left=NULL;
  newNode->right=NULL;
  newNode->height=1;
  return newNode;
}

int getBalanceFactor(Node *root){
  if(root==NULL)
    return 0;
  return getHeight(root->left) - getHeight(root->right);
}

Node *rightRotate(Node *y){
  Node *x=y->left;
  Node *T2=x->right;

  x->right=y;
  y->left=T2;

  y->height=max(getHeight(y->right),getHeight(y->left)) +1;
  x->height=max(getHeight(x->right),getHeight(x->left)) +1;

  return x;
}

Node *leftRotate(Node *x){
  Node *y=x->right;
  Node *T2=y->left;

  y->left=x;
  x->right=T2;

  x->height=max(getHeight(x->right),getHeight(x->left)) +1;
  y->height=max(getHeight(y->right),getHeight(y->left)) +1;

  return y;
}

void in_order(Node *root){
  if(root!=NULL){
    in_order(root->left);
    printf("%d ",root->element);
    in_order(root->right);
  }
}

Node *insert(Node *root,int element){
  if(root==NULL){
    return createNode(element);
  }
  else if(element<root->element){
    root->left=insert(root->left,element);
  }
  else{
    root->right=insert(root->right,element);
  }
  
  root->height=max(getHeight(root->right),getHeight(root->left)) +1;
  int bf=getBalanceFactor(root);

  if(bf>1 && element<root->left->element){
    return rightRotate(root);
  }

  if(bf<-1 && element>root->right->element){
    return leftRotate(root);
  }

  if(bf>1 && element>root->left->element){
    root->left=leftRotate(root->left);
    return rightRotate(root);
  }

  if(bf<-1 && element<root->right->element){
    root->right=rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

Node *findMin(Node *root){
  while(root->left!=NULL)
    root=root->left;
  return root;
}

Node *delete(Node *root,int element){
  Node *temp;
  if (root == NULL)
    return root;
  else if (element < root->element)
    root->left = delete(root->left, element);
  else if (element > root->element)
    root->right = delete(root->right, element);
    else{
      if(root->left==NULL){
        temp=root->right;
        if(temp){
          free(root);
          return temp;
        }
      }
      else if(root->right==NULL){
        temp=root->left;
        if(temp){
          free(root);
          return temp;
        }
      }
      else{
        temp=findMin(root->right);
        root->element=temp->element;
        root->right=delete(root->right,temp->element);
      }
    }
}

Node *search(Node *root,int element){
  if(root==NULL || root->element==element)
    return root;
  else if(element<root->element)
    return search(root->left,element);
  else
    return search(root->right,element);
}

int main(){
  Node *root=NULL,*temp=NULL;
  int choice,element;

  while(1){
    printf("1.INSERT\n2.DELETE\n3.SEARCH\n4.SORT\n");
    printf("Enter your choice:");
    scanf("%d",&choice);
    switch (choice)
    {
      case 1:
        printf("Enter the element to be inserted:");
        scanf("%d",&element);
        root=insert(root,element);
        printf("%d inserted\n",element);
        break;
        
      case 2:
        printf("Enter the element to be deleted:");
        scanf("%d",&element);
        root=delete(root,element);
        printf("%d deleted\n",element);
        break;

      case 3:
        printf("Enter the element to be searched:");
        scanf("%d",&element);
        temp=search(root,element);
        if(temp!=NULL){
          printf("%d found\n",temp->element);
        }
        else{
          printf("Element not found\n");
        }
        break;

      case 4:
        pre_order(root);
        printf("\n");
        break;

      default:
        printf("Invalid choice. Try again.\n");
    }
  }
}
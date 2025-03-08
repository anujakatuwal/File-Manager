#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>

#define MAX 300
typedef struct Node{
  char filename[MAX];
  struct Node *left;
  struct Node *right;
  int height;
}Node;

Node *createNode(char *filename){
  Node *newNode = (Node *) malloc (sizeof(Node));
  strcpy(newNode->filename,filename);
  newNode->left=NULL;
  newNode->right=NULL;
  newNode->height=1;
  return newNode;
}

int getHeight(Node *root){
  if(root==NULL)
    return 0;
  return root->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
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

Node *insert(Node *root,char *filename){
  if(root==NULL){
    return createNode(filename);
  } 

  if (strcmp(filename,root->filename)==0){
    return root;
  }
  else if (strcmp(filename,root->filename)<0){
    root->left=insert(root->left,filename);
  }
  else{
    root->right=insert(root->right,filename);
  }
  
  root->height=max(getHeight(root->right),getHeight(root->left)) +1;
  int bf=getBalanceFactor(root);

  if(bf>1 && strcmp(filename,root->left->filename)<0){
    return rightRotate(root);
  }

  if(bf<-1 && strcmp(filename,root->right->filename)>0){
    return leftRotate(root);
  }

  if(bf>1 && strcmp(filename,root->left->filename)>0){
    root->left=leftRotate(root->left);
    return rightRotate(root);
  }

  if(bf<-1 && strcmp(filename,root->right->filename)<0){
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

Node *delete(Node *root,char *filename){
  Node *temp;
  if (root == NULL)
    return root;
  else if (strcmp(filename, root->filename)<0)
    root->left = delete(root->left, filename);
  else if (strcmp(filename, root->filename)>0)
    root->right = delete(root->right, filename);
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
      strcpy(root->filename,temp->filename);
      root->right=delete(root->right,temp->filename);
    }
  }
}

Node *search(Node *root,char *filename){
  if(root==NULL || strcmp(root->filename,filename)==0)
    return root;
  else if (strcmp(filename,root->filename)<0)
    return search(root->left,filename);
  else
    return search(root->right,filename);
}
  
void in_order(Node *root){
  if(root!=NULL){
    in_order(root->left);
    printf("%s\n",root->filename);
    in_order(root->right);
  }
}

Node *scanDirectory(Node *root,char *dirName){
  struct dirent *entry;
  DIR *directory=opendir(dirName);

  if(directory==NULL){
    printf("Could not open directory: %s\n", dirName);
    printf("Please enter a valid directory path: ");
    scanf("%s", dirName);
    directory = opendir(dirName);
  }

  printf("You are inside %s\n",dirName);
  while((entry=readdir(directory)) !=NULL){
    if(entry->d_name[0] != '.'){
      root=insert(root,entry->d_name);
    }
  }

  closedir(directory);
  return root;
}

int main(){
  Node *root=NULL,*temp=NULL;
  int choice;
  char filename[MAX];
  char dirName[MAX];
  char path[MAX];
  printf("Enter directory path:");
  scanf("%s",dirName);
  root=scanDirectory(root,dirName);

  while(1){
    printf("\n1.CREATE FILE\n2.DELETE FILE\n3.SEARCH FILE\n4.DISPLAY ALL FILES\n5.EXIT\n");
    printf("Enter your choice:");
    scanf("%d",&choice);
    switch (choice)
    {
      case 1:
        printf("Enter the filename to be created:");
        scanf("%s", filename);
        snprintf(path, MAX, "%s\\%s", dirName, filename);

        while (access(path, F_OK) == 0) { 
            printf("%s already exists. Please enter a new filename.\n", filename);
            scanf("%s", filename); 
            snprintf(path, MAX, "%s\\%s", dirName, filename);  
        }

        FILE *file = fopen(path, "w");
        if (file) {
            fclose(file);
            root = insert(root, filename);
            printf("%s created in %s\n", filename, dirName);
        } else {
            printf("Failed to create file: %s\n", path);
        }
        break;

      case 2:
        printf("Enter the filename to be deleted:");
        scanf("%s",filename);
        snprintf(path, MAX, "%s\\%s", dirName, filename);

        if (remove(path) == 0) {
            root = delete(root, filename);
            printf("%s deleted from %s\n", filename,dirName);
        } else {
            printf("File not found or deletion failed.\n");
        }
        break;

      case 3:
        printf("Enter the filename to be searched:");
        scanf("%s",filename);
        temp=search(root,filename);
        if(temp!=NULL){
          printf("%s found\n",temp->filename);
        }
        else{
          printf("File not found\n");
        }
        break;

      case 4:
        printf("\nFiles in sorted order:\n");
        in_order(root);
        break;

      case 5:
        printf("Exiting program...\n");
        return 0;

      default:
        printf("Invalid choice. Try again.\n");
    }
  }
}
#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

struct node {
    int data;
    struct node* next;

};
int  Member(int value, struct node* head);
int  Insert(int value, struct node** head);
int  Delete(int value, struct node** head);
int  Is_Empty(struct node* head);
void Clear_Memory(struct node** head);


int n = 1000;	//Initial population element count
int m = 10000;	//Total random operations count

//These values must be changed according to each case
float mMember = 0.50;	//Fraction of Member 
float mInsert = 0.25;	//Fraction of Insert
float mDelete = 0.25;	//Fraction of Delete

float operation_choice;	
double start_time, finish_time, time_elapsed;

int main(void) {
	int j=0;
	for(j=0; j < 250; j ++){
	struct node* head = NULL;  
   	int i=0;

	//Inserting elements to linked-list
    for(;i<n;i++){   
        int r = rand()%65536;
        if(!Insert(r,&head)){
            i--;
        }
    }
   
    int member_count=0;	//Member function call count
    int insert_count=0;	//Insert function call count
    int delete_count=0;	//Delete function call count
         
    start_time = clock();    
    for(i=0;i<m;i++){
        operation_choice = (rand()%10000/10000.0); 
        int r = rand()%65536;
        if(operation_choice < mMember){
            Member(r,head);
            member_count++;
        }else if(operation_choice < mMember + mInsert ){
            Insert(r,&head);
            insert_count++;
        }else{           
            Delete(r,&head);
            delete_count++;
        }
    }
    finish_time = clock();
    time_elapsed = (finish_time - start_time)/CLOCKS_PER_SEC;
	//printf("Time Elapsed  = %.10f seconds\n", time_elapsed);	
    printf("%.10f\n", time_elapsed);
     
	Clear_Memory(&head);
	}
	
	
	return 0;
}   


 
int Member(int value, struct node *head_p)
{
    struct node *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        curr_p = curr_p->next;
    }
    if(curr_p==NULL || curr_p->data>value){
        return 0;
    }
    else{
        return 1;
    }
}

int Insert(int value, struct node **head_pp)
{
    struct node *curr_p = *head_pp;
    struct node *pred_p = NULL;
    struct node *temp_p;

    while (curr_p != NULL && curr_p->data < value){
        pred_p=curr_p;
        curr_p=curr_p->next;

    }
    
    if(curr_p==NULL || curr_p->data>value){
        temp_p=malloc(sizeof(struct node));
        temp_p->data=value;
        temp_p->next=curr_p;

        if(pred_p==NULL){
            *head_pp=temp_p;

        }
        else{
            pred_p->next=temp_p;
            return 1;
        }
    }
    else{
        return 0;
    }
}


int Delete (int value, struct node** head_pp) {
    struct node* curr_p= *head_pp;
    struct node* pred_p=NULL;

    while(curr_p != NULL && curr_p->data < value){
        pred_p=curr_p;
        curr_p=curr_p->next;
    }

    if(curr_p != NULL && curr_p->data==value){
        if(pred_p==NULL){
            *head_pp=curr_p->next;
            free(curr_p);
        }
        else{
            pred_p->next=curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else{
        return 0;
    }

}


//Function to free memory used for linked-list
void Clear_Memory(struct node** headp) {
	struct node* current;
	struct node* next;

	if (Is_Empty(*headp)) return;
	current = *headp; 
	next = current->next;
	while(next != NULL) {
		free(current);
		current = next;
		next = current->next;
	}
	free(current);
	*headp = NULL;
}

//Function to check if linked-list is empty
int  Is_Empty(struct node* head) {
	if (head == NULL){
      return 1;
	}else{
      return 0;
	}
}







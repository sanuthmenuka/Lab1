#include <stddef.h>
#include "list_node.h"
#include <stdlib.h>
#include <stdio.h>

int Member(int value, struct list_node_s *head_p)
{
    struct list_node_s *curr_p = head_p;

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

int Insert(int value, struct list_node_s **head_pp)
{
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;

    while (curr_p != NULL && curr_p->data < value){
        pred_p=curr_p;
        curr_p=curr_p->next;

    }
    
    if(curr_p==NULL || curr_p->data>value){
        temp_p=malloc(sizeof(struct list_node_s));
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


int Delete (int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p= *head_pp;
    struct list_node_s* pred_p=NULL;

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









// void PrintList(struct list_node_s *head_p)
// {
//     struct list_node_s *curr_p = head_p;
//     while (curr_p != NULL)
//     {
//         printf("%d -> ", curr_p->data);
//         curr_p = curr_p->next;
//     }
//     printf("NULL\n");
// }

// int main()
// {
//     struct list_node_s *head = NULL; // Initialize the list as empty

//     // Test Insert function
//     printf("Inserting values into the list...\n");
//     Insert(10, &head);
//     Insert(20, &head);
//     Insert(15, &head); // Insert between 10 and 20
//     Insert(5, &head);  // Insert at the beginning

//     printf("List after insertions: ");
//     PrintList(head);

//     // Test Member function
//     printf("Checking membership in the list:\n");
//     printf("Is 10 in the list? %s\n", Member(10, head) ? "Yes" : "No");
//     printf("Is 15 in the list? %s\n", Member(15, head) ? "Yes" : "No");
//     printf("Is 25 in the list? %s\n", Member(25, head) ? "Yes" : "No");

//     return 0;
// }
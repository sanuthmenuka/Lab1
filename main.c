#include <stddef.h>
#include "list_node.h"
#include "write_op.h"
#include <stdlib.h>
#include <stdio.h>
#include "C:\msys64\ucrt64\include\pthread.h"

#include "serial.h"

pthread_mutex_t linkedListLock;
pthread_rwlock_t rwlock;
int n = 10;
int m = 10000;

int operationIndex = 0;
pthread_mutex_t operationIndexLock;

#define CASE_NUM 1

#if CASE_NUM==1
    double mMember = 0.99;
    double mInsert = 0.005;
    double mDelete = 0.005;
#elif CASE_NUM==2
    double mMember = 0.90;
    double mInsert = 0.05;
    double mDelete = 0.05;
#else
    double mMember = 0.5;
    double mInsert = 0.25;
    double mDelete = 0.25;
#endif

long threadCount = 5;

struct list_node_s *head = NULL;
struct write_op_s *writeOps = NULL;

void PrintList(struct list_node_s *head_p)
{
    struct list_node_s *curr_p = head_p;
    while (curr_p != NULL)
    {
        printf("%d -> ", curr_p->data);
        curr_p = curr_p->next;
    }
    printf("NULL\n");
}

int is_unique(int arr[], int size, int num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return 0;
        }
    }
    return 1;
}

void generate_unique_random_numbers(int arr[], int n, int range) {
    int count = 0;
    while (count < n) {
        int random_value = rand() % range;
        if (is_unique(arr, count, random_value)) {
            arr[count] = random_value;
            count++;
        }
    }
}

void populateList(){
    int range = (1 << 16) - 1;
    int arr[n];

    srand(time(NULL));
    
    generate_unique_random_numbers(arr, n, range);

    for(long i=0;i<n;++i){
        Insert(arr[i], &head);
    }
}

void generateOperations(int operations[m][2]){
    int range = (1 << 16) - 1;

    int memberOperationCount = (int) m*mMember;
    int insertOperationCount = (int) m*mInsert;
    int deleteOperationCount = (int) m*mDelete;
    for(int i=0;i<m;++i){
        int opType = rand() % 3;
        if(opType==0){
            if(memberOperationCount>0){
                operations[m][0] = 0;
                memberOperationCount--;
            }else if (insertOperationCount>0){
                operations[m][0] = 1;
                insertOperationCount--;
            }else{
                operations[m][0] = 2;
                deleteOperationCount--;
            }
        }else if(opType==1){
            if (insertOperationCount>0){
                operations[m][0] = 1;
                insertOperationCount--;
            }else if(deleteOperationCount>0){
                operations[m][0] = 2;
                deleteOperationCount--;
            }else{
                operations[m][0] = 0;
                memberOperationCount--;
            } 
        }else{
            if(deleteOperationCount>0){
                operations[m][0] = 2;
                deleteOperationCount--;
            }else if(memberOperationCount>0){
                operations[m][0] = 0;
                memberOperationCount--;
            }else if (insertOperationCount>0){
                operations[m][0] = 1;
                insertOperationCount--;
            } 
        }
        operations[m][1] = rand() % range;
    }
}

void* operationMutex(void* arg) {
    int (*operations)[2] = (int (*)[2]) arg;
    while (operationIndex<m){
        pthread_mutex_lock(&linkedListLock);

        pthread_mutex_lock(&operationIndexLock);
        if(operationIndex<m){
            int op[2] = operations[operationIndex];
            operationIndex++;
            pthread_mutex_unlock(&operationIndexLock);
            
            if(op[0]==0){
                Member(op[1], head);
            }else if (op[0]==1){
                Insert(op[1], head);
            }else{
                Delete(op[1], head);
            }
        }else{
            pthread_mutex_unlock(&operationIndexLock);
        }
        pthread_mutex_unlock(&linkedListLock);
    }
    
    return NULL;
}

void* operationReadWrite(void* arg) {
    int (*operations)[2] = (int (*)[2]) arg;
    
    while (operationIndex<m){

        pthread_mutex_lock(&operationIndexLock);

        if(writeOps!=NULL && operationIndex-writeOps->startIndex>threadCount*2){
            if (writeOps->opType==1){
                pthread_rwlock_wrlock(&rwlock);
                Insert(writeOps->value, head);
                pthread_rwlock_unlock(&rwlock);
            }else{
                pthread_rwlock_wrlock(&rwlock);
                Delete(writeOps->value, head);
                pthread_rwlock_unlock(&rwlock);
            }
            writeOps = writeOps->next;
        }

        if(operationIndex<m){
            int op[2] = operations[operationIndex];
            operationIndex++;
            pthread_mutex_unlock(&operationIndexLock);
            
            if(op[0]==0){
                pthread_rwlock_rdlock(&rwlock);
                Member(op[1], head);
                pthread_rwlock_unlock(&rwlock);
            }else{
                struct write_op_s *p = writeOps;
                while(writeOps->next!=NULL){
                    p=p->next;
                }
                if(p==writeOps){
                    p->startIndex = operationIndex-1;
                }else{
                    p->startIndex = writeOps->startIndex;
                }
                p->opType= op[0];
                p->value= op[1];
                p->next=NULL;
            }
        }else{
            pthread_mutex_unlock(&operationIndexLock);
        }
    }

    if(writeOps!=NULL){
        if (writeOps->opType==1){
            pthread_rwlock_wrlock(&rwlock);
            Insert(writeOps->value, head);
            pthread_rwlock_unlock(&rwlock);
        }else{
            pthread_rwlock_wrlock(&rwlock);
            Delete(writeOps->value, head);
            pthread_rwlock_unlock(&rwlock);
        }
        writeOps = writeOps->next;
    }
    
    return NULL;
}

int main()
{
    populateList();
    int operations[m][2];
    generateOperations(operations);

    int method = 1;

    if(method==1){
        for(int i=0;i<m;i++){
            if(operations[i][0]==0){
                Member(operations[i][1], head);
            }else if(operations[i][0]==1){
                Insert(operations[i][1], head);
            }else{
                Delete(operations[i][1], head);
            }
        }
    }else {
        
        pthread_mutex_init(&operationIndexLock, NULL);
        if(method==2){
            pthread_t threads[threadCount];
            pthread_mutex_init(&linkedListLock, NULL);

            for (long i = 0; i < threadCount; i++) {
                pthread_create(&threads[i], NULL, operationMutex, (void*)operations);
            }

            for (int i = 0; i < threadCount; i++) {
                pthread_join(threads[i], NULL);
            }

            pthread_mutex_destroy(&linkedListLock);
        }else{
            pthread_t threads[threadCount];

            pthread_rwlock_init(&rwlock, NULL);
            writeOps = malloc(sizeof(struct write_op_s));

            for (long i = 0; i < threadCount; i++) {
                pthread_create(&threads[i], NULL, operationReadWrite, (void*)operations);
            }

            for (int i = 0; i < threadCount; i++) {
                pthread_join(threads[i], NULL);
            }

            pthread_rwlock_destroy(&rwlock);
        }

        pthread_mutex_destroy(&operationIndexLock);
    }

    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
#include<limits.h>

#define READ_END 0
#define WRITE_END 1

//stores the thread data
typedef struct data {
    int* int_arr;
    int sI;
    int eI;
}data;

//stores the max and min values of subarrays
struct result{
    int max;
    int min;
};

//function for finding min and max of subarrays
void* find_min_max(void* p) {
    data* ptr = (data*)p;
    int startInd = ptr->sI;
    int endInd = ptr->eI;
    struct result* max_min = (struct result*)calloc(2, sizeof(int));
    max_min->min = ptr->int_arr[startInd];
    max_min->max = ptr->int_arr[startInd];
    for(int i=startInd+1; i<=endInd; i++) {
        if(ptr->int_arr[i]<max_min->min) max_min->min = ptr->int_arr[i];
        if(ptr->int_arr[i]>max_min->max) max_min->max = ptr->int_arr[i];
    }
    pthread_exit(max_min);
}

int main(){
    int fd1[2];
    int fd2[2];
    int fd3[2];
    int fd4[2];
    int fd5[2];

    int n1=0,n2=0,n3=0;
    pid_t p,p2;
    int a=0,b=0;
    if (pipe(fd1)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd2)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd3)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd4)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd5)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }


    p=fork();
    if (p < 0)
    {
        fprintf(stderr, "Fork Failed" );
        return 1;
    }
    else if(p>0){
        printf("Enter to parent the no. of terms of Fibonacci series: ");
        scanf("%d",&n1);
        close(fd1[READ_END]);
        write(fd1[WRITE_END],&n1,sizeof(int));
        wait(NULL);
        close(fd1[WRITE_END]);
    }
    else {
        close(fd[WRITE_END]);
        read(fd1[READ_END],&a,sizeof(int));
        printf("Child1 received %d\n",a);
        int term1 = 1, term2 = 1, nextTerm = 0;
        printf("The Fibonacci Series printed by Child1 is: ");
        for (int i = 1; i <= a; ++i) {
            printf("%d ", term1);
            nextTerm = term1 + term2;
            term1 = term2;
            term2 = nextTerm;
        }
        printf( "\n" );
        close(fd1[READ_END]);
        exit(0);
    }

    p=fork();

    if (p < 0)
    {
        fprintf(stderr, "Fork Failed" );
        return 1;
    }
    else if(p>0){
        printf("Enter to parent the no. of child threads to be created: ");
        scanf("%d",&n2);
        close(fd2[READ_END]);
        write(fd2[WRITE_END],&n2,sizeof(int));
        close(fd2[WRITE_END]);
        for(int k=0;k<255555;k++);
        printf("Enter to parent number of array elements for Child2: ");
        scanf("%d",&n3);
        close(fd3[READ_END]);
        write(fd3[WRITE_END],&n3,sizeof(int));
        close(fd3[WRITE_END]);
        for(int k=0;k<255555;k++);
        int values[n3];
        printf("Enter to parent the array elements for Child2: ");
        close(fd4[READ_END]);
        for(int i = 0; i < n3; ++i) {
            scanf("%d", &values[i]);
            write(fd4[WRITE_END],&values[i],sizeof(int));
        }
        close(fd4[WRITE_END]);
        wait(NULL);


    }
    else{
        close(fd2[WRITE_END]);
        close(fd3[WRITE_END]);
        close(fd4[WRITE_END]);
        read(fd2[READ_END],&a,sizeof(int));
        printf("Child2 received %d as no. of threads \n",a);
        close(fd2[READ_END]);
        read(fd3[READ_END],&n3,sizeof(int));
        printf("Child2 received %d as number of array elements \n",n3);
        close(fd3[READ_END]);
        int* arr = (int*) calloc(n3, sizeof(int));

        for(int i = 0; i < n3; ++i) {
            read(fd4[READ_END],&b,sizeof(int));
            arr[i] = b;
        }
        close(fd4[READ_END]);

        //array that stores the thread data
        data thread_data[a];

        int element_no = n3/a; //no. of elements in each subarray
        int temp = 0;

        for(int k=0; k<a; k++) {
            thread_data[k].int_arr = arr;
            thread_data[k].sI = temp;
            thread_data[k].eI = temp+element_no-1;
            temp += element_no;
        }

        //assigning the values in the array of thread data

        //creating thread
        int i=0;
        pthread_t tid[a];

        //will store the final value
        int final_min=INT_MAX;
        int final_max=INT_MIN;

        while(i<a) {
            printf("\nLower bound for Thread%d is %d", i+1, thread_data[i].sI);
            printf("\nUpper bound for Thread%d is %d", i+1, thread_data[i].eI);

            pthread_create(&tid[i], NULL, find_min_max, &thread_data[i]);
            struct result* final;
            pthread_join(tid[i], (void**)&final); // return the min and max of the subarray
            printf("\nComputed by Thread%d: max = %d, min = %d", i+1, final->max, final->min);
            if(final->min < final_min) final_min = final->min;
            if(final->max > final_max) final_max = final->max;
            i++;
        }

        printf("\nCalculated by Child2: max = %d, min = %d\n", final_max, final_min);
        exit(0);

    }

    p=fork();

    if (p < 0)
    {
        fprintf(stderr, "Fork Failed" );
        return 1;
    }

    else if(p>0){
        printf("Enter the no. for factorial computation in parent: ");
        scanf("%d",&n1);
        close(fd5[READ_END]);
        write(fd5[WRITE_END],&n1,sizeof(int));
        close(fd1[WRITE_END]);
        wait(NULL);
        printf("Goodbye");
    }


    else {
        close(fd5[WRITE_END]);
        read(fd5[READ_END],&a,sizeof(int));
        close(fd5[READ_END]);
        printf("Child3 received 5 for factorial computation %d\n",a);
        int ans = 1;
        printf("Factorial computed by Child3 is ");
        for (int i = 2; i <= a; ++i) {
            ans = ans*i;
        }
        printf( "%d \n",ans );
        
        exit(0);
    }


    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
#include<limits.h>


//function for finding min and max of subarrays
void* find_min_max(void* received_arr) {
    int *ptr = (int *)received_arr;
    int n = sizeof(ptr)/sizeof(ptr[0]);
    printf("%d",n);       
    int* max_min= (int*) calloc(2, sizeof(int));
    max_min[0] = INT_MAX;
    max_min[1] = INT_MIN;
    for(int i=1; i<n; i++) {
        if(ptr[i]<max_min[0]) max_min[0] = ptr[i];
        if(ptr[i]>max_min[1]) max_min[1] = ptr[i];
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
    int a=0,b=0,c=0,d=0,e=0;
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
        write(fd1[1],&n1,sizeof(int));
        wait(NULL);
        close(fd1[1]);
    }
    else {
        read(fd1[0],&a,sizeof(int));
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
        close(fd1[0]);
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
        write(fd2[1],&n2,sizeof(int));
        close(fd2[1]);
        sleep(2);
        printf("Enter to parent number of array elements for Child2: ");
        scanf("%d",&n3);
        write(fd3[1],&n3,sizeof(int));
        close(fd3[1]);
        sleep(2);
        int values[n3];
        printf("Enter to parent the array elements for Child2: ");
        for(int i = 0; i < n3; ++i) {
            scanf("%d", &values[i]);
            write(fd4[1],&values[i],sizeof(int));
        }
        close(fd4[1]);
        wait(NULL);


    }
    else{
        read(fd2[0],&b,sizeof(int));
        printf("Child2 received %d as no. of threads \n",b);
        close(fd2[0]);
        read(fd3[0],&c,sizeof(int));
        printf("Child2 received %d as number of array elements \n",c);
        close(fd3[0]);
        int arr[c];

        for(int i = 0; i < c; ++i) {
            read(fd4[0],&d,sizeof(int));
            arr[i] = d;
        }
        close(fd4[0]);

        //array that stores the thread data
        if(c%b != 0) {
            printf("invalid Information added!");
            exit(0);
        }

        int numberOfElements = c/b; //no. of elements in each subarray
        int temp = 0;

        //creating thread
        int i=0,j=0;
        pthread_t tid[b];
        

        //will store the final value
        int final_min=INT_MAX;
        int final_max=INT_MIN;

        while(i<b) {
            
            int int_arr[numberOfElements]; 
            
            for(int k=0; k<numberOfElements; k++) {
                int_arr[k]=arr[j];
                j++;
            }
         
            printf("\nLower bound for Thread%d is %d", i+1, temp);
            printf("\nUpper bound for Thread%d is %d", i+1, j-1);
            temp += numberOfElements;
            pthread_create(&tid[i], NULL, find_min_max, &int_arr);
            int* final[2];
            pthread_join(tid[i], (void**)&final); // return the min and max of the subarray
            printf("\nComputed by Thread%d: max = %d, min = %d", i+1, *final[1], *final[0]);
            if(*final[0] < final_min) final_min = *final[0];
            if(*final[1] > final_max) final_max = *final[1];
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
        write(fd5[1],&n1,sizeof(int));
        wait(NULL);
        close(fd1[1]);
    }


    else {
        read(fd5[0],&a,sizeof(int));
        printf("Child3 received 5 for factorial computation %d\n",a);
        int ans = 1;
        printf("Factorial computed by Child3 is ");
        for (int i = 2; i <= a; ++i) {
            ans = ans*i;
        }
        printf( "%d \n",ans );
        close(fd5[0]);
        exit(0);
    }


    return 0;
}



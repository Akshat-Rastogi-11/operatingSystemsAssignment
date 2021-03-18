#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<pthread.h>
#include<limits.h>

//function for finding min and max of subarrays
void* find_min_max(void* p) {

    struct data{
        int startInd;
        int endInd;
        int *int_arr;
        int min;
        int max;
    };

    struct data* args = (struct data*)p;

    args->min = args->int_arr[args->startInd];
    args->max = args->int_arr[args->startInd];
    for(int i=args->startInd+1; i<=args->endInd; i++) {
        if(args->int_arr[i] < args->min) args->min = args->int_arr[i];
        if(args->int_arr[i] > args->max) args->max = args->int_arr[i];
    }
    pthread_exit(args);
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
        for(int x=0;x<255;x++){
        	for(int y=0;y<255;y++){
        		;
        	}
        }
        printf("Enter to parent number of array elements for Child2: ");
        scanf("%d",&n3);
        write(fd3[1],&n3,sizeof(int));
        close(fd3[1]);
        for(int x=0;x<255;x++){
        	for(int y=0;y<255;y++){
        		;
        	}
        }
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
        int* arr = (int*) calloc(c, sizeof(int));

        for(int i = 0; i < c; ++i) {
            read(fd4[0],&d,sizeof(int));
            arr[i] = d;
        }
        close(fd4[0]);

        int element_no = c/b; //no. of elements in each subarray
        int temp = 0;


        //creating thread
        int i=0;
        pthread_t tid[b];

        struct data{
                int startInd;
                int endInd;
                int *int_arr;
                int min;
                int max;
        };

        struct data args[b];

        for(int i=0; i<b; i++) {
            args[i].int_arr = arr;
            args[i].startInd = temp;
            args[i].endInd = temp+element_no-1;

            pthread_create(&tid[i], NULL, find_min_max, &args[i]);
            temp += element_no;

        }
        temp =0;
         //will store the final value
        int final_min=INT_MAX;
        int final_max=INT_MIN;

        for(int i=0; i<b; i++) {

            args[i].startInd = temp;
            args[i].endInd = temp+element_no-1;
            printf("\nLower bound for Thread%d is %d", i+1, args[i].startInd);
            printf("\nUpper bound for Thread%d is %d", i+1, args[i].endInd);

            pthread_join(tid[i], (void **)&args[i]); // return the min and max of the subarray
            printf("\nComputed by Thread%d: max = %d, min = %d", i+1, args[i].max, args[i].min);
            if(args[i].min < final_min) final_min = args[i].min;
            if(args[i].max > final_max) final_max = args[i].max;

            temp += element_no;
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
        printf("GoodBye!");
        close(fd1[1]);
    }


    else {
        read(fd5[0],&e,sizeof(int));
        printf("Child3 received 5 for factorial computation %d\n",e);
        int ans = 1;
        printf("Factorial computed by Child3 is ");
        for (int i = 2; i <= e; ++i) {
            ans = ans*i;
        }
        printf( "%d \n",ans );
        close(fd5[0]);
        exit(0);
    }


    return 0;
}

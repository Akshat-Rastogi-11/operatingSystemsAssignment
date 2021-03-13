#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
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
	read(fd2[0],&a,sizeof(int));
	printf("Child2 received %d as no. of threads \n",a);
	close(fd2[0]);
	read(fd3[0],&a,sizeof(int));
	printf("Child2 received %d as number of array elements \n",a);
	close(fd3[0]);
	int arr[a];
	for(int i = 0; i < a; ++i) {
     		read(fd4[0],&b,sizeof(int));
     		b = arr[i];
  	}
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



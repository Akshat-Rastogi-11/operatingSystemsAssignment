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

int n1=0,n2=0;
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

printf("Enter to parent the no. of terms of Fibonacci series:");
scanf("%d",&n1);
p=fork();

if (p < 0)
{
	fprintf(stderr, "Fork Failed" );
	return 1;
}
else if(p>0){
	write(fd1[1],&n1,sizeof(int));
	close(fd1[1]);
}
else{
	read(fd1[0],&a,sizeof(int));
	printf("Child1 received %d\n",a);
	int term1 = 1, term2 = 1, nextTerm = 0;
	printf("The Fibonacci Series printed by Child1 is:");
	for (int i = 1; i <= a; ++i) {
        	printf("%d, ", term1);
        	nextTerm = term1 + term2;
        	term1 = term2;
        	term2 = nextTerm;
    	}
	close(fd1[0]);
}

return 0;
}

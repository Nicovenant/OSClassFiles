#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
int fd[2]; //creates pipe fd
pipe(fd);

int t, t1; 
t=atoi(argv[1]);
t1 = atoi(argv[2]);

int array[10];



printf("number of children: %d\n", t); 
printf("I am looking for the %d term\n", t1);


if(t==1){
 

 printf("hello world, I am Parent the only one with process ID: (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
       // fork failed; exit
        fprintf(stderr, "fork failed\n");
         exit(1);
        
        } else if (rc == 0) {
          array[0]=0;
          array[1]=1;
        close(fd[0]);
        

        int i;
        for(i=2; i<t1; i++){  //add all the elements of the array.
          array[i] = array[i-1] + array[i-2];
         }
          write(fd[1], &array[t1-1], sizeof(array[t1-1]));
          printf(" I am process %d and I am sending : %d to my parent\n", (int) getpid(), array[t1-1]);
         close(fd[1]);
        exit(0);   //I am including exit(0) to kill the process after is done !!!!@
    }
   else {
        int sum1;
        int final=0;
        close(fd[1]);
        read(fd[0], &sum1, sizeof(sum1));
        final = sum1;
        close(fd[0]);
        printf("I am the parent at the end with ID %d with a final sum of: %d \n" , (int) getpid(), final);
    }

}
    return 0;
}

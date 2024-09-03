#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    // initialize pipes
    
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);

   //Some variables
    int luis;
    int l,i;
    int num;
    int num1;
    
// An array to store the command line array
   int pal[6];
    
// Read the numbers from the command line

    for(l=1; l<argc; l++){
       num = atoi(argv[l]);
     
       pal[l-1] = num;   //l-1 because start reading the elements of argv at 1 in zero I have ./LuisChild
       printf("num = %d\n", pal[l-1]); //printing the elements of the array
     }

    int numElements = 6; // Non dynamic approach :(
    
    luis = fork(); 
    if (luis == 0) { //creating a child
        
        close(p1[1]); //
        close(p2[0]);
         int vals[numElements];
        read(p1[0], &vals, sizeof(vals));
        // compute the sum
        int sum = 0;
        printf("child %d: received ", getpid());
        int i;
        for (i=0;i<numElements;i++){
            printf("%d  ", vals[i]);
            sum += vals[i];
        }
        printf("\n");
        printf("child %d: sending %d to parent\n", getpid(), sum);
        // send result to parent
        write(p2[1], &sum, sizeof(sum));
        exit(0);
    } else {
        // parent
        close(p1[0]);
        close(p2[1]);
        // send values to child
        write(p1[1], &pal, sizeof(pal));
        printf("parent is %d\n", getpid());
        // receive and print the final value
        int recval;
        read(p2[0], &recval, sizeof(recval));
        printf("parent: final sum is %d\n", recval);
        exit(0);
    }
    return 0;
}

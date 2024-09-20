#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>  

// Function to check if a number is prime
int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Function to calculate primes in a range and write them to a pipe
void calculate_primes(int start, int end, int write_fd) {
    close(write_fd);  // Close unused read end of the pipe
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            write(write_fd, &i, sizeof(int));  // Write prime number to pipe
        }
    }
    int done = -1;  // End marker
    write(write_fd, &done, sizeof(int));
    close(write_fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc % 2 == 0) {
        fprintf(stderr, "Usage: %s start1 end1 [start2 end2 ...]\n", argv[0]);
        exit(1);
    }

    int num_ranges = (argc - 1) / 2;
    int pipes[num_ranges][2];
    pid_t pids[num_ranges];
    int total_primes = 0;

    // Creating pipes and child processes
    for (int i = 0; i < num_ranges; i++) {
        int start = atoi(argv[2 * i + 1]);
        int end = atoi(argv[2 * i + 2]);

        // Create pipe for each child
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pids[i] = fork();

        if (pids[i] == 0) {
            // Child process
            close(pipes[i][0]);  // Close read end in child
            printf("I am child with pid: %d, calculating primes in range %d-%d.\n", getpid(), start, end);
            calculate_primes(start, end, pipes[i][1]);
            exit(0);  // Exit child after finishing
        } else if (pids[i] < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Parent process
    for (int i = 0; i < num_ranges; i++) {
        close(pipes[i][1]);  // Close write end in parent

        printf("I am the parent with pid: %d, receiving primes from child %d: ", getpid(), i + 1);

        int prime;
        while (read(pipes[i][0], &prime, sizeof(int)) > 0 && prime != -1) {
            printf("%d ", prime);
            total_primes++;
        }
        printf("\n");
        close(pipes[i][0]);

        // Wait for child to finish
        waitpid(pids[i], NULL, 0);
    }

    printf("Total primes found: %d.\n", total_primes);
    return 0;
}

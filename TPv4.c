#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void exercise1(const char *ip) {
    int pipefd[2];
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) { // Child process
        close(pipefd[0]); // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
        execlp("ping", "ping", "-c", "1", ip, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipefd[1]); // Close the write end of the pipe
        char buffer[1024];
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);
        printf("%s", buffer); // Print the output of the ping command
    }
}

void exercise2(const char *ip) {
    int pipefd[2];
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) { // Child process
        close(pipefd[0]); // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
        execlp("ping", "ping", "-c", "1", ip, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipefd[1]); // Close the write end of the pipe
        char buffer[1024];
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);

        int transmitted = 0, received = 0;
        char *line = strtok(buffer, "\n");
        while(line) {
            if(strstr(line, "packets transmitted")) {
                sscanf(line, "%d packets transmitted, %d received,", &transmitted, &received);
            }
            line = strtok(NULL, "\n");
        }
        
        float avg_time = 0.0;
        char *ptr = strstr(buffer, "avg = ");
        if(ptr) {
            sscanf(ptr, "avg = %f", &avg_time);
        }
        
        float received_percentage = ((float)received / transmitted) * 100;
        printf("Percentage of packets received: %.2f%%\n", received_percentage);
        printf("Average round-trip time: %.2f ms\n", avg_time);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        return 1;
    }
    
    printf("Exercise 1:\n");
    exercise1(argv[1]);
    
    printf("\nExercise 2:\n");
    exercise2(argv[1]);

    
void exercise3(const char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        perror("Error opening file");
        return;
    }
    
    char ip[256];
    while(fgets(ip, sizeof(ip), file)) {
        // Remove trailing newline character
        ip[strcspn(ip, "\n")] = 0;

        // Ping the IP
        int pipefd[2];
        if(pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if(pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0) { // Child process
            close(pipefd[0]); // Close the read end of the pipe
            dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
            execlp("ping", "ping", "-c", "1", ip, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else { // Parent process
            close(pipefd[1]); // Close the write end of the pipe
            char buffer[1024];
            read(pipefd[0], buffer, sizeof(buffer));
            close(pipefd[0]);

            int transmitted = 0, received = 0;
            char *line = strtok(buffer, "\n");
            while(line) {
                if(strstr(line, "packets transmitted")) {
                    sscanf(line, "%d packets transmitted, %d received,", &transmitted, &received);
                }
                line = strtok(NULL, "\n");
            }

            float avg_time = 0.0;
            char *ptr = strstr(buffer, "avg = ");
            if(!ptr) {
                ptr = strstr(buffer, "mdev = "); // Alternative check if avg is not found
            }
            if(ptr) {
                char avg_str[20];
                sscanf(ptr, "%*s %*s %*s %s", avg_str);
                avg_time = atof(avg_str);
            }
            
            float received_percentage = ((float)received / transmitted) * 100;
            if(received_percentage == 0) {
                printf("%s => %.0f, +oo\n", ip, received_percentage);
            } else if (avg_time == 0.0) { // Handle potential discrepancies in ping output
                printf("%s => %.0f, N/A\n", ip, received_percentage);
            } else {
                printf("%s => %.0f, %.3f ms\n", ip, received_percentage, avg_time);
            }
        }
    }
    
    fclose(file);
}

    printf("\nExercise 3:\n");
    exercise3("ip_list.txt");
return 0;
}

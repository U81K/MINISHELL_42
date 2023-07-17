#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int main() {
//     int num_forks = 3; // Number of forks
//     int i;

//     for (i = 0; i < num_forks; i++) {
//         pid_t pid = fork();

//         if (pid < 0) {
//             perror("fork");
//             exit(1);
//         } else if (pid == 0) {
//             // Child process

//             printf("Child process ID: %d\n", getpid());
//             printf("Parent process ID: %d\n", getppid());
//             exit(0);
//         }
//     }

//     // Parent process
//     for (i = 0; i < num_forks; i++) {
//         wait(NULL); // Wait for each child process to finish
//     }

//     return 0;
// }

int main() {
    int fd[2];
    

    pipe(fd);
    int pid = fork();

    if (pid == 0) 
    {
        // Close the read end of the pipe
        close(fd[0]);

        // Duplicate the write end of the pipe to stdout
        dup2(fd[1], STDOUT_FILENO);

        // Close the write end of the pipe
        close(fd[1]);

        // Execute the ls command
        execlp("ls", "ls", NULL);

        exit(1);
    } 
    else 
    {
        // Close the write end of the pipe
        close(fd[1]);

        // Duplicate the read end of the pipe to stdin
        dup2(fd[0], STDIN_FILENO);

        // Close the read end of the pipe
        close(fd[0]);

        // Execute the wc -l command
        execlp("wc", "wc", "-l", NULL);

        // If exec fails, print an error
        perror("execlp");
        exit(1);
    }

    return 0;
}
// t_env *run_commands(t_cmd *cmd, t_env* env, t_info *info) 
// {
//     int num_cmds = nbr_cmd(info);
//     int i;
//     int fd[num_cmds - 1][2];

//     i = 0;
//     if(num_cmds == 1)
//     {
//         env = commands(&cmd[0], env, info);
//         return env;
//     }
//     while (i < num_cmds - 1) 
//         pipe(fd[i++]);
//     i = 0;
//     while (i < num_cmds) 
//     {
//         int pid = fork();
//         if (pid == 0) 
//         {
//             // Child process
            
//             // Connect input from previous command output
//             if (i > 0) 
//             {
//                 close(fd[i - 1][1]); // Close the write end of the previous pipe
//                 dup2(fd[i - 1][0], STDIN_FILENO); // Redirect input from the read end of the previous pipe
//                 close(fd[i - 1][0]); // Close the read end of the previous pipe
//             }

//             // Connect output to next command input
//             if (i < num_cmds - 1) 
//             {
//                 close(fd[i][0]); // Close the read end of the current pipe
//                 dup2(fd[i][1], STDOUT_FILENO); // Redirect output to the write end of the current pipe
//                 close(fd[i][1]); // Close the write end of the current pipe
//             }
//             env = commands(&cmd[i], env, info);
//             exit(exist_status);
//         }
//         i++;
//     }
//     i = 0;
//     while (i < num_cmds - 1) 
//     {
//         close(fd[i][0]);
//         close(fd[i][1]);
//         i++;
//     }
//     i = 0;
//     while (i < num_cmds - 1) 
//     {
//         wait(NULL);
//         i++;
//     }
//     return env;
// }
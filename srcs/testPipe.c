#include <unistd.h>
#include <sys/errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define    STDIN        0
#define STDOUT        1
#define READ_END    0
#define WRTIE_END    1

int
    main(int argc, char **av, char **envp)
{
    int        loopsize = 4;
    int        *fd = malloc(sizeof(*fd) * 2 * loopsize);
    char ***argv = malloc(sizeof(**argv) * loopsize);
    argv[0] = malloc(sizeof(*argv) * 3);
    argv[1] = malloc(sizeof(*argv) * 3);
    argv[2] = malloc(sizeof(*argv) * 3);
    argv[3] = malloc(sizeof(*argv) * 3);
    argv[0][0] = strdup("/bin/cat");
    argv[0][1] = strdup("main.c");
    argv[0][2] = NULL;
    argv[1][0] = strdup("/usr/bin/grep");
    argv[1][1] = strdup("int");
    argv[1][2] = NULL;
    argv[2][0] = strdup("/usr/bin/grep");
    argv[2][1] = strdup("i");
    argv[2][2] = NULL;
    argv[3][0] = strdup("/usr/bin/grep");
    argv[3][1] = strdup("");
    argv[3][2] = NULL;

    pid_t    pid;
    int        status;
    int        fdfile = open("results.txt", O_WRONLY | O_TRUNC);

    for (int i = 0; i < loopsize; ++i)
    {
        pipe(&fd[i * 2]);
        pid = fork();
        if (pid == 0)
        {
            if (i != loopsize - 1)
                dup2(fd[i * 2 + 1], STDOUT); //dup write end in stdout
            if (i > 0)
                dup2(fd[(i - 1) * 2], STDIN); //dup read end of previous pipe in stdin
            execve(argv[i][0], argv[i], envp);
            exit(1);
        }
        else
        {
            waitpid(pid, &status, 0);
            close(fd[i * 2 + 1]); //close write end
            if (i == loopsize - 1)
                close(fd[i * 2]); //close read end
            if (i > 0)
                close(fd[(i - 1) * 2]); //close read end of previous pipe
        }
    }
}
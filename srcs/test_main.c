#include "minishell.h"

t_cmd *test_exec()
{
    int fd1;

    //t_token	*init_token(t_token *next, char *str, int type)
    // t_cmd	*init_cmd(t_cmd *next, t_token *arg, t_token *redir)
    t_cmd *test;
    test = init_cmd(NULL, init_token(NULL, "echo", word), NULL);
    test->arg->next = init_token(NULL, "-n", word);
    test->arg->next->next = init_token(NULL, "$L", word);
    test->redir = init_token(NULL, ">>", rdout);
    test->redir->next = init_token(NULL, "remplir", fd);
    test->fdin = -1;
    fd1 = open("./remplir", O_CREAT | O_RDWR | O_APPEND, 0666);
    test->fdout = fd1;
    test->next = init_cmd(NULL, init_token(NULL, "cat", word), NULL);
    test->next->arg->next = init_token(NULL, "-e", word);
    test->next->fdin = test->fdout;
    test->next->fdout = 1;
    close(fd1); // a toi de jouer !
    print_cmd(test);
    return (test);
}
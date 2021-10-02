void handlePipes(int no_of_words, char *command[])
{
    int pipe_count = 0;
    for (int i = 0; i < no_of_words; i++)
        if (!strcmp(command[i], "|"))
            pipe_count++;

    if (pipe_count == 0)
        handleRedirection(no_of_words, command);
    else
    {
        int fds[2 * pipe_count + 1];
        for (int i = 0; i < pipe_count; i++)
            if (pipe(fds + 2 * i) == -1)
                handleErrors(-19);

        int pid;
        int cur_word = 0;
        int argc = 0;
        char *argv[MAX_NO_WORDS_IN_COMMAND + 5] = {NULL};

        for (int i = 0; i < pipe_count + 1; i++)
        {
            for (; cur_word < no_of_words; cur_word++)
            {
                if (!strcmp(command[cur_word], "|"))
                {
                    cur_word++;
                    break;
                }
                argv[argc++] = command[cur_word];
            }

            pid = fork();
            if (pid == 0)
            {

                // skipping last command after final pipe.
                if (i < pipe_count)
                    dup2(fds[2 * i + 1], STDOUT_FD);

                // skipping first command b4 initial pipe.
                if (i > 0)
                    dup2(fds[2 * (i - 1)], STDIN_FD);

                for (int j = 0; j < 2 * pipe_count; j++)
                    close(fds[j]);

                handleRedirection(argc, argv);

                exit(0);
            }

            for (int j = 0; j <= argc; j++)
                argv[j] = NULL;
            argc = 0;
        }

        for (int j = 0; j < 2 * pipe_count; j++)
            close(fds[j]);
        for (int i = 0; i < pipe_count + 1; i++)
            wait(NULL);
    }
}
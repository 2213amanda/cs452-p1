#include "lab.h"

/**
   * @brief Set the shell prompt. This function will attempt to load a prompt
   * from the requested environment variable, if the environment variable is
   * not set a default prompt of "shell>" is returned.  This function calls
   * malloc internally and the caller must free the resulting string.
   *
   * @param env The environment variable
   * @return const char* The prompt
   */
  char *get_prompt(const char *env){

        const char *val = getenv(env);
        char *prompt = malloc(sizeof(char *) + 1);

        if(val == NULL){
            *prompt = malloc(strlen("shell>") + 1);
            strcpy(prompt, "shell>");
        } else {
            *prompt = malloc(strlen(val) + 1);
            strcpy(prompt, val);
        }

        return prompt;
  }

  /**
   * Changes the current working directory of the shell. Uses the linux system
   * call chdir. With no arguments the users home directory is used as the
   * directory to change to.
   *
   * @param dir The directory to change to
   * @return  On success, zero is returned.  On error, -1 is returned, and
   * errno is set to indicate the error.
   */
  int change_dir(char **dir){

        char *target;

        if (dir == NULL || *dir == NULL || strlen(*dir) == 0){
            target = getenv("HOME");
            if(target == NULL){
                fprintf(stderr, "HOME environment variable not set.\n");
                return -1;
            }
        } else {
            target = *dir;
        }

        if(chdir(target) != 0){
            fprintf(stderr, "Unable to change directory.\n");
            return -1;
        }

        return 0;
  }

  /**
   * @brief Convert line read from the user into to format that will work with
   * execvp. We limit the number of arguments to ARG_MAX loaded from sysconf.
   * This function allocates memory that must be reclaimed with the cmd_free
   * function.
   *
   * @param line The line to process
   *
   * @return The line read in a format suitable for exec
   */
  char **cmd_parse(char const *line){

    if(line == NULL){
        return NULL;
    }

    size_t maxArgs = ARG_MAX / sizeof(char *);
    char **args = malloc((maxArgs + 1) * sizeof(char *));
    if(args == NULL){
        fprintf(stderr, "parsing malloc failed");
        return NULL;
    }

    size_t count = 0;
    char *parsedChar;
    char *myLine = strdup(line);
    if (myLine == NULL){
        fprintf(stderr, "line duplication error.\n");
        free(args);
        return NULL;
    }

    parsedChar = strtok(myLine, " \t\n");
    while (parsedChar != NULL && count < maxArgs){
        args[count] = strdup(parsedChar);
        if(args[count] == NULL){
            fprintf(stderr, "parsing error.\n");
            for (size_t i = 0; i < count; i++){
                free(args[i]);
            }
            free(args);
            free(myLine);
            return NULL;
        }
        count++;
        parsedChar = strtok(NULL, " \t\n");
    }

    args[count] = NULL;

    free(parsedChar);
    free(myLine);
    return args;

  }

  /**
   * @brief Free the line that was constructed with parse_cmd
   *
   * @param line the line to free
   */
  void cmd_free(char ** line){
    if(line == NULL){
        return;
    } else {
        size_t i = 0;
        while(line[i] != NULL){
            free(line[i]);
            i++;
        }
        free(line);
    }
  }

  /**
   * @brief Trim the whitespace from the start and end of a string.
   * For example "   ls -a   " becomes "ls -a". This function modifies
   * the argument line so that all printable chars are moved to the
   * front of the string
   *
   * @param line The line to trim
   * @return The new line with no whitespace
   */
  char *trim_white(char *line){
    if (line == NULL){
        return NULL;
    }

    char *first = line;
    while(*first && isspace((unsigned char)*first)){
        first++;
    }

    char *last = line + strlen(line) - 1;
    while (last > first && isspace((unsigned char)*last)){
        last--;
    }
    
    *(last + 1) = '\0';

    if (first != line){
        memmove(line, first, last - first + 2);
    }

    return line;
  }


  /**
   * @brief Takes an argument list and checks if the first argument is a
   * built in command such as exit, cd, jobs, etc. If the command is a
   * built in command this function will handle the command and then return
   * true. If the first argument is NOT a built in command this function will
   * return false.
   *
   * @param sh The shell
   * @param argv The command to check
   * @return True if the command was a built in command
   */
  bool do_builtin(struct shell *sh, char **argv){
    return NULL;
  }

  /**
   * @brief Initialize the shell for use. Allocate all data structures
   * Grab control of the terminal and put the shell in its own
   * process group. NOTE: This function will block until the shell is
   * in its own program group. Attaching a debugger will always cause
   * this function to fail because the debugger maintains control of
   * the subprocess it is debugging.
   *
   * @param sh
   */
  void sh_init(struct shell *sh){
    if (sh == NULL){
        fprintf(stderr, "shell structure is NULL.\n");
        return;
    }

    sh->shell_pgid = getpid();
    if(setpgid(sh->shell_pgid, sh->shell_pgid) == -1){
        fprintf(stderr, "setpgid failed.\n");
        exit(-1);
    }

    if(tcgetattr(STDIN_FILENO, &sh->shell_tmodes) == -1){
        fprintf(stderr, "get attr failed.\n");
        exit(-1);
    }

  }

  /**
   * @brief Destroy shell. Free any allocated memory and resources and exit
   * normally.
   *
   * @param sh
   */
  void sh_destroy(struct shell *sh){
    exit(0);
  }

  /**
   * @brief Parse command line args from the user when the shell was launched
   *
   * @param argc Number of args
   * @param argv The arg array
   */
  void parse_args(int argc, char **argv){
    if (argc < 1){
        return;
    }

    return; 

  }

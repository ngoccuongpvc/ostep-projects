#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum Ops
{
    GET,
    PUT,
    DELETE,
    CLEAR,
    ALL
};

struct Command
{
    enum Ops ops;
    int argc;
    char **args;
};

struct Record
{
    char *key, *value;
};

enum Ops get_ops_enum(char *ops)
{
    if (strcmp(ops, "p") == 0)
    {
        return PUT;
    }
    else if (strcmp(ops, "g") == 0)
    {
        return GET;
    }
    else if (strcmp(ops, "d") == 0)
    {
        return DELETE;
    }
    else if (strcmp(ops, "c") == 0)
    {
        return CLEAR;
    }
    else if (strcmp(ops, "a") == 0)
    {
        return ALL;
    }
    else
    {
        printf("ops = %s\n", ops);
        perror("Invalid ops\n");
        exit(1);
    }
}

struct Command *parse_command(char *arg)
{
    char *tofree, *string;
    tofree = string = strdup(arg);

    char *ops = strsep(&string, ",");
    if (ops == NULL)
    {
        perror("Invalid command\n");
        exit(EXIT_FAILURE);
    }
    char **argv = NULL, *token;
    int argc = 0;
    while ((token = strsep(&string, ",")) != NULL)
    {
        argc += 1;
        argv = realloc(argv, argc * sizeof(char *));
        argv[argc - 1] = strdup(token);
    }

    struct Command *command = malloc(sizeof(struct Command));
    command->ops = get_ops_enum(ops);
    command->argc = argc;
    command->args = argv;
    free(tofree);
    return command;
}

void put(struct Record *records, int *n_records, char *k, char *v)
{
    for (int i = 0; i < *n_records; ++i)
    {
        if (strcmp(records[i].key, k) == 0)
        {
            records[i].value = realloc(records[i].value, strlen(v));
            strcpy(records[i].value, v);
            break;
        }
    }
    *n_records += 1;
    realloc(records, sizeof(struct Record *) * (*n_records));
    records[*n_records - 1].key = strdup(k);
    records[*n_records - 1].value = strdup(v);
}

int main(int argc, char *argv[])
{
    struct Record *records = NULL;
    int n_records = 0;

    for (int i = 1; i < argc; ++i)
    {
        printf("%s\n", argv[i]);
        struct Command *command = parse_command(argv[i]);
        if (command->ops == PUT) {
            put(records, &n_records, command->args[0], command->args[1]);
        }
    }
    return 0;
}
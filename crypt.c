#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#define MAX_KEY_LEN 256

void process_file(char *path, char *key, int key_length)
{
    FILE *fd;
    char *buffer;
    long filelen;
    if (!(fd = fopen(path, "rb")))
    {
        fprintf(stderr, "Error: failed to read file : %s\n%s\n", path, strerror(errno));
        return;
    }
    fseek(fd, 0, SEEK_END);
    filelen = ftell(fd);
    rewind(fd);
    buffer = (char *)malloc(filelen * sizeof(char));
    fread(buffer, filelen, 1, fd);
    fclose(fd);
    for (long i = 0; i < filelen; i++)
    {
        buffer[i] = buffer[i] ^ key[i % key_length];
    }
    if (!(fd = fopen(path, "w")))
    {
        fprintf(stderr, "Error: failed to write to file : %s\n%s\n", path, strerror(errno));
        return;
    }
    fwrite(buffer, 1, filelen, fd);
    fclose(fd);
    return;
}

void traverse_dir(char *path, char *key, int key_length)
{
    DIR *fd;
    struct dirent *in_file;
    if ((fd = opendir(path)))
    {
        while (in_file = readdir(fd))
        {
            if (!strcmp(in_file->d_name, "."))
            {
                continue;
            }
            if (!strcmp(in_file->d_name, ".."))
            {
                continue;
            }

            if (in_file->d_type == DT_REG)
            {
                char new_path[256];
                sprintf(new_path, "%s/%s", path, in_file->d_name);
                process_file(new_path, key, key_length);
            }
            else if (in_file->d_type == DT_DIR)
            {
                char new_path[256];
                sprintf(new_path, "%s/%s", path, in_file->d_name);
                traverse_dir(new_path, key, key_length);
            }
        }
    }
    else
    {
        fprintf(stderr, "Error: failed to open directory : %s\n%s\n", path, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: crypt -flag path\n");
        fflush(stdout);
        return 0;
    }
    if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "-d"))
    {
        char key[MAX_KEY_LEN + 1];
        printf("Enter key: \x1B[8m");
        fgets(key, MAX_KEY_LEN + 1, stdin);
        printf("\x1B[0m");
        fflush(stdout);
        int key_length = strlen(key);
        if (!strcmp(argv[1], "-f"))
        {
            process_file(argv[2], key, key_length);
        }
        else
        {
            traverse_dir(argv[2], key, key_length);
        }
    }
    else
    {
        printf("Options: -f : file, -d : directory\n");
        fflush(stdout);
        return 0;
    }
}
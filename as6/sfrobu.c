#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
void checkError(ssize_t error)
{
    if (error < 0)
    {
        fprintf(stderr, "Error reading file");
        exit(1);
    }
}

int frobcmpF(char const *a, char const *b)
{
    //since we added a space at the end of each string, we keep reading until we get to the end of the string(space)
    while (*a != ' ' || *b != ' ')
    {
        if (*a == ' ')
        {
            //if a is short than b
            return -1;
        }

        if (*b == ' ')
        {
            //if b is short than a
            return 1;
        }

        char clearA = *a ^ 42;
        char clearB = *b ^ 42;
        clearA = toupper(clearA);
        clearB = toupper(clearB);
        if (clearA > clearB)
        {
            return 1;
        }
        else if (clearA < clearB)
        {
            return -1;
        }
        a++;
        b++;
    }
    return 0;
}

int cmpF(const void *a, const void *b)
{
    return frobcmpF(*(const char **)a, *(const char **)b);
}

int frobcmp(char const *a, char const *b)
{
    //since we added a space at the end of each string, we keep reading until we get to the end of the string(space)
    while (*a != ' ' || *b != ' ')
    {
        if (*a == ' ')
        {
            //if a is short than b
            return -1;
        }

        if (*b == ' ')
        {
            //if b is short than a
            return 1;
        }

        char clearA = *a ^ 42;
        char clearB = *b ^ 42;

        if (clearA > clearB)
        {
            return 1;
        }
        else if (clearA < clearB)
        {
            return -1;
        }
        a++;
        b++;
    }
    return 0;
}

int cmp(const void *a, const void *b)
{
    return frobcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char **argv)
{
    int kib = 8192;
    char *buff = (char *)malloc(kib * sizeof(char)); //8 KiB

    int upper = 0;

    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments entered");
        exit(1);
    }
    //check if -f was entered
    if (argc == 2 && strcmp(argv[1], "-f") == 0)
        upper = 1;
    else if (argc == 2)
    {
        fprintf(stderr, "Incorrect argument entered");
        exit(1);
    }

    struct stat fileInfo;
    ssize_t error = 0;
    error = fstat(0, &fileInfo);
    checkError(error);
    int stdin = 1;
    int fileSize = 0;

    //if from file
    if (S_ISREG(fileInfo.st_mode))
    {
        stdin = 0;
        fileSize = fileInfo.st_size;
        buff = (char *)realloc(buff, fileSize);

        error = read(0, buff, fileSize);
        checkError(error);
       // buff[fileSize] = 0; //EOF
    }

    //if from standard input
    if (stdin == 1)
    {
        int totalsize = 0;
        int readsize = 0;
        do
        {
            readsize = read(0, buff + totalsize, 8192);
            totalsize += readsize;
            if (totalsize >= kib && readsize != 0)
            {
                kib = 2 * kib;
                buff = (char *)realloc(buff, kib);
            }

        } while (readsize > 0);

        buff[totalsize] = 0;
        fileSize = totalsize;
    }

    int rows = 0;
    int cells = 0;
    char **arr;
    arr = (char **)malloc((rows + 1) * sizeof(char *));
    arr[0] = (char *)malloc((cells + 1) * sizeof(char));
    int words = 0;
    int i = 0;
    int lastCells = 0;
    int space = 0;
    while (i < fileSize && (buff[i]) != EOF)
    {
        arr[rows][cells] = buff[i];
        cells++;
        arr[rows] = (char *)realloc(arr[rows], (cells + 1) * sizeof(char));
        space = 0;

        if (buff[i] == ' ')
        {
            words++;
            space = 1;
            arr[rows][cells] = '\0';

            rows++;
            lastCells = cells;
            cells = 0;
            arr = (char **)realloc(arr, (rows + 1) * sizeof(char *));
            arr[rows] = (char *)malloc((cells + 1) * sizeof(char));
            if (arr[rows] == NULL)
            {
                free(arr);
                fprintf(stderr, "Memory not allocated");
                exit(1);
            }
        }
        i++;
    }
    if (fileSize != 0 && space == 0)
    {
        //if last character was not space then terminate string and increase words
        words++;
        arr[rows][cells] = 0;
    }
    else if (fileSize != 0)
    {
        //delete extra row if last char ended in space
        rows--;
        arr = (char **)realloc(arr, (rows + 1) * sizeof(char *));
        if (arr == NULL)
        {
            free(arr);
            fprintf(stderr, "Memory not allocated");
            exit(1);
        }
    }

    if (fileSize != 0 && space == 0 && arr[rows][cells] != ' ')
    {
        //adds a space if the last words did not finish in a space
        arr[rows][cells] = ' ';
        cells++;
        arr[rows] = (char *)realloc(arr[rows], (cells + 1) * sizeof(char));

        arr[rows][cells] = 0;
    }

    int (*ptr)(const void *, const void *) = &cmp;
    int (*ptrF)(const void *, const void *) = &cmpF;

    if (upper == 1)
    {
        qsort(arr, words, sizeof(char *), ptrF);
    }
    else
    {
        qsort(arr, words, sizeof(char *), ptr);
    }

    char sp = ' ';
    for (int i = 0; i < words; i++)
    {
        int j = 0;
        while (arr[i][j] != ' ')
        {
            error = write(1, &arr[i][j], 1);
            checkError(error);
            j++;
        }
            error = write(1, &sp, 1);
            checkError(error);
    }

    for (int i = 0; i < words; i++)
    {
        free(arr[i]);
    }
    if (words == 0)
    {
        free(arr[0]);
    }

    free(buff);
    free(arr);
    exit(0);
}

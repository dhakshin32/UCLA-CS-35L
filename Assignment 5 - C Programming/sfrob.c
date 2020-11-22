#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

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

        char clearA = *a^ 42;
        char clearB = *b^ 42;

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

int main(void)
{
    //part 1: memory management
   
    int c;
    int rows = 0;
    int cells = 0;
    char **arr;
    arr = (char **)malloc((rows + 1) * sizeof(char *));
    arr[0] = (char *)malloc((cells + 1) * sizeof(char));

    int words = 0;
    int lastCells = 0;
    int space = 0;
    while ((c = getchar()) != EOF)
    {
        arr[rows][cells] = c;
        cells++;
        arr[rows] = (char *)realloc(arr[rows], (cells + 1) * sizeof(char));
        space = 0;
        if (c == ' ')
        {
            words++;
            space = 1;
            arr[rows][cells] = '\0';
           
            rows++;
            lastCells = cells;
            cells = 0;
            arr = (char **)realloc(arr, (rows + 1) * sizeof(char *));
            arr[rows] = (char *)malloc((cells + 1) * sizeof(char));
        }
    }
    if (space == 0)
    {
        //if last character was not space then terminate string and increase words
        words++;
        arr[rows][cells] = 0;
    }
    else
    {
        //delete extra row if last char ended in space
        rows--;
        arr = (char **)realloc(arr, (rows + 1) * sizeof(char *));
    }

    if (space == 0 && arr[rows][cells] != ' ')
    {
        //adds a space if the last words did not finish in a space
        arr[rows][cells] = ' ';
        cells++;
        arr[rows] = (char *)realloc(arr[rows], (cells + 1) * sizeof(char));
        arr[rows][cells] = 0;
    }

    int (*ptr)(const void *, const void *) = &cmp;


       qsort(arr, words, sizeof(char *), ptr);
    
    
    for (int i = 0; i < words; i++){
        if(arr[i][0]!=' '){
            printf("%s", arr[i]);
        }
    }

    for (int i = 0; i < words; i++){
       free(arr[i]);
    }

    if(words==0){
        free(arr[0]);
    }

    free(arr);
}

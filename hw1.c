#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

int getArrayIndex(char* word, int cacheSize)
{
    int sum = 0;
    
    for (int i = 0; i < strlen(word); i++)
    {
        sum += *(word + i);
    }
    
    return sum % cacheSize;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid arguments\n");
        fprintf(stderr, "USAGE: %s <x> <filename>\n", *argv);
        return EXIT_FAILURE;
    }
	
	for (int i = 0; i < 400; i++)
	{
	    if (*(*(argv + 1) + i) == '\0')
	    {
	        break;
	    }
	    
	    if (!isdigit(*(*(argv + 1) + i)))
	    {
	        fprintf(stderr, "ERROR: Cache size error\n");
	        return EXIT_FAILURE;
	    }
	}
    
    int cacheSize = atoi(*(argv + 1)); //Size of hash table to store words, given by arg2
    
    char** wordCache = calloc(cacheSize, sizeof(char*));
    
    FILE* f = fopen(*(argv + 2), "r");
    
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: fopen() failed\n");
        return EXIT_FAILURE;
    }
    
    char* buffer = calloc(128, sizeof(char));
    
    char c = fgetc(f);
    int wIndex = 0;
    
    while (c != EOF)
    {
        if (isalnum(c))
        //Build the word one character at a time as long as they are alphanumeric
        {
            *(buffer + wIndex) = c;
            wIndex++;
        }
        else
        //When a non-alphanumeric character is reached, cap off the string with the
        //null character and if the word has at least two characters in it add it to
        //the cache
        {
            *(buffer + wIndex) = '\0';
            
            if (wIndex >= 2)
            {
                int cIndex = getArrayIndex(buffer, cacheSize);
                
                printf("Word \"%s\" ==> %d ", buffer, cIndex);
                
                if (*(wordCache + cIndex) == NULL)
                {
                    char* word = calloc(wIndex + 1, sizeof(char));
                    strcpy(word, buffer);
                    *(wordCache + cIndex) = word;
                    
                    printf("(calloc)\n");
                }
                else
                {
                    char* word = realloc(*(wordCache + cIndex), (wIndex + 1) * sizeof(char));
                    strcpy(word, buffer);
                    *(wordCache + cIndex) = word;
                    
                    printf("(realloc)\n");
                }
            }
            
            wIndex = 0;
        }
        
        c = fgetc(f);
    }
    
    for (int i = 0; i < cacheSize; i++)
    //Print the final contents of each slot in the cache
    {
        if (*(wordCache + i))
        {
            printf("Cache index %d ==> \"%s\"\n", i, *(wordCache + i));
        }
    }
    
    //Free up all allocated memory and close the input file
    free(buffer);
    
    for (int i = 0; i < cacheSize; i++)
    {
        free(*(wordCache + i));
    }
    
    free(wordCache);
    
    fclose(f);
    
    return EXIT_SUCCESS;
}
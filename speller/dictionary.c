// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 27;

const unsigned int SINGLE_QUOTE = 39;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashedChar = hash(word);

    node *currentNode = table[hashedChar];
    while (currentNode != NULL)
    {
        if (strcasecmp(currentNode->word, word) == 0)
        {
            return true;
        }
        currentNode = currentNode->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    char firstChar = word[0];

    if (firstChar == SINGLE_QUOTE)
    {
        return 26;
    }

    return toupper(word[0]) - 'A';
}

// Utility to make a new node
node *createNewNode(char *newWord)
{
    node *newNode = malloc(sizeof(node));
    if (newNode == NULL)
    {
        printf("Couldn't allocate enough memory for a new node");
        return 0;
    }

    strcpy(newNode->word, newWord);
    newNode->next = NULL;

    return newNode;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char *word = malloc(sizeof(char) * (LENGTH + 1));

    while (fgets(word, LENGTH + 2, file) != NULL)
    {
        word[strlen(word) - 1] = '\0';
        int hashedValue = hash(word);

        node *targetNode = table[hashedValue];
        node *newNode = createNewNode(word);

        if (targetNode == NULL)
        {
            table[hashedValue] = newNode;
        }
        else
        {
            newNode->next = targetNode;
            table[hashedValue] = newNode;
        }
    }

    int result = feof(file);
    free(word);
    fclose(file);
    return result;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int counter = 0;

    for (int i = 0; i < sizeof(table); i++)
    {
        if (table[i] != NULL)
        {
            for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
            {
                counter++;
            }
        }
    }

    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *currentNode = table[i];
        while (currentNode != NULL)
        {
            node *tmp = currentNode->next;

            if (tmp != NULL)
            {
                free(currentNode);
            }
            printf("%s\n", currentNode->word);
            currentNode = tmp;
        }
        free(currentNode);
    }

    return size();
}

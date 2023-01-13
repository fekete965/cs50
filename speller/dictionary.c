// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
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

    // Check for single quote and return a fallback index
    if (firstChar == SINGLE_QUOTE)
    {
        return 26;
    }

    return toupper(word[0]) - 'A';
}

// Utility to make a new node
node *createNewNode(char *newWord)
{
    // Allocate memory for a new node
    node *newNode = malloc(sizeof(node));
    // Check for null pointer
    if (newNode == NULL)
    {
        printf("Couldn't allocate enough memory for a new node");
        return 0;
    }
    // Copy the incoming word as a char[] and assign it to the node's word property
    strcpy(newNode->word, newWord);
    newNode->next = NULL;

    return newNode;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open a dictionary
    FILE *file = fopen(dictionary, "r");
    // Checking for null file pointer
    if (file == NULL)
    {
        return false;
    }

    // Allocate memory for a word using the known longest word's length + 1
    char *word = malloc(sizeof(char) * LENGTH + 1);

    // Read the dictionary line by line
    while (fgets(word, LENGTH + 2, file) != NULL)
    {
        // Remove trailing \n (new line) character
        word[strlen(word) - 1] = '\0';
        // Get the hashed first character
        int hashedValue = hash(word);
        // Get the node under the hash number
        node *targetNode = table[hashedValue];
        // In any case, create a new node
        node *newNode = createNewNode(word);
        // Check for existing nodes using the hash number
        if (targetNode == NULL)
        {
            // If we have no node, assign the new node to the hash index
            table[hashedValue] = newNode;
        }
        else
        {
            // If we already have a node under the hash index,
            // the new node's next propety should point to the existing node,
            // and the new node should become the first node under the hash index
            newNode->next = targetNode;
            table[hashedValue] = newNode;
        }
    }
    // Free allocated memory for the word
    free(word);
    // Check for end of file
    int result = feof(file);
    // Close the file
    fclose(file);
    // Send back result
    return result;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Define a counter
    int counter = 0;
    // Traverse the table
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            // Traverse through each node
            for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
            {
                // If there is a node, increase the counter
                counter++;
            }
        }
    }
    // Return final word count
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Traverse through the hash table
    for (int i = 0; i < N; i++)
    {
        // Get the current node
        node *currentNode = table[i];
        // If the node exists
        while (currentNode != NULL)
        {
            // Save the next node temporarily
            node *tmp = currentNode->next;

            // Free the memory if needed
            free(currentNode);
            // Assign current note to temp
            currentNode = tmp;
        }
        // Free the parent node
        free(currentNode);
    }

    return true;
}

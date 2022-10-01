#include <cs50.h>
#include <stdio.h>
#include <string.h>

int string_length(string input);

int main(void)
{
    string str = get_string("Type in a string: ");
    int length = strlen(str);

    printf("%i\n", length);
}

// Custom implementation
int string_length(string s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        i++;
    }

    return i;
}

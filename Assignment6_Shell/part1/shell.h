#ifndef MYSHELL_H
#define MYSHELL_H
#define BUFFER_SIZE 80
#define COMMAND_NUM 7

// Create the struct for historyQueue
typedef struct node{
    char* line;
    struct node* next;
}node_t;

typedef struct queue{
    node_t* back;
    node_t* front;
    int size;
}queue_t;

// Create a queue
queue_t* createQueue();

// Enqueue an item
void queueEnqueue(queue_t* q, char* item);

// Dequeue an item
char* queueDequeue(queue_t* q);

// Free queue
void freeQueue(queue_t* q);

// Create and malloc memory for the array
char** createArray();

// Free array memory
void freeArray(char** array);

// Print error message if command not found
void printError();

// A signal handler
void sigint_handler(int sig);

// Read in user input and update the history queue
int readLine(char* line, queue_t* historyQ);

// Reset line and inputCommand
void reset(char* line);

// Check if the input line contains valid commands
int isCommand(char* line, char** commandArray);

// Parse the valid command line input into array of command strings
// The returned value indicates the actual size of the array
int parseLine(char* line, char** inputCommand);

// Parse the pipe commands at the sides of the pipe sign
void parsePipe(char** inputCommand, char(*pipeLeft)[BUFFER_SIZE],
char(*pipeRight)[BUFFER_SIZE], int* pipeSize);

// Execute commands
void executeCommand(char** inputCommand, int commandCount);

// Execute pipe commands
int executePipeCommand(char(*pipeLeft)[BUFFER_SIZE],
char(*pipeRight)[BUFFER_SIZE], int* pipeSize);

// Create cd function
void cd(char** inputCommand, char** commandArray);

// Create echo function
void echo(char** inputCommand, char** commandArray);

// Create help function
void help(char** inputCommand, char** commandArray);

// Create pwd function
void pwd(char** inputCommand, char** commandArray);

// Create history function
void printHistory(queue_t* historyQ);

#endif
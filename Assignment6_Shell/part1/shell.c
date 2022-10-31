// Ameya Santosh Gidh
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  alarm(180); 
  // We Initialize history here.
  queue_t* historyQ=createQueue();
   // We set input string array
  char line[BUFFER_SIZE];

  // We store all the commands here
  // Indexes 4, 5, 6 show the special function call commands
  char* commandArray[]={"cd", "echo", "help", "pwd", "history", "exit", "|"};
  
  // Initialize an array of built-in command function pointers 
  void (*commandPfnArray[COMMAND_NUM-1])(char** inputCommand, 
  char** commandArray)={&cd, &echo, &help, &pwd};
  // Signal Handler 
  signal(SIGINT, sigint_handler); 
	
  while(1){
      // Input command and make the shell ready for the next command
      char** inputCommand=createArray();
      reset(line);
      // Input user command	  
      readLine(line, historyQ);  
      // -1 if command execution fails else returns the command
      int task=isCommand(line, commandArray);
      // Built-in command is called
      int commandCount = parseLine(line, inputCommand); 
      if(task >= 0 && task < 4){ 
          commandPfnArray[task](inputCommand, commandArray);
      }
      else if(4==task){ 
	// history command
        printHistory(historyQ);
      }
      else if(5==task){ 
          // exit command
          freeArray(inputCommand);
          break;
      }
      else if(6==task){ 
	   // Pipe command 
           char pipeLeft[COMMAND_NUM][BUFFER_SIZE];
          char pipeRight[COMMAND_NUM][BUFFER_SIZE];
          // Store the actual number of commands at each side of the pipe
          int pipeSize[2];  
          parsePipe(inputCommand, pipeLeft, pipeRight, pipeSize);
          executePipeCommand(pipeLeft, pipeRight, pipeSize); 
      }
      // Next command is called
      else{
          executeCommand(inputCommand, commandCount); 
      }
      freeArray(inputCommand);
  }
  // Free Up Memory
  freeQueue(historyQ);
  return 0;
}

// Creating a queue dataStructure
queue_t* createQueue(){
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    // Initialize the fields of queue_t 
    queue->back = NULL;
    queue->front = NULL;
    queue->size = 0;
    return queue;    
}

// Enqueue a new command
void queueEnqueue(queue_t* q, char* item){
    // Dequeue the oldest record if the q is full
    if(q->size == BUFFER_SIZE){
        queueDequeue(q);
    }
    node_t* newNode=(node_t*)malloc(sizeof(node_t));
    newNode->line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    // Copy the item to the newNode 
    strcpy(newNode->line, item);
    newNode->next=NULL;
    // If q is empty, update the head
    if(q->size==0){
        q->front=newNode;
        q->back=newNode; 
    }else{
        q->back->next=newNode;
        q->back=newNode; 
    }
    // Update the size of q
    (q->size)++;
}

char* queueDequeue(queue_t* q){
    if(q->size != 0){
       // Dequeue an item
        char* item=(char*)malloc(sizeof(char)*BUFFER_SIZE);
        strcpy(item, q->front->line);
        node_t* temp;
        temp=q->front;
        q->front=q->front->next;
        if(q->size==1){
            q->back=NULL;    
        }
        // Update the new front position
        // Update q size
        (q->size)--;
        // Free the dequeued item's line and node memory
        free(temp->line);
        free(temp);
        return item;
    }
    return NULL;
}

void freeQueue(queue_t* q){
    int i;
    node_t* iter=q->front;
    while(iter!=NULL){
        node_t* temp=iter;
        iter=iter->next;
        free(temp->line);
        free(temp);
    }
    free(q);
}

// Malloc memory function for inputCommand array
char** createArray(){
    char** buffer;
    buffer=(char**)malloc(sizeof(char*)*BUFFER_SIZE);
    int i;
    for(i=0; i<BUFFER_SIZE; i++){
        buffer[i]=(char*)malloc(sizeof(char)*BUFFER_SIZE);
        strcpy(buffer[i], "\0");
    }
    return buffer; 
}
// Prints error if the shell fails to recognize meaning of the givenn input.
void printError(){
    printf("mini-shell>Command not found--Did you mean something else?\n");
}

// Create a signal handler to interrupt
void sigint_handler(int sig){
    write(1, "\nmini-shell terminated\n", 23);
    exit(0);
}

// Read in the command line input
int readLine(char* line, queue_t* historyQ){
    printf("mini-shell>");
    fgets(line, BUFFER_SIZE, stdin);
    // Update the history queue
    queueEnqueue(historyQ, line);
}

// Search the input command in the command collection
int isCommand(char* line, char** commandArray){
    // Check if contains pipe sign 
    if(0!=strstr(line, commandArray[6])){
        return 6;
    }
    int i;
    char copy[BUFFER_SIZE];
    for(i=0; i<COMMAND_NUM-1; i++){
        // Check if the line contains the valid command, 
        // if so, return the index in commandArray
        strcpy(copy, commandArray[i]);
        strcat(copy, "\n");
        int result = strcmp(line, copy);
        // Check if the substring is a valid command wrapped without other letters
        if(result != 0){
            char copySpace[BUFFER_SIZE];
            strcpy(copySpace, commandArray[i]);
            strcat(copySpace, " ");
            if(strstr(line, copySpace) != NULL){ 
                return i;
            }
        }
        else if(result == 0){
            return i; 
        }
    }
    return -1;
}

// Reset collections for the new loop
void reset(char* line){
    // Reset line 
    int i;
    for(i=0; i<BUFFER_SIZE; i++){
        line[i]='\0';
    }
}

// Parse the valid command line input into words
int parseLine(char* line, char** inputCommand){
    char copy[BUFFER_SIZE];
    strcpy(copy, line);
    int count=0;
    // Assign correspinding delimiter based on whether the line has | 
    char delimiter[3]; 
    if(NULL==strchr(copy, '|')){
        delimiter[0]='\n';
        delimiter[1]=' ';
        delimiter[2]='\0';
    }else{
        delimiter[0]='\n';
        delimiter[1]='|';
        delimiter[2]='\0';
    }
    // Split the line using the delimiter 
    char* word=strtok(copy, delimiter);
    while(NULL!=word){
        strcpy(inputCommand[count], word);
        count++;
        word=strtok(NULL, delimiter);
    }
    return count;
}

// Parse the pipe commands
void parsePipe(char** inputCommand, char(*pipeLeft)[BUFFER_SIZE], 
char(*pipeRight)[BUFFER_SIZE], int* pipeSize){ 
    // Clear pipeLeft, pipeRight, and pipeSize
    int i;
    for(i=0; i<COMMAND_NUM; i++){
        strcpy(pipeLeft[i], "\0");
        strcpy(pipeRight[i], "\0");
    }
    pipeSize[0]=0;
    pipeSize[1]=0;
    // Store commands to pipeLeft and pipeRight from inputCommand strings
    char left[BUFFER_SIZE];
    char right[BUFFER_SIZE];
    strcpy(left, inputCommand[0]);
    strcpy(right, inputCommand[1]);
    int count=0;
    char* delimiter=" \n";
    char* wordLeft=strtok(left, delimiter);
    while(NULL!=wordLeft){ // split and store the left arguments
        strcpy(pipeLeft[count], wordLeft);
        count++;
        wordLeft=strtok(NULL, delimiter);
    }
    pipeSize[0]=count;
    count=0;
    char* wordRight=strtok(right, delimiter);
    while(NULL!=wordRight){ // split and store the right arguments
        strcpy(pipeRight[count], wordRight);
        count++;
        wordRight=strtok(NULL, delimiter);
    } 
    pipeSize[1]=count;
}

// Execute commands
void executeCommand(char** inputCommand, int commandCount){
    // Store the child process id
    pid_t childId;
    // Create a fork
    childId=fork();
    // Check if the fork is successfully created
    if(-1==childId){
        printError(); 
        exit(1);
    }
    if(0==childId){
        char* argv[commandCount+1];
        int i;
        for(i=0; i<commandCount; i++){
            argv[i]=inputCommand[i];
        }
        argv[commandCount]=NULL;
        if(-1==execvp(argv[0], argv)){
            printError();
            exit(1);
        }
    }else{
        wait(NULL);
    }
}

// Execute pipe commands
int executePipeCommand(char(*pipeLeft)[BUFFER_SIZE], 
char(*pipeRight)[BUFFER_SIZE], int* pipeSize){
    int fd[2]; 
    pipe(fd);
    pid_t child1Pid, child2Pid; // Store the child process id 
    int child1_status, child2_status; 
    child1Pid=fork(); // Create the first fork
    if(-1==child1Pid){
        printError();
        exit(1);
    }
    if(0==child1Pid){ // Child process for the left side argument
        dup2(fd[1], 1); // Duplicate fd[1] to STDOUT
        close(fd[0]);
        close(fd[1]);
        int size=pipeSize[0];
        char* argv[size+1];
        int i;
        for(i=0; i<size; i++){
            argv[i]=pipeLeft[i];
        }
        argv[size]=NULL;
        if(-1==execvp(argv[0], argv)){
            printError();
            exit(1);
        }
    }else{ // Parent process
        child2Pid=fork(); // Create another fork
        if(-1==child2Pid){ // Check if the child is successfully created
            printError();
            exit(1);
        }
        if(0==child2Pid){ // child process for the right side of pipe
            dup2(fd[0], 0);
            close(fd[1]);
            close(fd[0]);     
            int size=pipeSize[1];
            char* argv[size+1];
            int i;
            for(i=0; i<size; i++){
                argv[i]=pipeRight[i];
            }
            argv[size]=NULL;
            if(-1==execvp(argv[0], argv)){
                printError(); 
                exit(1);
            }
        }
    }
    close(fd[0]); // Close file descriptors 
    close(fd[1]);
    waitpid(child1Pid, &child1_status, 0);
    waitpid(child2Pid, &child2_status, 0);
    return 0;        
}

// cd function
void cd(char** inputCommand, char** commandArray){
   chdir(inputCommand[1]);    
   pwd(inputCommand, commandArray);
}

// echo function
void echo(char** inputCommand, char** commandArray){
    char* blank="\n";
    if(inputCommand[1]=="\0"){
        printf("%s", blank);
    }else{
        int i;
        for(i=1; i<BUFFER_SIZE; i++){
            if(inputCommand[i][0]=='\0'){
                break;
            }
            printf("%s ", inputCommand[i]);
        }
    }
    printf("\n");
}

// help function
void help(char** inputCommand, char** commandArray){
    printf("Yutong He's Mini Shell\n");
    printf("Here is the list of the built-in commands:\n");
    int i;
    // Exclude the pipe command from the commandArray
    for(i=0; i<COMMAND_NUM-1; i++){
        printf(">%s\n", commandArray[i]);
    }
}

// pwd function
void pwd(char** inputCommand, char** commandArray){
    char buff[BUFFER_SIZE];
    printf("%s\n", getcwd(buff, BUFFER_SIZE)); 
}

// history function
void printHistory(queue_t* historyQ){
    printf("Yutong He's Mini Shell\n");
    printf("Here is the history of the commands the user typed in:\n");
    int i=0;
    node_t* temp=historyQ->front;
    while(temp!=NULL){
        printf("%d %s", i, temp->line);
        i++;
        temp=temp->next;
    }
}

// Free memory of the array
void freeArray(char** array){
    int i;
    for(i=0; i<BUFFER_SIZE; i++){
        free(array[i]);
    }
    free(array);
}

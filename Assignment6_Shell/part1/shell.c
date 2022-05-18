
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_BUFFER_SIZE 80

void sigint_handler(int sig){	
	write(1,"\n shell1 ended\n",23);
	exit(0);
}

int cd_command(char **args) {

	if (args[1] == NULL) {
    		fprintf(stderr, "No argument for \"cd\"\n");
  	} else {
    		if (chdir(args[1]) != 0) {
      			perror("Eror");
    		}
  	}
  	return 1;
}


int findPipe(char** arg) {
        int i = 0;
        while(arg[i] != NULL) {
       		if (strcmp(arg[i], "|") == 0) {
	   		return i;
        	}
        	i ++;
    	}
   	return -1;
}

char** prePipe(char** arg) {
  	int index = findPipe(arg); 
  	char** result = malloc(sizeof(char*) * 20); 
  	int i;
  	while(i<index) {
		result[i] = arg[i];
		i++;
  	}
  	return result;
}


char** postPipe(char** arg) {
 	int index = findPipe(arg); 
  	char** result = malloc(sizeof(char*) * 20);
  	int i = index+1;
  	int j = 0;

  	while(arg[i] != NULL) {
		result[j] = arg[i];
		i++;
		j++;
  	}	
  	return result;
}

void executePipedCommand(char** arg1, char** arg2) {

	int fd[2];

	pid_t id;   

    	id = fork();
    	if (id == -1) {
        	printf("Fork failure\n");
       	 	exit(EXIT_FAILURE);
    	}else if (id == 0) {
		pipe(fd);
		if(fork() == 0) {
		    close(fd[0]);
		    dup2(fd[1], 1);	
		    execvp(arg1[0], arg1);
	            printf("Did not execute\n");
        	    exit(0);
	} 
        wait(NULL);
	close(fd[1]);
	dup2(fd[0], 0);
	execvp(arg2[0], arg2);
   	printf("Did not execute\n");
	exit(0);
	}
	wait(NULL);
}


void executeCommand(char* myargv[]) {
	int fd[2];

  	if (pipe(fd) == -1) {
        	printf("Pipe creation failure\n");
        	exit(EXIT_FAILURE);
   	}
	
	pid_t id;   
    	id = fork();
    	if (id == -1) {
        	printf("Fork failed\n");
       	 	exit(EXIT_FAILURE);
    	}

    	if (id == 0) {

        	close(fd[0]);
        	close(fd[1]);
		execvp(myargv[0], myargv);
		printf("Command not found\n");
		exit(0);
   	} else {
        	wait(NULL);
		close(fd[1]);
        	close(fd[0]);
    	}
}

int guessingGame() {
     	srand(time(NULL));
    	int randomNumber = rand() % 10 + 1;
    
   	printf("===============================\n");
    	printf("CPU Says: Pick a number 1-10\n");
    	printf("===============================\n");
    
    	int userGuess;
    	int count = 0;
    	int i;
   
    	for(i = 0; i < 5; i++){
        	printf("Make a guess: ");
        	scanf("%d", &userGuess);
        	count ++;
        	if(userGuess < randomNumber){
            		printf("No, Guess higher!\n");
            		if(i == 4){
                		printf("You lost the game.\n");
            		}
        	}else if(userGuess > randomNumber){
           		printf("No, Guess lower!\n");
            		if(i == 4){
                		printf("You lost the game.\n");
            		}
        	}else if(userGuess == randomNumber){
            		printf("You got it!\n");
            		break;
        	}
    	}
	getchar();
    	return 1;  
}


void help_command() {
	printf("Mini-shell \n");
	printf("Type a program name and arguments \n");
	printf("Built -in commands are: \n");
	printf("help\ncd\nexit\nguessinggame\n");
}

char** readCommand (char* line) {
	int i = 0;
	char **array = malloc(MAX_BUFFER_SIZE * sizeof(char*));
  	char** arg1;
  	char** arg2;
	if(!array) {
      		exit(EXIT_FAILURE);
    	}

    	char *p = strtok (line, " ");
    	while (p != NULL){
        	array[i] = p;
        	p = strtok(NULL, " \n");
        	i++;
    	}
    	char* path = strtok(array[i-1], "\n");
    	array[i-1] = path;
    	array[i] = NULL;

    	if((strcmp(array[0], "exit") == 0) && (array[1] == NULL))  {
		free(array);
		exit(0);
	} else if((strcmp(array[0], "help") == 0) && (array[1] == NULL)) {
	  	help_command();
	} else if ((strcmp(array[0], "cd") == 0) && (array[2] == NULL)) {
	  	cd_command(array);
	} else if ((strcmp(array[0], "guessinggame") == 0) && (array[1] == NULL))  {
		guessingGame();
	} else {
int index = findPipe(array);
    		if (index != -1) {
      			arg1 = prePipe(array);
      			arg2 = postPipe(array);
      			executePipedCommand(arg1, arg2);
      	        	free(arg1);
      	        	free(arg2);
    		} else {
      			executeCommand(array);   
   	
	 	}   
   	}
	return array;
}

int main(){

	char line[MAX_BUFFER_SIZE];
	printf("You can only terminate by pressing Ctrl+C\n");
	char** array;
	while(1){
		printf("mini-shell >> ");
fgets(line, MAX_BUFFER_SIZE, stdin);
      		char* command = line;
      		array = readCommand(command);
		memset(line, 0, MAX_BUFFER_SIZE);
		free(array);
	}
	return 0;
}

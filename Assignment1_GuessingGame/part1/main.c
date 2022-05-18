# include<time.h>
# include <stdio.h>
 
int guess_game(int *arr,int i)
    { 
        /*
         guess_game function takes
         an array , an integer to store the index of that array
        */
    
    srand(time(NULL));
     
    int count = 1;
    // count is used to count the number of guesses
      
    int guess;
    // guess is used to take the user input
     
    int r = (rand() % (10 - 1 + 1)) + 1;
    // r is the generated random number
     
    printf("Make a guess : ");
    scanf("%d",&guess);

    while(guess !=r || i == 5)
    {
        if(guess < r)
         {    
            printf("\n No guess higher!\n");
            count++;
        }
 
        else
        {
            printf("\n No guess lower!\n");
            count++;
        }
        
       printf("\n Make a guess : ");
       scanf("%d",&guess);
    }
    
    if (guess == r)
        {
            printf("\n You got it! \n");
             arr[i] = count;
             i++;
        }
 
    return 0;
    }

int main()
{  
    int arr[5];// arr is the array containing the
    int i; // i is the counter for the loop
    int j; // j is the counter for the loop
   
    for(i=0; i < 5; i++)
    {
         printf("==========================\n");
         printf("CPU Says: Pick a number 1-10\n");
         printf("===========================\n");
         guess_game(&arr,i);// calling the guess_game function
    }

    printf("====================================================\n");
    printf("|Here are the results of your guessing abilities|\n");
    printf("====================================================\n");

    for(j=0;j<5;j++)
    {
        printf("\n"); 
        printf("Game %d took you %d guesses ",j,arr[j]); 
    }
    return 0;
}


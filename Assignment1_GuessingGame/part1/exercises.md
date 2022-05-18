# Exercises

In order to start thinking a bit algorithmically, modify this file and answer the following questions. Be sure to add/commit/push any changes made to this file to our repository.

1. In the worst case in our guessing game--how many guesses could it take to get the right number if we had no hint at all? Explain.
2. In the worst case in our guessing game--how many guesses does it take to get the right number if we get a hint of 'higher or lower' when guessing numbers 1-10 **and** guess intelligently (always picking in the middle of the remaining set of numbers) ? *Hint* In your answer show this mathematically with the [log function](https://www.mathsisfun.com/algebra/logarithms.html).

## Answers

1. *Your answer to question 1*

    Answer: - In the worst case the game would take 10 guesses without hints as the user will input all the ten numbers. From 1 to 10 and in the worst case the random number might be 10  and assuming the user inputs them in ascending order and does not repeat a number. So the game will require to make ten guesses starting from 1 up to 10. 

    Answer is 10 guesses.



2. *Your answer to question 2*

    Answer: - If we guess intelligently and pick always the middle of the remaining set of numbers in the worst case the time will be given by the logn function. Where n is 10 and base for log function is 2.
    The game will need no more than 4 guesses to get the desired output
  
    Here we are dividing the numbers by 2 as we pick the middle of the remaining set of numbers. 

    1 = N / 2^x

    Here we find the middle element by dividing by 2
    Taking log on both sides we have

    Log(2^x) = logN

    X log2 =  logN   …..(logab  = b*loga)

    X = logN 
    
    Answer is 4 guesses

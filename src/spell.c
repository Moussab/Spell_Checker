#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/times.h>
#include "dictionary.h"


const int MAX_WORD_LEN = 45;

// default dictionary
char *WORDS = "words.txt";

// prototype
double calculate(struct rusage *, struct rusage *);


int main(int argc, char *argv[])
{


  // check for correct number of args
  if (argc < 3) {
    printf("Usage: speller [dict] file\n");
    return 1;
  }

  // determine dictionary to use
  char *dict = (argc > 2) ? argv[1] : WORDS;

  // load dictionary
  bool loaded = load(dict);

  // abort if dictionary not loaded
  if (!loaded) {
    printf("Could not load %s.\n", dict);
    return 2;
  }

  FILE *inputFiles[argc - 2];
  int i = 0;
  for (i = 2; i < argc; i++)
  {
      inputFiles[i-2] = fopen(argv[i], "r");
      if (inputFiles[i-2] == NULL) {
        printf("Could not open %s.\n", argv[i]);
        return 3;
      }
  }


  for (i = 2; i < argc; i++)
  {
    printf("|******************* Checking spelling for : %s *********************|\n", argv[i]);
    // prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[MAX_WORD_LEN+1];
    bool isLastWord = false; // necessary if last word not followed by a return or space

    //spell-check each word in file
    for (int c = fgetc(inputFiles[i-2]); c != EOF || !isLastWord; c = fgetc(inputFiles[i-2])) 
    {

      if (c == EOF) {
        isLastWord = true;
      }


      // allow alphabetical characters and apostrophes (for possessives)
      if (isalpha(c)) {
        // append character to word
        word[index] = c;
        index++;

        // ignore alphabetical strings too long to be words
        if (index > MAX_WORD_LEN) {

          // consume remainder of string
          while ((c = fgetc(inputFiles[i-2])) != EOF && isalpha(c));

            // prepare for new word
            index = 0;
          }
      }  // ignore words with numbers (like MS Word)
      else if (isdigit(c)) {
        // consume remainder of string until next non-alphanumeric character
        while ((c = fgetc(inputFiles[i-2])) != EOF && isalnum(c));

          // prepare for new word
        index = 0;
      } // we must have found a whole word
      else if (index > 0 && (c != '\'' && index > 0)) {
        // terminate current word
        word[index] = '\0';

        // update counter
        words++;

        bool misspelled = !check(word);

        // print word if misspelled
        if (misspelled) {
          printf("Spelling error: %s\n", word);
          misspellings++;
        }

        // prepare for next word
        index = 0;
      }
    }

    // check for error
    if (ferror(inputFiles[i-2])) {
      fclose(inputFiles[i-2]);
      printf("Error reading %s.\n", argv[i]);
      return 4;
    }

    // close file
    fclose(inputFiles[i-2]);   
  }

  return 0;
}
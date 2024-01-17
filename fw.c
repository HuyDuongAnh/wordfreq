/* Programmers: Huy Duong & Visalbotr Chan
 * Course: CPE 357
 * Assignment 2 - wf
 *
 * The command line of the program will consist of an optional argument of
 * the form -n <num> to give the number of words, and an optional set of
 * filenames. 
 *
 * If the -n argument is not given, the program should default to
 * STDTEN, and if no filenames are given, the program should take its input from
 * stdin.
 *
 * In case the argument to -n is not an integer, the program should report 
 * the error,print a usage message, and stop.
 *
 * If, while processing files, you encounter a file that cannot be opened, 
 * print an error message explaining why, and continue with the rest.
 *
 * The output of this program will start with one line reading, “The top k
 * words (out of n) are:” where k is the number requested and n is the 
 * numberof unique words encountered in the input.
 *
 * After that header line, the individual words are listed in descending 
 * order on individual lines as the count, right justified in a field nine 
 * characters wide, a space, then the word.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"

int main(int argc, const char *argv[]){
    word *hash_table[HASHSIZE] = {NULL};
    int numberOfListedWord = 0;
    FILE *fp = NULL;
    // check if the user wants to pass input from stdin
    if(argc == 1){  
        fp = stdin;  
        process_word_to_hashtable(fp, hash_table);
        numberOfListedWord =STDTEN;

    }
    // when the argument "-n" is passed
    else if(!strcmp(argv[1],"-n")){
        if(atoi(argv[2])){
            numberOfListedWord = atoi(argv[2]);
            for(int i = 0; i < argc-3; i++){
                if(read_file_check(argv[i+3])){
                    fp = fopen(argv[i+3], "r");
                    process_word_to_hashtable(fp, hash_table);
                    fclose(fp);
                }
            }
        }// when argument "-n" is passed but the line number is not given
        else{ 
            perror("usage: fw [-n num] [ file1 [file 2 ...]]\n");
            return 1;
        }
    }
    else { // when the user doesn't specified number of line
        numberOfListedWord = STDTEN; // STDTEN is a default number of Listed word
        for(int i = 0; i < argc-1; i++){
            if(read_file_check(argv[i+1])){
                fp = fopen(argv[i+1], "r");
                process_word_to_hashtable(fp, hash_table);
                fclose(fp);
            } 
        }   
    }
    word *tempList;
    tempList = hash_to_list(hash_table); 
    sort_print(numberOfListedWord, hash_to_list(hash_table), 
            hash_table);

}



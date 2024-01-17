#include <stdio.h>
#include "word.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

unsigned int hash_function(char *s){
    unsigned int hashval;
    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}
//this function take a file and process its words 
//to a hash table
void process_word_to_hashtable(FILE *file, word **hash_table)
{
    char *myC;
    while((myC= read_word(file))){
        evaluate_word(myC, hash_table); 
    }

}

/*call look_up_word to see if the word is in hash table
  if not, call new_word and store return address into hashtable.
  if the bucket[index] is empty at new struct word 
  else add it to the bottom chain (by calling append_word())
  else if it's already in the hashtable, increment frequency of the word
 * */
void evaluate_word (char *str, word **hash_table){
    word *address = look_up_word(str, hash_table);
    if(!address)
        append_word(str, hash_table);
    else
        address->freq++;
}

/* allocate memory (check error for malloc) for struct word, store str 
 * into wd, then return word.
 * */
word *new_word(char *str){
    int num_word = 1;
    word* w_new = calloc(num_word, sizeof(word));
    if(w_new == NULL){
        perror("calloc: failed\n");
    }
    else{
        w_new->wd = str;
        w_new->freq = 1;
        w_new->next = NULL;
    }
    return w_new;
}

/* call hash function to get the index of bucket, and traverse through
 * the chain until node points to NULL, add new word, increase frequency
 * Note from Huy: make sure to check w_new == NULL after calling new_word
 * */
void append_word(char *str, word **hash_table){
    int index = hash_function(str); 
    word *bucket = hash_table[index];
    word *bucketPrev = NULL; 
    if (bucket == NULL)
    {
        hash_table[index] = new_word(str); 
    }else{
        while((bucket) != NULL){
            bucketPrev = bucket;
            bucket = bucket -> next;  
        }	
        bucketPrev->next = new_word(str);
    }
}
/* call hash_function to get the index of str, traverse through the chain
 * if the word isn't found, return NULL
 * if the word is found return the adress of the word 
 * */


word *look_up_word(char *str, word **hash_table){
    unsigned int index = hash_function(str);
    word *bucket = hash_table[index];
    if(bucket == NULL)
    {
        return NULL;
    }
    while((bucket->next != NULL) &&(strcmp(bucket->wd, str))  )
    {
        bucket = bucket -> next; 
    }
    if(!(strcmp(bucket->wd, str)))
    {
        return bucket;
    }else
    {
        return NULL;
    }

}



/* count how many words stored inside hash_table
 * */
int count_hash(word **hash_table){
    int word_num = 0;
    struct node *next_node = NULL;
    for(int i = 0; i < HASHSIZE; ++i){
        if(hash_table[i]){
            next_node = hash_table[i];
            while(next_node){
                word_num++;
                next_node = next_node->next;
            }
        }
    }
    return word_num;
}


/* traverse from left to right of the hash table, at each location, 
 * traverse from top to bottom, increase word count 
 * create array with the size of word count
 * traverse through hash table (left to right, then top to bottom), 
 * and if hash_table[index] is !NULL put each word at the index into 
 * new array list.
 * */
word *hash_to_list(word **hash_table){
    word *list = calloc(count_hash(hash_table), sizeof(word));
    struct node *next_node = NULL;
    int list_index = 0;
    for(int i = 0; i < HASHSIZE; ++i){
        if(hash_table[i]){
            next_node = hash_table[i]; 
            while(next_node){
                list[list_index] = *next_node;
                next_node = next_node->next;
                list[list_index].next = NULL;
                list_index++;
            }   
        }
    }
    return list;
}



/* return negative number if a comes before b
 * return positvie if a comes after b
 * return 0 if a and b are equal 
 */
int wordcmp(word *a, word *b){
    if(a->freq == b->freq)
    {
        return strcmp(b->wd, a->wd);
    }
    else if( a->freq > b->freq)
    {
        return -1;
    }else
    {
        return 1;
    }
}

/* call built in sort function and print
 * */
void sort_print(int user_num, word *list, word **hash_table){
    int cnt = count_hash(hash_table);
    int loopNum = 0; 
    qsort(list, cnt, sizeof(word), 
            (int (*) (const void*, const void *)) wordcmp);

    printf("The top %d words (out of %d) are:\n", user_num, cnt);

    if(user_num > cnt){
        loopNum = cnt;
    }else{
        loopNum = user_num; 
    }
    for(int i = 0; i < loopNum; ++i){
        printf("%*d %s\n",WIDTHLENGTH, list[i].freq, list[i].wd);
    }
}

/* traverse through list and free each word and its next pointer
 * */
void clean_up_list(word *list){
    word *temp;
    while(list != NULL){
        temp = list;
        list = list->next;
        free(temp);
    }
}

/* traverse horizontally and vertically to free each element in 
 * hash_table
 * Status: Incomplete
 * */
void clean_up_hash(word **hash_table){
}

int read_file_check(const char* fileName){
    FILE *fp;
    fp = fopen(fileName, "r"); 
    if (fp == NULL){
        fprintf(stderr, "%s: No such file or directory\n", fileName); 
        return 0;  
    }   
    else{
        fclose(fp); 
        return 1;
    }   
}
/*read word return a char pointer to a string if word is found in text
 *it will skip any special charcters and numbers such as \n , !: 1-9
 *it will return a NULL pointer when it hits EOF
 *caller needs to free the buffer
 */
char *read_word(FILE *file){
    char *buffer = NULL; 
    int array_size = 128; 
    buffer = (char*)calloc(sizeof(char), array_size);
    if(buffer == NULL) 
    {
        perror("malloc: failed");
        return NULL;

    }
    int count = 0; 
    int c = 0; 
    while(((c = getc(file)) != EOF)){

        while(!isalpha(c) && (*buffer == 0))
        {   
            if( c!= EOF)
            {
                c = getc(file); 
            }else{
                break;
            }
        }
        if((c == EOF) || !(isalpha(c))){
            break;
        }		
        if(count < array_size){
            buffer[count] = tolower(c);
            ++count;
        }else{
            array_size = array_size *2;
            buffer = realloc(buffer, array_size);
            if(buffer == NULL)
            {
                perror("calloc: failed");
                return NULL;
            } 
            buffer[count] = tolower(c); 
            ++count; 
        }

    }
    if(c == EOF)
    {

        buffer = NULL;
    }
    return buffer;
}


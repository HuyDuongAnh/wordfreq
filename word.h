#ifndef WORD_H
#define WORD_H
#define HASHSIZE 101 

typedef struct node{
   char *wd;
   int freq;
   struct node *next;
}word;

unsigned int hash_function(char *);                 //done
word *new_word(char *);                             //Huy
void evaluate_word(char *, word **);                //Huy
void append_word(char *, word **);                  //Visal
word *look_up_word(char *, word **);                //Visal
int count_hash(word **);                            //Huy
int count_list(word *);                             //Huy
word *hash_to_list(word **);                        //Huy
int wordcmp(word* , word*);                         //Visal & Huy
void sort_print(int ,word *);                       //Huy
void clean_up_list(word *);                         //Huy
void clean_up_hash(word **);                        //Huy
char *read_word(FILE *);                            //done
int read_file_check(const char*);                   //done
void process_word_to_hashtable(FILE *, word **);    //Visal
#endif

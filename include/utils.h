#ifndef UTIL_H
#define UTIL_H

#define MAX_CLASSES 2
#define MAX_WORDS 4000
#define HASH_SIZE 1048859
#define MAX_WORD_LEN 20

struct Vocabulary {
  char *word;
  int classes[MAX_CLASSES];
  struct Vocabulary *next;
};

struct ClassInfo {
  char *class_name;
  int word_count;
  int document_count;
};

extern struct Vocabulary *hashtable[HASH_SIZE];
extern struct ClassInfo classes[MAX_CLASSES];
extern int total_entries;

void to_lowercase(char *word);

unsigned int hash(const char *word);

void clean_word(char *word);
#endif

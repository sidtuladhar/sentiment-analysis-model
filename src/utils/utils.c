#include <ctype.h>
#include "utils.h"

void to_lowercase(char *word) {
  for (int i = 0; word[i]; i++) {
    word[i] = tolower(word[i]);
  }
}

unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++)) {
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  }
  return hash % HASH_SIZE;
}

void clean_word(char *word) {
  int i, j = 0;
  for (i = 0; word[i] != '\0'; i++) {
    if (isalnum(word[i])) {
      word[j]= word[i];
      j++;
    }
  }
  word[j] = '\0'; 
}

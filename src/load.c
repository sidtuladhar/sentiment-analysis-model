#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"
#include "utils.h"

void load() {

  FILE *file = fopen("./data/naive_bayes_model.txt", "r");
  if (!file) {
    perror("Error opening file");
    return;
  }
  
  char word[MAX_WORD_LEN];
  int pos_count, neg_count;
  while (fscanf(file, "%s %d %d", word, &pos_count, &neg_count) == 3) {
    unsigned int index = hash(word);
    struct Vocabulary *new_entry = malloc(sizeof(struct Vocabulary));
    new_entry->word = strdup(word);
    new_entry->classes[0] = pos_count;
    new_entry->classes[1] = neg_count;
    new_entry->next = hashtable[index];
    hashtable[index] = new_entry;
    total_entries++;
  }  
  
  fclose(file);
}

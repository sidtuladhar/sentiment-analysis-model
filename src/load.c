#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"
#include "utils.h"
#include "cJSON.h"
#include "preprocessing.h"


void load() {
  init_classes();

  FILE *file = fopen("./data/naive_bayes_model.txt", "r");
  if (!file) {
    perror("Error opening file");
    return;
  }

  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file)) {
    char word[MAX_WORD_LEN];
    int pos_count, neg_count;

    int result = sscanf(line, "%s %d %d", word, &pos_count, &neg_count);
    if (result == 3) {
      // printf("Word: %s, Pos Count: %d, Neg Count: %d\n", word, pos_count, neg_count);
      unsigned int index = hash(word);
      struct Vocabulary *new_entry = malloc(sizeof(struct Vocabulary));
      new_entry->word = strdup(word);
      new_entry->classes[0] = pos_count;
      new_entry->classes[1] = neg_count;
      new_entry->next = hashtable[index];
      hashtable[index] = new_entry;
      total_entries++;
      continue;
    }

    result = sscanf(line, "%d %d", &classes[0].word_count, &classes[1].word_count);
    if (result == 2 && classes[0].word_count == 0) {
      // printf("Class word counts: %d, %d\n", classes[0].word_count, classes[1].word_count);
      continue;
    } else {
      fscanf(file, "%d %d", &classes[0].document_count, &classes[1].document_count);
      continue;
    }

    printf("Error format: %s\n", line);
  } 
  fclose(file);
  printf("Finished loading! Lines: %d\n", total_entries);
  printf("word: %d, %d\n", classes[0].word_count, classes[1].word_count);
  printf("docs: %d, %d\n", classes[0].document_count, classes[1].document_count);

}

void calculate_bayes(const char *review, const int isPositive) {
  char buffer[MAX_LINE_LENGTH];
  strncpy(buffer, review, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0';
  
  char *word = strtok(buffer, " ,.-!?\"\'\n/<>:;()");
  
  while (word != NULL) {
    to_lowercase(word);
    word = strtok(NULL, " ,.-!?\"\'\n/<>:;()");
  }

 
}

void read_and_calculate() {

  FILE *file = fopen("./data/test_data.jsonl", "r");
  if (file == NULL) {
    printf("Error: Unable to read test file.\n");
    return;
  }

  char *line = malloc(MAX_LINE_LENGTH);
  if (line == NULL) {
    perror("Memory allocation during read failed\n");
    fclose(file);
  }

  while (1) {
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
      printf("Error reading line or end of file reached.\n");
      break;
    }

    cJSON *json = cJSON_Parse(line);
    if (json == NULL) { 
      continue;
    }

    cJSON *review = cJSON_GetObjectItemCaseSensitive(json, "text");
    cJSON *rating = cJSON_GetObjectItemCaseSensitive(json, "rating");
    
    if (!cJSON_IsNumber(rating) || !cJSON_IsString(review)) {
      // printf("Rating unavailable at %d\n", count);
      continue;
    }

    if (rating->valueint >= 4) {
      calculate_bayes(review->valuestring, 1);
    } else {
      calculate_bayes(review->valuestring, 0);
    }
  }
} 

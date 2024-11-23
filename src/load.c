#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "load.h"
#include "utils.h"
#include "cJSON.h"
#include "preprocessing.h"

int test_reviews = 0;
int success_reviews = 0;
int zero_errors = 0;
int fails = 0;

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
      unsigned int index = hash(word);
      struct Vocabulary *new_entry = malloc(sizeof(struct Vocabulary));
      new_entry->word = strdup(word);
      new_entry->classes[1] = pos_count;
      new_entry->classes[0] = neg_count;
      new_entry->next = hashtable[index];
      hashtable[index] = new_entry;
      total_entries++;
      continue;
    }

    result = sscanf(line, "%d %d", &classes[0].word_count, &classes[1].word_count);
    if (result == 2 && classes[0].word_count == 0) {
      continue;
    } else {
      fscanf(file, "%d %d", &classes[0].document_count, &classes[1].document_count);
      continue;
    }

    printf("Error format: %s\n", line);
  } 
  fclose(file);
  // printf("Finished loading! Lines: %d\n", total_entries);
  // printf("word: %d, %d\n", classes[0].word_count, classes[1].word_count);
  // printf("docs: %d, %d\n", classes[0].document_count, classes[1].document_count);

}

static void calculate_bayes(const char *review, const int isPositive, float p_positive, float p_negative, int total_positive_words, int total_negative_words, int vocab_size) {
  char buffer[MAX_LINE_LENGTH];
  strncpy(buffer, review, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0';
  
  char *word = strtok(buffer, " ,.-!?\"\'\n/<>:;()");
  double bayes_p_positive = log(p_positive);
  double bayes_p_negative = log(p_negative);
  while (word != NULL) {
    clean_word(word);

    if (strlen(word) > 1 && strlen(word) <= MAX_WORD_LEN && !(strcmp("and", word) == 0 || strcmp("the", word) == 0 || strcmp("is", word) == 0 
      || strcmp("a", word) == 0 || strcmp("of", word) == 0 || strcmp("in", word) == 0)) {
      to_lowercase(word);
      unsigned int index = hash(word);
      struct Vocabulary *entry = hashtable[index];
      int exists = 0;
      while (entry != NULL) {
        if (strcmp(entry->word, word) == 0) {
          bayes_p_positive *= log((1 + entry->classes[1]) / ((double)vocab_size + total_positive_words));
          bayes_p_negative *= log((1 + entry->classes[0]) / ((double)vocab_size + total_negative_words));
          exists = 1;
          break;
        }
        entry = entry->next;
      }

      if (exists == 0) {
        bayes_p_positive *= log(1 / ((double)vocab_size + total_positive_words));
        bayes_p_negative *= log(1 / ((double)vocab_size + total_negative_words));
      }
    }
    word = strtok(NULL, " ,.-!?\"\'\n/<>:;()");
  }

  if ((bayes_p_positive > bayes_p_negative && isPositive == 1) || (bayes_p_negative > bayes_p_positive && isPositive == 0)) {
    success_reviews++;
  } else if (bayes_p_negative == bayes_p_positive) {
    zero_errors++;
  } else {
    fails++; 
  } 
  // printf("Positive: %.25f, Negative: %.25f, isPositive: %d\n", bayes_p_positive, bayes_p_negative, isPositive);
  test_reviews++;
}

void read_and_calculate() {
  float p_positive = (float)classes[1].document_count / (classes[1].document_count + classes[0].document_count);
  float p_negative = (float)classes[0].document_count / (classes[1].document_count + classes[0].document_count);
  int total_positive_words = classes[1].word_count;
  int total_negative_words = classes[0].word_count;
  int vocab_size = total_entries;  
  
  printf("P_Positive: %f, P_Negative: %f", p_positive, p_negative);

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
      calculate_bayes(review->valuestring, 1, p_positive, p_negative, total_positive_words, total_negative_words, vocab_size);
    } else {
      calculate_bayes(review->valuestring, 0, p_positive, p_negative, total_positive_words, total_negative_words, vocab_size);
    }

  }

  printf("Accuracy: %f\n", (float)success_reviews / test_reviews);
  printf("fails: %d, success: %d, zero_errors: %d", fails, success_reviews, zero_errors);
  free(line);
  fclose(file);
} 

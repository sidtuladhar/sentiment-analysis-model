#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cJSON.h"
#include "utils.h"

#define MAX_LINE_LENGTH 10000

struct Vocabulary *hashtable[HASH_SIZE];
struct ClassInfo classes[MAX_CLASSES];
int total_entries = 0;
int existing = 0;

void check_load_factor() {
    float load_factor = (float)total_entries / HASH_SIZE;
    printf("Load Factor: %.2f\n", load_factor);

    if (load_factor > 0.7) {
        printf("Warning: Load factor exceeds 0.7. Consider resizing.\n");
    }
}

void init_classes() {
  classes[0].class_name = "Negative";
  classes[0].word_count = 0;
  classes[0].document_count = 0;

  classes[1].class_name = "Positive";
  classes[1].word_count = 0;
  classes[1].document_count = 0;
}

void print_bucket_distribution() {
    for (int i = 0; i < HASH_SIZE; i++) {
        int count = 0;
        struct Vocabulary *entry = hashtable[i];
        while (entry) {
            count++;
            entry = entry->next;
        }
        printf("Bucket %d: %d entries\n", i, count);
    }
}

void word_to_vocab(const char *word, int isPositive) {
  if (strcmp("and", word) == 0 || strcmp("the", word) == 0 || strcmp("is", word) == 0 
    || strcmp("a", word) == 0 || strcmp("of", word) == 0 || strcmp("in", word) == 0) {
    return;
  }
  unsigned int index = hash(word);
  struct Vocabulary *entry = hashtable[index];

  while (entry != NULL) {
    if (strcmp(entry->word, word) == 0) {
      entry->classes[isPositive]++;
      existing++;
      return;
    }
    entry = entry->next;
  }

  struct Vocabulary *new_entry = malloc(sizeof(struct Vocabulary));
  new_entry->word = strdup(word);
  new_entry->classes[0] = 0;
  new_entry->classes[1] = 0;
  new_entry->classes[isPositive] = 1;
  new_entry->next = hashtable[index];
  hashtable[index] = new_entry;
  total_entries++;
}

void parse_word(const char *review, const int isPositive) {
  char buffer[MAX_LINE_LENGTH];
  strncpy(buffer, review, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0';
  
  char *word = strtok(buffer, " ,.-!?\"\'\n");
  
  while (word != NULL) {
    to_lowercase(word);
    classes[isPositive].word_count++;
    word_to_vocab(word, isPositive); 
    word = strtok(NULL, " ,.-!?\"\'\n");
  }
}

int main() {
    init_classes();

    FILE *fp = fopen("./data/raw/videogame.jsonl", "r"); 
    if (fp == NULL) { 
            printf("Error: Unable to open the file.\n"); 
            return 1; 
        }

    char *line = malloc(MAX_LINE_LENGTH);
    if (line == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
    }

    int count = 0;

    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, fp) == NULL) {
            printf("Error reading line or end of file reached: %d", count);
            break;
        }
        count++; 
        cJSON *json = cJSON_Parse(line);
        if (json == NULL) { 
          continue;
        }

        cJSON *review = cJSON_GetObjectItemCaseSensitive(json, "text");
        // cJSON *title = cJSON_GetObjectItemCaseSensitive(json, "title");
        cJSON *rating = cJSON_GetObjectItemCaseSensitive(json, "rating");
       

        if (!cJSON_IsNumber(rating) || !cJSON_IsString(review)) {
          // printf("Rating unavailable at %d\n", count);
          continue;
        }

        if (rating->valueint >= 4) {
          classes[0].document_count++;
          parse_word(review->valuestring, 1);
        } else {
          classes[1].document_count++;
          parse_word(review->valuestring, 0);
        }
        cJSON_Delete(json);
    }
    fclose(fp);    
    printf("pos: %d\n", classes[0].document_count);
    free(line);

return 0;
}

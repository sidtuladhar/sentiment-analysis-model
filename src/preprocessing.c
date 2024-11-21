#include <stdio.h> 
#include <stdlib.h>
#include "cJSON.h"
#include "utils.h"

#define MAX_LINE_LENGTH 10000

struct Vocabulary vocab[MAX_WORDS];
struct ClassInfo classes[MAX_CLASSES];

void init_classes() {
  classes[0].class_name = "Positive";
  classes[0].word_count = 0;
  classes[0].document_count = 0;

  classes[1].class_name = "Negative";
  classes[1].word_count = 0;
  classes[1].document_count = 0;
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
        if (json == NULL) { // review too long
            // const char *error_ptr = cJSON_GetErrorPtr();
            // if (error_ptr != NULL) {
            //     fprintf(stderr, "JSON Parse Error before: %s\n", error_ptr);
            // }
            // Handle parse error (e.g., skip to next line)
            continue;
        }

        cJSON *review = cJSON_GetObjectItemCaseSensitive(json, "text");
        cJSON *title = cJSON_GetObjectItemCaseSensitive(json, "title");
        cJSON *rating = cJSON_GetObjectItemCaseSensitive(json, "rating");
       

        if (!cJSON_IsNumber(rating)) {
          // printf("Rating unavailable at %d\n", count);
          continue;
        }

        if (rating->valueint >= 4) {
          classes[0].document_count++;
        } else if (rating->valueint <= 2) {
          classes[1].document_count++;
        }
        
        cJSON_Delete(json);
    }
    fclose(fp);    
    printf("pos: %d\n", classes[0].document_count);
    free(line);
return 0;
}

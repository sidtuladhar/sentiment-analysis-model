#include <stdio.h> 
#include <stdlib.h>
#include "cJSON.h"

#define MAX_LINE_LENGTH 10000


int main() {

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

    for (int i = 0; i <= 10; i++) {

        if (fgets(line, MAX_LINE_LENGTH, fp) == NULL) {
            fprintf(stderr, "Error reading line %d or end of file reached.\n", i);
            break;
        }

        // printf("parsing line %d: %s", i, line);

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

        if (title->valuestring != NULL) {
            printf("title: %s\n", title->valuestring);
        }
        if (rating->valueint != 0) {
            
            printf("rating: %d\n", rating->valueint);
        }
        cJSON_Delete(json);
    }
    fclose(fp);    

return 0;
}
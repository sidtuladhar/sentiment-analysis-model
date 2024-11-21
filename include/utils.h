#ifndef UTIL_H
#define UTIL_H

#define MAX_CLASSES 2
#define MAX_WORDS 5000


struct Vocabulary {
  char *word;
  int classes[MAX_CLASSES];
};

struct ClassInfo {
  char *class_name;
  int word_count;
  int document_count;
};


#endif

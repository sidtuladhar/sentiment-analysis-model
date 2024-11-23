#include <stdio.h>
#include "preprocessing.h"
#include "load.h"

int reprocess = 1;

int main() {
  
  if (reprocess == 1) {
  preprocess();
  }

  load();

  return 0;
}

#include <stdio.h>
#include "preprocessing.h"
#include "load.h"

int reprocess = 0;

int main() {
  
  if (reprocess == 1) {
  preprocess();
  }

  load();

  return 0;
}

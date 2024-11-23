# Sentiment Analysis with Naive Bayes in C

## Overview
This project implements a Naive Bayes classifier in C to perform sentiment analysis on Amazon reviews. It processes textual data, constructs a vocabulary using a hashmap, and applies probabilistic calculations to classify reviews as positive or negative.

## Features
- Efficient text preprocessing with tokenization and stop-word filtering.
- Hashmap-based vocabulary storage for fast lookups.
- Probabilistic sentiment classification using Naive Bayes.
- Logging support for debugging and analyzing results.
- **49.95%** accuracy on the test dataset.

## Requirements
- C Compiler (e.g., GCC)
- Libraries:
  - cJSON for JSON parsing
  - Standard C libraries for string manipulation and file handling

## How It Works
1. **Data Preprocessing**: Reviews are tokenized, cleaned, and stored in a hashmap.
2. **Model Training**: The vocabulary is populated with word frequencies for both positive and negative classes.
3. **Classification**: The model calculates probabilities using Naive Bayes and logs classification results.
4. **Performance Metrics**: Reports accuracy, failures, and zero-error cases for evaluation.

   ```bash
## Dataset Information
The dataset is **not included** in the repository. Instead, you can access the dataset [here](https://amazon-reviews-2023.github.io/main.html).

```bibtex
@article{hou2024bridging,
  title={Bridging Language and Items for Retrieval and Recommendation},
  author={Hou, Yupeng and Li, Jiacheng and He, Zhankui and Yan, An and Chen, Xiusi and McAuley, Julian},
  journal={arXiv preprint arXiv:2403.03952},
  year={2024}
}
```

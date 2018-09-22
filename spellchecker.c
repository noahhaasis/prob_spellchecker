/*
 * TODO:
 * Check if the file operations succeeded
 * DRY out next_word_in_dict and next_word_in_text
 */
#include <stdio.h>
#include <stdlib.h>

#include "bloom_filter.h"

#define INITIAL_WORD_BUFFER_LEN 6
#define ESTIMATED_DICT_SIZE 150000
#define FALSE_POSITIVE_PROBABILITY 0.01


bloom_filter_t *get_filter_from_dict(FILE *dict);

/*
 * Return the next word from the dict stream
 * It's assumed that words are seperated using new lines
 */
char *next_word_in_dict(FILE *dict);

/*
 * Return the next word from the text stream.
 * A word is a sequence of characters and apostrophes.
 * Everything else gets ignored.
 */
char *next_word_in_text(FILE *text);

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./spellchecker dictionary text\n");
        return 1;
    }

    FILE *dictionary;
    dictionary = fopen(argv[1], "r");

    bloom_filter_t *filter;
    filter = get_filter_from_dict(dictionary);
    fclose(dictionary);

    FILE *text;
    text = fopen(argv[2], "r");

    int misspelled_words = 0;
    char *word;
    while ((word = next_word_in_text(text))) {
        if (!bloom_filter_might_contain(filter, word)) {
            misspelled_words++;
            printf("%s\n", word);
        }

        free(word);
    }
    bloom_filter_destroy(filter);
    fclose(text);

    printf("Misspelled words: %d\n", misspelled_words);

    return EXIT_SUCCESS;
}

char *next_word_in_text(FILE *text) {
    char *word_buffer = malloc(INITIAL_WORD_BUFFER_LEN);
    int word_len = 0, buffer_size = INITIAL_WORD_BUFFER_LEN;
    char c;

    while ((c = fgetc(text)) != EOF) {
        // Skip whitespace


        // TODO: Decide on a word policy and implement it

        if (buffer_size == word_len) {
            buffer_size *= 2;
            word_buffer = realloc(word_buffer, buffer_size);
        }
    }

    if (word_len == 0 && c == EOF) {
        free(word_buffer);
        return NULL;
    }

    word_buffer[word_len] = '\0';

    return word_buffer;
}

char *next_word_in_dict(FILE *dict) {
    char *word_buffer = malloc(INITIAL_WORD_BUFFER_LEN);
    int word_len = 0, buffer_size = INITIAL_WORD_BUFFER_LEN;
    char c;
    while ((c = fgetc(dict)) != '\n' && c != EOF) {
        word_buffer[word_len++] = c;

        if (word_len == buffer_size) {
            buffer_size *= 2;
            word_buffer = realloc(word_buffer, buffer_size);
        }
    }

    if (word_len == 0 && c == EOF) {
        free(word_buffer);
        return NULL;
    }

    word_buffer[word_len] = '\0';
    return word_buffer;
}

bloom_filter_t *get_filter_from_dict(FILE *dict) {
    bloom_filter_t *filter = bloom_filter_create(ESTIMATED_DICT_SIZE, FALSE_POSITIVE_PROBABILITY);
    char *word;

    while ((word = next_word_in_dict(dict))) {
        bloom_filter_add(filter, word);
        free(word);
    }

    return filter;
}

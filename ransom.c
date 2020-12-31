#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 6

struct list_node {
    struct list_node *next;
    char word[MAX_WORD_LEN];
    int word_count;
};

struct word_list {
    struct list_node *head;
};

char* readline();
char** split_string(char*);

/** create_map
 * @brief Create a data structure (linked list) of the words and the number of 
            times each word is shown in the magazine/note. 
 * @param *list Pointer to a linked list with the 2 fields we mentioned above.
 * @param **words Array of strings which represent the words.
 * @param count Integer which represents the size of the array of strings we
                mentioned above.
 * @return -
 * @note -
 */
int create_map(struct word_list *list, char **words, int count);


/** append_list_node
 * @brief Create a linked list and adding elements to it, where each element
            contains 2 fields - the word, the number of times the word is shown
            in the magazine/note.
 * @param **head Pointer to pointer to a struct with the 2 fields we have
                    mentioned above.
 * @param *word_data String which represents the word we add to the linked list.
 * @return 1 if malloc fails, otherwise 0
 * @note -
 */
int append_list_node(struct list_node **head, char *word_data);


/** free_list
 * @brief Free linked list we create during the run from the memory.
 * @param *list Pointer to a linked list with the 2 fields we mentioned above.
 * @return 1 if malloc fails, otherwise 0
 * @note -
 */
void free_list(struct word_list *list);


/** checkMagazine
 * @brief Main function of the program, uses other functions to decide whether 
            there are enough words in the magazine in order to create the 
            desired note.
 * @param magazine_count Integer which represents the number of words which are
                            shown in the magazine.
 * @param **magazine Array of strings which represent the words in the magazine.
 * @param note_count Integer which represents the number of words which are
                            shown in the note.
 * @param **magazine Array of strings which represent the words in the note.
 * @return -
 * @note -
 */
// Complete the checkMagazine function below.
void checkMagazine(int magazine_count, char** magazine, int note_count, char** note) {
    int check_result = 1;
    int malloc_fail = 0; /* 1 if magazine contains all note words, else 0 */
    struct list_node *cur_note_node, *cur_magazine_node;
    
    /* Initialize lists */
    struct word_list magazine_list, note_list;
    magazine_list.head = NULL;
    note_list.head = NULL;

    /* Check for special cases */
    if ((magazine_count < note_count)) {
        printf("No");
        return;
    }
    if ((note_count == 0)) {
        printf("Yes");
        return;
    }

    /* Create magazine and note linked lists */
    malloc_fail = create_map(&magazine_list, magazine, magazine_count);
    if (malloc_fail) {
        free_list(&magazine_list);
        exit(1);
    }
    malloc_fail = create_map(&note_list, note, note_count);
    if (malloc_fail) {
        free_list(&magazine_list);
        free_list(&note_list);
        exit(1);
    }

    /* Check if magazine has all words in note including repetitions */
    cur_note_node = note_list.head;

    while (cur_note_node) {

        cur_magazine_node = magazine_list.head;

        while (cur_magazine_node) {

            if (!strcmp(cur_note_node->word, cur_magazine_node->word)) {

                if (cur_magazine_node->word_count >= 
                    cur_note_node->word_count) {

                    break;

                } else {

                    check_result = 0;
                    break;
                }

            } else if (cur_magazine_node->next == NULL) {
                check_result = 0;
                break;
            }

            cur_magazine_node = cur_magazine_node->next;
        }

        if (check_result == 0) {
            break;
        }

        cur_note_node = cur_note_node->next;
    }

    if (check_result == 0) {
        printf("No");
    } else {
        printf("Yes");
    }

    /* Free all memory allocated in create_map for magazine and note */
    free_list(&magazine_list);
    free_list(&note_list);

    return;    
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int create_map(struct word_list *list, char **words, int count) {
    struct list_node **list_head = &(list->head);
    int malloc_fail = 0;
    for(int i=0; i<count; i++) {
        malloc_fail = append_list_node(list_head, words[i]);
        if (malloc_fail) return 1;
    }
    return 0;
}

int append_list_node(struct list_node **head, char *word_data) { 
    
    struct list_node *cur_node = *head;
    struct list_node *prev_node;

    /* Create and insert data to new node */
    struct list_node *new_node = (struct list_node*) 
                                              malloc(sizeof(struct list_node));
    /* Check malloc success */
    if (new_node == NULL) {
        return 1;
    }
    strcpy(new_node->word, word_data);
    new_node->word_count = 1;

    /* This new node is going to be the last node, so set next to NULL */
    new_node->next = NULL; 
  
    /* If the Linked List is empty, make the new node as head */
    if (*head == NULL) 
    { 
       *head = new_node; 
       return 0;
    } 
  
    /* Else traverse the list. If word exists, update the relevant count field
    and delete new node, else add node to end of list */
    while (cur_node) {
        if (!strcmp(cur_node->word, new_node->word)) {
            cur_node->word_count += 1;
            free(new_node);
            return 0;
        } else {
            prev_node = cur_node;
            cur_node = cur_node->next;
        }
    }

    prev_node->next = new_node; 
    return 0; 
} 

void free_list(struct word_list *list) {

    struct list_node *cur_node = list->head;
    struct list_node *temp;

    while (cur_node) {
        temp = cur_node;
        cur_node = cur_node->next;

        free(temp);
    }
}

void print_list_content(struct word_list *list, char list_name[5]) {
    struct list_node *cur_node = list->head;
    printf("%s\n", list_name);
    while (cur_node) {
        printf("%s, %d\n", cur_node->word, cur_node->word_count);
        cur_node = cur_node->next;
    }
    return;
}

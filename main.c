#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max_line_len = 100;
int max_lines = 100;
char **text;
int line_count = 0;


void show_rules() {
    printf("Choose the command\n"
           "1 Write a new line\n"
           "2 Start a new line\n"
           "3 Save file\n"
           "4 Load file name\n"
           "5 Show actual text\n"
           "6 Insert the text\n"
           "7 Find word\n"
           "8 Clear console\n"
           "9 Exit\n");
}

char* get_input() {
    int buffer_size = 25;
    char *input;
    char symbol;
    int index = 0;
    input = (char*) malloc(buffer_size * sizeof(char));

    while ((symbol = getchar()) != '\n') {
        if (index >= buffer_size - 1) {
            buffer_size *= 2;
            input = (char*) realloc(input, buffer_size * sizeof(char));
        }
        input[index] = symbol;
        index++;
    }
    input[index] = '\0';
    return input;
}


void free_memory() {
    for (int i = 0; i < line_count; i++) {
        free(text[i]);
    }
    free(text);
}


void write_line() {
    printf("Enter the text:\n");
    char *input = get_input();
    text[line_count] = input;
    line_count++;
}


void start_new_line() {
    if (line_count < max_lines) {
        text[line_count] = strdup("\0");
        line_count++;
    }
    else{
        max_lines *= 2;
        text = realloc(text, max_lines);
    }
}


void save_to_file() {
    printf("Enter the file name for saving: ");
    char *file_name = get_input();
    char *file_name_txt = malloc(strlen(file_name) + 5 * sizeof(char));
    strcpy(file_name_txt, file_name);
    strcat(file_name_txt, ".txt");;
    FILE *file = fopen(file_name, "w");
    for (int i = 0; i < line_count; i++) {
        fprintf(file, "%s\n", text[i]);
    }
    fclose(file);
    free(file_name);
    free(file_name_txt);
}


void load_from_file() {
    printf("Enter the file name for loading: ");
    char *file_name = get_input();
    char *file_name_txt = malloc(strlen(file_name) + 5 * sizeof(char));
    strcpy(file_name_txt, file_name);
    strcat(file_name_txt, ".txt");;
    FILE *file = fopen(file_name_txt, "r");
    free_memory();
    text = (char **)malloc(max_lines * sizeof(char *));
    line_count = 0;
    char buffer[max_line_len];
    while (fgets(buffer, max_line_len, file) != NULL) {
        if (line_count >= max_lines) {
            max_lines += 2;
            text = realloc(text, sizeof(char*) * max_lines);
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        text[line_count] = strdup(buffer);
        line_count++;

    }
    fclose(file);
    printf("Text has been loaded successfully.\n");
    free(file_name);
    free(file_name_txt);
}


void print_text() {
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", text[i]);
    }
}


void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}


void insert_text() {
    int line;
    int idx;
    printf("Choose line and index: ");
    scanf("%d %d", &line, &idx);
    clear_input_buffer();
    if (line < 0 || line >= line_count || idx < 0 || idx > strlen(text[line])) {
        printf("Invalid line or index.\n");
        return;
    }
    printf("Enter text to insert: ");
    char *substr = get_input();
    size_t new_length = strlen(text[line]) + strlen(substr) + 1;
    char *buffer = (char *)malloc(new_length * sizeof(char));
    strncpy(buffer, text[line], idx);
    buffer[idx] = '\0';
    strcat(buffer, substr);
    strcat(buffer, text[line] + idx);
    free(text[line]);
    text[line] = strdup(buffer);
    free(substr);
}


void search_text() {
    printf("Enter text to search: ");
    char *sub_str = get_input();
    for (int i = 0; i < line_count; i++) {
        char *idx = text[i];
        while ((idx = strstr(idx, sub_str)) != NULL) {
            printf("%s index: %d %ld\n", sub_str, i, idx - text[i]);
            idx++;
        }
    }
    free(sub_str);
}


int main() {
    text = (char **)malloc(max_lines * sizeof(char *));
    show_rules();

    while (1) {
        printf("> ");
        char command = getchar();
        clear_input_buffer();

        switch (command) {
            case '1':
                write_line();
                break;
            case '2':
                start_new_line();
                break;
            case '3':
                save_to_file();
                break;
            case '4':
                load_from_file();
                break;
            case '5':
                print_text();
                break;
            case '6':
                insert_text();
                break;
            case '7':
                search_text();
                break;
            case '8':
                printf("Coming soon");
                break;
            case '9':
                free_memory();
                exit(0);
            default:
                printf("Invalid command\n");
                break;
        }

        show_rules();
    }

    return 0;
}
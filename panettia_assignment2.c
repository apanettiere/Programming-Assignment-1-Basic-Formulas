#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct movie {
    char title[256];
    int year;
    char languages[5][21];
    double rating;
    struct movie* next;
} movie;

movie* create_movie(char* title, int year, char languages[][21], double rating) {
    movie* new_movie = (movie*)malloc(sizeof(movie));
    if (new_movie == NULL) {
        fprintf(stderr, "Failed to allocate memory for new movie.\n");
        return NULL;
    }
    strcpy(new_movie->title, title);
    new_movie->year = year;
    for (int i = 0; i < 5; i++) {
        strcpy(new_movie->languages[i], languages[i]);
    }
    new_movie->rating = rating;
    new_movie->next = NULL;
    return new_movie;
}

void insert_movie(movie** head, movie* new_movie) {
    if (*head == NULL) {
        *head = new_movie;
    } else {
        movie* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_movie;
    }
}

movie* parse_line(char* line) {
    char* token;
    char title[256];
    int year;
    char languages[5][21] = {""};
    double rating;
    int lang_index = 0;

    token = strtok(line, ",");
    strcpy(title, token);

    token = strtok(NULL, ",");
    year = atoi(token);

    token = strtok(NULL, ",");
    token = strtok(token, "[]");
    while (token != NULL && lang_index < 5) {
        if (strcmp(token, ";") != 0) {
            strcpy(languages[lang_index++], token);
        }
        token = strtok(NULL, ";]");
    }

    token = strtok(NULL, ",");
    rating = strtod(token, NULL);

    return create_movie(title, year, languages, rating);
}

movie* load_movies(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return NULL;
    }
    movie* head = NULL;
    char buffer[1024];
    int count = 0;

    fgets(buffer, 1024, file); // Skip the header line
    while (fgets(buffer, 1024, file)) {
        movie* new_movie = parse_line(buffer);
        insert_movie(&head, new_movie);
        count++;
    }
    fclose(file);
    printf("Processed file %s and parsed data for %d movies\n", filename, count);
    return head;
}

void show_movies_by_year(movie* head, int year) {
    int found = 0;
    while (head != NULL) {
        if (head->year == year) {
            printf("%s\n", head->title);
            found = 1;
        }
        head = head->next;
    }
    if (!found) {
        printf("No data about movies released in the year %d\n", year);
    }
}

void show_highest_rated_movies(movie* head) {
    struct {
        int year;
        double rating;
        char title[256];
    } best_movies[122]; // Assuming range from 1900 to 2021
    int i;
    for (i = 0; i < 122; i++) {
        best_movies[i].year = 1900 + i;
        best_movies[i].rating = 0.0;
        strcpy(best_movies[i].title, "");
    }

    while (head != NULL) {
        int index = head->year - 1900;
        if (head->rating > best_movies[index].rating) {
            best_movies[index].rating = head->rating;
            strcpy(best_movies[index].title, head->title);
        }
        head = head->next;
    }

    for (i = 0; i < 122; i++) {
        if (best_movies[i].rating != 0) {
            printf("%d %.1f %s\n", best_movies[i].year, best_movies[i].rating, best_movies[i].title);
        }
    }
}

void show_movies_by_language(movie* head, char* language) {
    int found = 0;
    while (head != NULL) {
        for (int i = 0; i < 5; i++) {
            if (strcmp(head->languages[i], language) == 0) {
                printf("%d %s\n", head->year, head->title);
                found = 1;
                break;
            }
        }
        head = head->next;
    }
    if (!found) {
        printf("No data about movies released in %s\n", language);
    }
}

void free_movies(movie* head) {
    while (head != NULL) {
        movie* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    movie* head = load_movies(argv[1]);
    if (head == NULL) {
        return EXIT_FAILURE;
    }

    int choice, year;
    char language[21];

    do {
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");
        printf("Enter a choice from 1 to 4: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the year for which you want to see movies: ");
                scanf("%d", &year);
                show_movies_by_year(head, year);
                break;
            case 2:
                show_highest_rated_movies(head);
                break;
            case 3:
                printf("Enter the language for which you want to see movies: ");
                scanf("%20s", language); // Limit input to 20 characters
                show_movies_by_language(head, language);
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("You entered an incorrect choice. Try again.\n");
                break;
        }
    } while (choice != 4);

    free_movies(head);
    return EXIT_SUCCESS;
}

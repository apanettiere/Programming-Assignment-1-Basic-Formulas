#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct movie {
    char *title;
    int year;
    char *languages;
    double rating;
    struct movie *next;
} Movie;

Movie *createMovie(char *line);
Movie *processFile(char *filename);
void showMoviesByYear(Movie *head);
void showHighestRatedMovies(Movie *head);
void showMoviesByLanguage(Movie *head);
void freeMovies(Movie *head);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./movies <filename>\n");
        return EXIT_FAILURE;
    }

    Movie *movies = processFile(argv[1]);
    int choice;

    do {
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");
        printf("Enter a choice from 1 to 4: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showMoviesByYear(movies);
                break;
            case 2:
                showHighestRatedMovies(movies);
                break;
            case 3:
                showMoviesByLanguage(movies);
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("You entered an incorrect choice. Try again.\n");
        }
    } while (choice != 4);

    freeMovies(movies);
    return EXIT_SUCCESS;
}

Movie *createMovie(char *line) {
    Movie *m = malloc(sizeof(Movie));
    char *saveptr;

    m->title = strdup(strtok_r(line, ",", &saveptr));
    m->year = atoi(strtok_r(NULL, ",", &saveptr));
    m->languages = strdup(strtok_r(NULL, ",", &saveptr));
    m->rating = atof(strtok_r(NULL, "\n", &saveptr));
    m->next = NULL;

    return m;
}

Movie *processFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);  // Skip header

    Movie *head = NULL, *tail = NULL;
    while (fgets(buffer, sizeof(buffer), file)) {
        Movie *newMovie = createMovie(buffer);
        if (!tail) {
            head = tail = newMovie;
        } else {
            tail->next = newMovie;
            tail = newMovie;
        }
    }
    fclose(file);

    int count = 0;
    Movie *temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    printf("Processed file %s and parsed data for %d movies\n", filename, count);
    return head;
}

void showMoviesByYear(Movie *head) {
    int year;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    int found = 0;
    while (head) {
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

void showHighestRatedMovies(Movie *head) {
    double highestRatings[122] = {0};  // Array for years 1900-2021
    Movie *highestMovies[122] = {NULL};

    while (head) {
        int index = head->year - 1900;
        if (head->rating > highestRatings[index]) {
            highestRatings[index] = head->rating;
            highestMovies[index] = head;
        }
        head = head->next;
    }

    for (int i = 0; i < 122; i++) {
        if (highestMovies[i]) {
            printf("%d %.1f %s\n", highestMovies[i]->year, highestMovies[i]->rating, highestMovies[i]->title);
        }
    }
}

void showMoviesByLanguage(Movie *head) {
    char language[21];
    printf("Enter the language for which you want to see movies: ");
    scanf("%20s", language);  // Avoid buffer overflow

    int found = 0;
    while (head) {
        if (strstr(head->languages, language)) {  // Simple substring match
            printf("%d %s\n", head->year, head->title);
            found = 1;
        }
        head = head->next;
    }
    if (!found) {
        printf("No data about movies released in %s\n", language);
    }
}

void freeMovies(Movie *head) {
    while (head) {
        Movie *temp = head;
        head = head->next;
        free(temp->title);
        free(temp->languages);
        free(temp);
    }
}



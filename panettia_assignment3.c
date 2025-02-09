#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define MAX_FILENAME_LEN 256
#define MAX_LINE_LEN 1024

// Function prototypes
void pickLargestFile();
void pickSmallestFile();
void pickFileByName();
void processFile(const char* filename);
void createDirectoryAndProcessMovies(const char* filename);

int main() {
    int choice;

    do {
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int fileChoice;
                do {
                    printf("\nWhich file you want to process?\n");
                    printf("Enter 1 to pick the largest file\n");
                    printf("Enter 2 to pick the smallest file\n");
                    printf("Enter 3 to specify the name of a file\n\n");
                    printf("Enter a choice from 1 to 3: ");
                    scanf("%d", &fileChoice);

                    switch (fileChoice) {
                        case 1:
                            pickLargestFile();
                            break;
                        case 2:
                            pickSmallestFile();
                            break;
                        case 3:
                            pickFileByName();
                            break;
                        default:
                            printf("You entered an incorrect choice. Try again.\n");
                    }
                } while (fileChoice < 1 || fileChoice > 3);
                break;
            }

            case 2:
                printf("Exiting the program.\n");
                break;

            default:
                printf("You entered an incorrect choice. Try again.\n");
        }
    } while (choice != 2);

    return 0;
}

// Function to find and process the largest file
void pickLargestFile() {
    DIR *dir;
    struct dirent *ent;
    long largestSize = -1;
    char largestFile[MAX_FILENAME_LEN] = {0};

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, "movies_") && strstr(ent->d_name, ".csv")) {
                struct stat st;
                stat(ent->d_name, &st);
                if (st.st_size > largestSize) {
                    largestSize = st.st_size;
                    strncpy(largestFile, ent->d_name, MAX_FILENAME_LEN);
                }
            }
        }
        closedir(dir);
        if (largestSize > -1) {
            printf("Now processing the chosen file named %s\n", largestFile);
            processFile(largestFile);
        } else {
            printf("No valid files found.\n");
        }
    } else {
        perror("Could not open directory");
    }
}

// Function to find and process the smallest file
void pickSmallestFile() {
    DIR *dir;
    struct dirent *ent;
    long smallestSize = LONG_MAX;
    char smallestFile[MAX_FILENAME_LEN] = {0};

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, "movies_") && strstr(ent->d_name, ".csv")) {
                struct stat st;
                stat(ent->d_name, &st);
                if (st.st_size < smallestSize) {
                    smallestSize = st.st_size;
                    strncpy(smallestFile, ent->d_name, MAX_FILENAME_LEN);
                }
            }
        }
        closedir(dir);
        if (smallestSize != LONG_MAX) {
            printf("Now processing the chosen file named %s\n", smallestFile);
            processFile(smallestFile);
        } else {
            printf("No valid files found.\n");
        }
    } else {
        perror("Could not open directory");
    }
}

// Function to pick and process a specific file by name
void pickFileByName() {
    char filename[MAX_FILENAME_LEN];

    while (1) {
        printf("Enter the complete file name: ");
        scanf("%s", filename);

        if (access(filename, F_OK) != -1) {
            printf("Now processing the chosen file named %s\n", filename);
            processFile(filename);
            break;
        } else {
            printf("The file %s was not found. Try again\n\n", filename);
        }
    }
}

// Function to process the selected file
void processFile(const char* filename) {
    printf("Processing file: %s\n", filename);
    createDirectoryAndProcessMovies(filename);
}

// Function to create a directory and process the movies
void createDirectoryAndProcessMovies(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }

    char line[MAX_LINE_LEN];
    int year;
    char title[256];
    char directoryName[128];
    srand(time(NULL));

    int random_number;
    do {
        random_number = rand() % 100000;
        snprintf(directoryName, sizeof(directoryName), "your_onid.movies.%d", random_number);
    } while (access(directoryName, F_OK) == 0);

    mkdir(directoryName, 0750);
    printf("Created directory with name %s\n\n", directoryName);

    fgets(line, sizeof(line), file); // Skip header line

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%255[^,],%d", title, &year);

        char filePath[150];
        snprintf(filePath, sizeof(filePath), "%s/%d.txt", directoryName, year);

        FILE *yearFile = fopen(filePath, "a");
        if (yearFile) {
            fprintf(yearFile, "%s\n", title);
            fclose(yearFile);
            chmod(filePath, 0640); // Ensures correct file permissions
        } else {
            perror("Error writing to year file");
        }
    }

    fclose(file);
}



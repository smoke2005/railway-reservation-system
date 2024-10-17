#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "daytrain_wl.h"

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 10


void checkwl(int train_no, int no_of_seats) {
    printf("Entering into waiting list...");

    handle_WaitingList_Update(train_no, no_of_seats);
}

void day_update_csv(int train_no, char *class_type, int seats_to_deduct) {
    char filename[] = "C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\day_v1.csv";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file\n");
        fclose(file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];
    int found = 0;
    char title[MAX_LINE_LENGTH];

    // Copy the header line
    if (fgets(title, MAX_LINE_LENGTH, file) != NULL) {
        fprintf(temp_file, "%s", title);
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Make a copy of the line
        char rline[MAX_LINE_LENGTH];
        strcpy(rline, line);

        //printf("%s", rline); // Print the line to verify

        // Split the line into fields using strtok
        char *token = strtok(line, ",");
        int i = 0; // Initialize the index
        while (token != NULL && i < MAX_FIELDS) {
            fields[i++] = token; // Store the token in the fields array
            token = strtok(NULL, ",");
        }

        if (atoi(fields[0]) == train_no) {
            found = 1;

            if (strcmp(class_type, "AC") == 0) {
                int ac_seats = atoi(fields[1]);
                if (ac_seats >= seats_to_deduct) {
                    ac_seats -= seats_to_deduct;
                    sprintf(fields[1], "%d", ac_seats);
                } else {
                    printf("Not enough AC seats available\n");
                    checkwl(train_no, seats_to_deduct);
                    sprintf(fields[3], "%d",(atoi(fields[3]))-seats_to_deduct);
                }
            } else if (strcmp(class_type, "EX") == 0) {
                int ec_seats = atoi(fields[2]);
                if (ec_seats == 0 && fields[2][0] != '0') {
                    printf("Error: Invalid EXEC seat count\n");
                    return; // Exit or handle error as needed
                }
                if (ec_seats >= seats_to_deduct) {
                    ec_seats -= seats_to_deduct;
                    sprintf(fields[2], "%d", ec_seats);
                } else {
                    printf("Not enough EXEC seats available\n");
                    checkwl(train_no, seats_to_deduct);
                     sprintf(fields[3], "%d",(atoi(fields[3]))-seats_to_deduct);
                }
            }

            fprintf(temp_file, "%s,%s,%s,%s\n", fields[0], fields[1], fields[2], fields[3]);
        } else {
            // Write the original line to the temporary file
            fprintf(temp_file, "%s", rline);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    remove(filename);
    rename("temp.csv", filename);
}


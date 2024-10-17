#ifndef BOOKING_V2_H_INCLUDED
#define BOOKING_V2_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAIN_NAME_LENGTH 50
#define MAX_ORIGIN_LENGTH 50
#define MAX_DESTINATION_LENGTH 50
#define MAX_DATE_LENGTH 15
#define MAX_TRAIN_DATA_LENGTH 1024
#define INITIAL_CAPACITY 10

typedef struct {
    int train_id;
    char train_name[MAX_TRAIN_NAME_LENGTH];
    char origin[MAX_ORIGIN_LENGTH];
    char destination[MAX_DESTINATION_LENGTH];
    char type[7];
    char oricode[5];
    char destcode[5];
    char duration[30];
    int index;
    char date[MAX_DATE_LENGTH];
} Train;

// Function to read a single line from a CSV file
char* readLine(FILE* file) {
    char row[1000];
    char line[MAX_TRAIN_DATA_LENGTH];
    fgets(row,1000,file);
    if (fgets(line, sizeof(line), file) == NULL) {
        return NULL;
    }
    return strdup(line); // Allocate memory for the line
}

int parseTrainData(char* line, Train* train) {
    int a=sscanf(line, "%d,%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%[^\n]",
          &train->train_id, train->train_name, &train->type, train->date,
          train->oricode, train->origin,
          train->destcode, train->destination,
          train->duration, &train->index);
    return a;
}

// Function to get user input for origin, destination, and date
void getUserInput(char* origin, char* destination, char* date) {
    printf("\nEnter origin station: ");
    fgets(origin, MAX_ORIGIN_LENGTH, stdin);
    origin[strcspn(origin, "\n")] = 0; // Remove newline character

    printf("Enter destination station: ");
    fgets(destination, MAX_DESTINATION_LENGTH, stdin);
    destination[strcspn(destination, "\n")] = 0; // Remove newline character

    printf("Enter date (DD-MM-YYYY): ");
    fgets(date, MAX_DATE_LENGTH, stdin);
    date[strcspn(date, "\n")] = 0; // Remove newline character
}

// Function to read train data from a CSV file and filter by user input
Train* readFilteredTrainData(Train* trains, int num_trains, const char* origin, const char* destination, const char* date) {
    int num_filtered_trains = 0;

    for (int i = 0; i < num_trains; i++) {
        if (strcmp(trains[i].origin, origin) == 0 &&
            strcmp(trains[i].destination, destination) == 0 &&
            strcmp(trains[i].date, date) == 0) {
            // If the record matches the user input, copy it to the beginning of the array
            if (i != num_filtered_trains) {
                trains[num_filtered_trains] = trains[i];
            }
            num_filtered_trains++;
        } else {
            // If the record doesn't match, delete it from the array
            if (i < num_filtered_trains) {
                trains[i] = trains[num_filtered_trains];
            }
        }
    }

    // Resize the array to remove the deleted records
    trains = realloc(trains, sizeof(Train) * num_filtered_trains);
    return trains;
}

void printTrainData(Train *filtrain) {
    printf("+--------------+-------------------+---------------+\n");
    printf(BRIGHT_YELLOW"| Train Number |    Train Name     |  Travel Date  |\n"RESET);
    printf("+--------------+-------------------+---------------+\n");
    printf(BRIGHT_YELLOW"| %12d | %17s | %13s |\n"RESET, filtrain->train_id, filtrain->train_name, filtrain->date);
    printf("+--------------+-------------------+---------------+\n");
}

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 10
// Function to print berth type
char* sl_berth_type(int s) {
    char* brth = malloc(4 * sizeof(char)); // Allocate enough memory for 3 characters + null terminator
    if (s > 0 && s < 864) {
        if (s % 8 == 1 || s % 8 == 4) {
            strcpy(brth, "LB");
        } else if ((s % 8 == 2) || (s % 8 == 5)) {
            strcpy(brth, "MB");
        } else if (s % 8 == 3 || s % 8 == 6) {
            strcpy(brth, "UB");
        } else if (s % 8 == 7) {
            strcpy(brth, "SLB");
        } else {
            strcpy(brth, "SUB");
        }
    } else {
        strcpy(brth, "Invalid seat number");
    }
    return brth;
}

char* ac_berth_type(int s) {
    char* brth = malloc(3 * sizeof(char)); // Allocate enough memory for 2 characters + null terminator
    if (s % 2 == 0)
        strcpy(brth, "UB");
    else
        strcpy(brth, "LB");
    return brth;
}

// Function to check if the train CSV file already exists
int trainChk(int train_no) {
    // Create a buffer to hold the file name
    char filename[20];
    // Format the file name based on the train number
    sprintf(filename, "%d.csv", train_no);

    // Attempt to open the file for reading
    FILE *file = fopen(filename, "r");

    // If file is successfully opened, close it and return 1
    if (file != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

// Structure to store seat information
typedef struct {
    char booked;
    char coach[4];
    int seatno;
    char berth[4];
    long int pnr;
    int userid;
    char status[4];
    char name[30];
    int age;
} Seat;

// Function to read seat information from the CSV file
int read_seats(const char* filename, Seat seats[], int max_seats) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;

    char line[256];
    int seat_count = 0;
    // Skip the header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && seat_count < max_seats) {
        sscanf(line, "%c,%3[^,],%d,%3[^,],%ld,%d,%3[^,],%29[^,],%d",
            &seats[seat_count].booked,
            seats[seat_count].coach,
            &seats[seat_count].seatno,
            seats[seat_count].berth,
            &seats[seat_count].pnr,
            &seats[seat_count].userid,
            seats[seat_count].status,
            seats[seat_count].name,
            &seats[seat_count].age);
        seat_count++;
    }

    fclose(file);
    return seat_count;
}

// Function to create or append to the CSV file
void create_csv(int train_no, char coach[4], long int pnr, int userid, char berth_pref[][4], int num_seats, char names[][30], int ages[],char status[]) {
    // Create the filename using the train number
    char filename[20];
    sprintf(filename, "%d.csv", train_no);

    // Check if the file already exists
    int file_exists = trainChk(train_no);

    // Open the CSV file for appending (this will create the file if it doesn't exist)
    FILE *file = fopen(filename, "a+");
    rewind(file);
    if (file == NULL) {
        printf("Error: Unable to create or open file %s\n", filename);
        return;
    }

    // Read existing seats
    Seat seats[864] = { 0 };
    int seat_count = 0;
    if (file_exists) {
        seat_count = read_seats(filename, seats, 864);
    }

    if (!file_exists) {
        // If the file does not exist, write the headers
        fprintf(file, "booked,coach,seatno,berth,pnr,userid,status,name,age\n");
    }

    int seats_booked = 0;
    int seatno = seat_count+1;

    while (seats_booked < num_seats) {
        char* berth;
        if ((strcmp("2AC", coach) == 0) || (strcmp("1AC", coach) == 0))
            berth = ac_berth_type(seatno);
        else if ((strcmp("3AC", coach) == 0) || (strcmp("SL", coach) == 0))
            berth = sl_berth_type(seatno);
        else {
            printf("Invalid coach type\n");
            fclose(file);
            return;
        }

        // Check if the current seat is booked or not
        int is_booked = 0;
        for (int j = 0; j < seat_count; j++) {
            if (seats[j].seatno == seatno && seats[j].booked == 'B') {
                is_booked = 1;
                break;
            }
        }

        // If seat is not booked and matches the preferred berth
        if (!is_booked && strcmp(berth, berth_pref[seats_booked]) == 0) {
            fprintf(file, "%c,%s,%d,%s,%ld,%d,%s,%s,%d\n", 'B', coach, seatno, berth, pnr, userid, status, names[seats_booked], ages[seats_booked]);
            seats_booked++;
        } else {
            // Mark the seat as unbooked with dummy details
            fprintf(file, "%c,%s,%d,%s,%d,%d,%s,%s,%d\n", 'U', coach, seatno, berth, 9999, 9999, "AVL", "N/A", 0);
        }

        free(berth);
        seatno++;
    }

    // Close the file
    fclose(file);

    if (seats_booked == num_seats) {
        printf("Successfully booked %d seats.\n", num_seats);
    } else {
        printf("Could not book all %d seats. Only %d seats were booked.\n", num_seats, seats_booked);
    }
}


#endif // BOOKING_V2_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 123
#define WAITING_LIST_SIZE 123

extern train_no, seat_num;
extern char coach[4];
extern char berth[6][4];
extern int userid;
extern char train_type[5];
extern long int pnr;
extern char status[3];
extern char names[6][30];
extern int ages[6];

typedef struct {
    char name[NAME_LENGTH];
    int age;
} PASSENGER;

PASSENGER day_waitingList[WAITING_LIST_SIZE];
int day_numWaitingPersons = 0;

void add_Person_To_day_WaitingList(int *wl_seats, int noOfPassenger) {

    if ((*wl_seats - noOfPassenger) < 0) {
        printf("Waiting list is full!\n");
        return;
    }
    for (int i = 0; i < noOfPassenger; i++) {
        PASSENGER newPerson;
        printf("\nEnter name: ");
        scanf("%s", newPerson.name);
        strcpy(names[0],newPerson.name);
        printf("Enter age: ");
        scanf("%d", &newPerson.age);
        ages[0]=newPerson.age;
        printf("Enter Berth Type (LB, MB, UB, SLB, SUB): ");
        scanf("%s", berth[0]);
        day_waitingList[day_numWaitingPersons] = newPerson;
        printf("Your waiting list no is: %d\n", (WAITING_LIST_SIZE - *wl_seats) + 1);
        day_numWaitingPersons++;
        double prob=predict_probability(numWaitingPersons, 0, 1, 2, 0);
        printf("CNF Probability: %f",prob);
        (*wl_seats)--;
    }
    printf("Person added to waiting list successfully.\n");
}

void display_day_List(PASSENGER list[], int numPersons, const char *listName) {
    printf("%s:\n", listName);
    printf("Name\tAge\n");
    for (int i = 0; i < numPersons; i++) {
        if (list[i].age != 0) {
            printf("%s\t%d\n", list[i].name, list[i].age);
        }
    }
}

int get_NumPassengers_From_CSV(int index, int *wl_seats, int *row_count) {
    FILE *file = fopen("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\day_v1.csv", "r");
    if (!file) {
        perror("Failed to open CSV file");
        return -1; // Return -1 on error
    }

    char buffer[256];
    int found = 0;
    int row = 0;
    char csv_data[500][256];  // Adjust size according to your file

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(csv_data[row], buffer);
        char *token = strtok(buffer, ",");
        if (token != NULL && atoi(token) == index) {
            found = 1;
            int column = 0;
            while (token != NULL) {
                column++;
                token = strtok(NULL, ",");
                if (column == 3 && token != NULL) { // 4th column for wl_seats
                    *wl_seats = atoi(token);
                    break;
                }
            }
            break;
        }
        row++;
    }
    fclose(file);

    if (!found) {
        printf("Index not found in CSV file.\n");
        return -1; // Index not found
    }
    *row_count = row;
    return 0;
}

void update_NumPassengers_In_CSV(int row_count, int numPassengers) {
    FILE *file = fopen("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\day_v1.csv", "r");
    if (!file) {
        perror("Failed to open CSV file");
        return;
    }

    char buffer[256];
    char csv_data[500][256];  // Adjust size according to your file
    int total_lines = 0;

    // Read the entire file into memory
    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(csv_data[total_lines], buffer);
        total_lines++;
    }
    fclose(file);

    // Update the specific row in memory
    char temp_buffer[256];
    strcpy(temp_buffer, csv_data[row_count]);
    char *token = strtok(temp_buffer, ",");
    char updated_line[256] = "";
    int column = 0;

    while (token != NULL) {
        column++;
        if (column == 4) { // 4th column for wl_seats
            snprintf(updated_line + strlen(updated_line), sizeof(updated_line) - strlen(updated_line), "%d,", numPassengers);
        } else {
            snprintf(updated_line + strlen(updated_line), sizeof(updated_line) - strlen(updated_line), "%s,", token);
        }
        token = strtok(NULL, ",");
    }
    // Remove the trailing comma
    if (updated_line[strlen(updated_line) - 1] == ',') {
        updated_line[strlen(updated_line) - 1] = '\0';
    }
    strcat(updated_line, "\n"); // Ensure the line ends with a newline character
    strcpy(csv_data[row_count], updated_line);

    // Write the entire file back to disk
    file = fopen("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\day_v1.csv", "w");
    if (!file) {
        perror("Failed to open CSV file for writing");
        return;
    }
    for (int i = 0; i < total_lines; i++) {
        fputs(csv_data[i], file);
    }
    fclose(file);
}

void handle_WaitingList_Update(int index, int numPassengers) {
    int wl_seats;
    int row_count;
    if (get_NumPassengers_From_CSV(index, &wl_seats, &row_count) == -1) {
        printf("Index not found or file error.\n");
        return;
    }

    add_Person_To_day_WaitingList(&wl_seats, numPassengers);
/*
    char displayOption;
    printf("Do you want to display the waiting list? (y/n): ");
    scanf(" %c", &displayOption); // Added space before %c to skip any leading whitespace
    if (displayOption == 'y' || displayOption == 'Y') {
        display_day_List(waitingList, numWaitingPersons, "Waiting List");
    }
*/
    update_NumPassengers_In_CSV(row_count, wl_seats);
}

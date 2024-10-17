#ifndef NIGHTRAINS_RAC_WL_H_INCLUDED
#define NIGHTRAINS_RAC_WL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WAITING_LIST_SIZE 123
#define RAC_LIST_SIZE 123
#define NAME_LENGTH 123

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
} Passenger;

typedef Passenger RACPassenger;

Passenger waitingList[WAITING_LIST_SIZE];
RACPassenger racList[RAC_LIST_SIZE];
int numWaitingPersons = 0;
int numRACPersons = 0;

void add_Person_To_WaitingList(int *wl_seats, int noOfPassenger) {
    if ((*wl_seats - noOfPassenger) < 0) {
        printf("Waiting list is full!\n");
        return;
    }
    for (int i = 0; i < noOfPassenger; i++) {
        Passenger newPerson;
        printf("\nEnter name: ");
        scanf("%s", newPerson.name);
        strcpy(names[0],newPerson.name);
        printf("Enter age: ");
        scanf("%d", &newPerson.age);
        ages[0]=newPerson.age;
        printf("Enter Berth Type (LB, MB, UB, SLB, SUB): ");
        scanf("%s", berth[0]);
        waitingList[numWaitingPersons] = newPerson;
        printf(BRIGHT_GREEN"Your waiting list no is: %d\n"RESET, numWaitingPersons + 1);
        numWaitingPersons++;
        double prob=predict_probability(2, 0, 1, 2, 0);
        printf("CNF Probability: %f\n",prob);
        (*wl_seats)--;
        strcpy(status,"WL");
        char snum[5];
        strcat(status,itoa(numWaitingPersons,snum,10));
        create_csv(train_no, coach, pnr, userid, berth, seat_num, names, ages, status);
    }
    printf("Person added to waiting list successfully.\n");
}

int add_Person_To_RAC(int *rac_seats, int *wl_seats) {
    if (*rac_seats <= 0) {
        printf(BRIGHT_RED"\nWe're sorry! RAC list is also full. We'll add you to the waiting list.\n"RESET);
        return 0;
    }

    int noOfPassengers;
    noOfPassengers=seat_num;

    int rac_available = *rac_seats;
    if (noOfPassengers > rac_available) {
        printf("Only %d seats are available in RAC, adding remaining %d to waiting list...\n", rac_available, noOfPassengers - rac_available);
        for (int i = 0; i < rac_available; i++) {
            RACPassenger newRACPerson;
            printf("\nEnter name: ");
            scanf("%s", newRACPerson.name);
            strcpy(names[0],newRACPerson.name);
            printf("Enter age: ");
            scanf("%d", &newRACPerson.age);
            ages[0]=newRACPerson.age;
            racList[numRACPersons] = newRACPerson;
            printf("Enter Berth Type (LB, MB, UB, SLB, SUB): ");
            scanf("%s", berth[0]);
            printf(BRIGHT_GREEN"\nYour RAC no is: %d\n"RESET, numRACPersons + 1);
            strcpy(status,"RAC");
            create_csv(train_no, coach, pnr, userid, berth, seat_num, names, ages, status);
            numRACPersons++;
            (*rac_seats)--;
        }
        add_Person_To_WaitingList(wl_seats, noOfPassengers - rac_available);
    } else {
        for (int i = 0; i < noOfPassengers; i++) {
            RACPassenger newRACPerson;
            printf("\nEnter name: ");
            scanf("%s", newRACPerson.name);
            strcpy(names[0],newRACPerson.name);
            printf("Enter age: ");
            scanf("%d", &newRACPerson.age);
            ages[0]=newRACPerson.age;
            //racList[numRACPersons] = newRACPerson;
            printf("Enter Berth Type (LB, MB, UB, SLB, SUB): ");
            scanf("%s", berth[0]);
            printf(BRIGHT_GREEN"\nYour RAC no is: %d\n"RESET, numRACPersons + 1);
            strcpy(status,"RAC");
            create_csv(train_no, coach, pnr, userid, berth, seat_num, names, ages, status);
            numRACPersons++;
            (*rac_seats)--;
        }
    }

    printf("\nPerson(s) added to RAC list successfully.\n");
    return 1;
}

void handle_train_reservation(int *rac_seats, int *wl_seats, int num) {
    train_no=num;
    FILE *file = fopen("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\night_v1.csv", "r");
    if (!file) {
        perror("Failed to open CSV file");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    int found = 0;
    char csv_data[500][256];  // Adjust size according to your file
    int row_count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(csv_data[row_count], buffer);
        char *token = strtok(buffer, ",");
        if (token != NULL && atoi(token) == num) {
            found = 1;
            int column = 0;
            while (token != NULL) {
                column++;
                token = strtok(NULL, ",");
                if (column == 5 && token != NULL) {
                    *rac_seats = atoi(token);
                } else if (column == 6 && token != NULL) {
                    *wl_seats = atoi(token);
                }
            }
        }
        row_count++;
    }
    fclose(file);

    if (!found) {
        printf("Index not found in CSV file.\n");
        return;
    }

    numWaitingPersons = WAITING_LIST_SIZE - *wl_seats;
    numRACPersons = RAC_LIST_SIZE - *rac_seats;

    int racResult = add_Person_To_RAC(rac_seats, wl_seats);
    if (!racResult) {
        int noOfPassenger;
        noOfPassenger=seat_num;
        add_Person_To_WaitingList(wl_seats, noOfPassenger);
    }

    // Update the CSV data in memory
    file = fopen("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\night_v1.csv", "w");
    if (!file) {
        perror("Failed to open CSV file for writing");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < row_count; i++) {
        char temp_buffer[256];
        strcpy(temp_buffer, csv_data[i]);
        char *token = strtok(temp_buffer, ",");
        if (token != NULL && atoi(token) == num) {
            char rest_of_row[256];
            strcpy(rest_of_row, csv_data[i] + strlen(token) + 1);  // Skip past the index column
            char *fields[7];
            fields[0] = token;
            for (int j = 1; j < 7; j++) {
                fields[j] = strtok(NULL, ",");
            }
            fprintf(file, "%s,%s,%s,%s,%s,%d,%d\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4], *rac_seats, *wl_seats);
        } else {
            fputs(csv_data[i], file);
        }
    }
    fclose(file);
}

#endif // NIGHTRAINS_RAC_WL_H_INCLUDED







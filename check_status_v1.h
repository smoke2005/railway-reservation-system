#ifndef CHECK_STATUS_V1_H_INCLUDED
#define CHECK_STATUS_V1_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define MAX_BOOKINGS 100

extern int train_no;
extern char payload_text[10000];

typedef struct {
    char booked[2];
    char coach[4];
    int seatno;
    char berth[3];
    long int pnr;
    int userid;
    char status[4];
    char name[50];
    int age;
} Booking;

int read_csv(int train_no, Booking bookings[], int max_bookings) {
    char filename[20];
    sprintf(filename, "%d.csv", train_no);

    // Check if the file already exists
    int file_exists = trainChk(train_no);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1;
    }

    char line[256];
    int count = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < max_bookings) {
        Booking *b = &bookings[count];

        char *token = strtok(line, ",");
        strcpy(b->booked, token);

        token = strtok(NULL, ",");
        strcpy(b->coach, token);

        token = strtok(NULL, ",");
        b->seatno = atoi(token);

        token = strtok(NULL, ",");
        strcpy(b->berth, token);

        token = strtok(NULL, ",");
        b->pnr = atoi(token);

        token = strtok(NULL, ",");
        b->userid = atoi(token);

        token = strtok(NULL, ",");
        strcpy(b->status, token);

        token = strtok(NULL, ",");
        strcpy(b->name, token);

        token = strtok(NULL, ",");
        b->age = atoi(token);
        count++;
    }

    fclose(file);
    return count;
}

void print_booking_status(Booking booking) {
    printf("| %-8d | %-8d | %-10s | %-3d | %-6s %-4d %-4s | %-4s |\n",
           booking.userid, booking.pnr, booking.name, booking.age,
           booking.coach, booking.seatno, booking.berth, booking.status);
}

void check_booking_status_and_print(int pnr, Booking bookings[], int num_bookings) {
    int found = 0;

    printf("+----------+----------+------------+-----+------------------+--------+\n");
    printf(BRIGHT_YELLOW"| User ID  |  PNR No  | Name       | Age | Coach Seat Berth | Status |\n"RESET);
    printf("+----------+----------+------------+-----+------------------+--------+\n");

    for (int i = 0; i < num_bookings; i++) {
        if (bookings[i].pnr == pnr) {
            found = 1;
            print_booking_status(bookings[i]);
        }
    }

    if (!found) {
        printf("| %-40s |\n", "Booking not found for pnr");
    }

    printf("+----------+----------+------------+-----+------------------+--------+\n");
}

void payloadtext(int pnr, Booking bookings[], int num_bookings) {
    int found = 0;
    char details[4000]; // Buffer for booking details
    memset(payload_text, 0, sizeof(payload_text));
    printf("%d",num_bookings);

    for (int i = 0; i < num_bookings; i++) {
        if (bookings[i].pnr == pnr) {
            found = 1;
            // Update payload text with booking details
            snprintf(details, sizeof(details),
                     "Subject: Booking Details\r\n"
                     "Thank you for booking your ticket with us. Have a safe journey!\n"
                     "+----------------------------+\r\n"
                     "|   Booking Ticket        |\r\n"
                     "+----------------------------+\r\n"
                     " User ID: %-13d\r\n"
                     " Train No: %-12d\r\n"
                     " Name: %-17s\r\n"
                     " Age: %-18d\r\n"
                     " Coach: %-10s\r\n"
                     " Seat: %-10d\r\n"
                     " Berth: %-10s\r\n"
                     " Status: %-15s\r\n"
                     " PNR: %-20ld\r\n"
                     "+----------------------------+\r\n",
                     bookings[i].userid, train_no, bookings[i].name,
                     bookings[i].age,bookings[i].coach, bookings[i].seatno,bookings[i].berth, bookings[i].status, bookings[i].pnr);

            // Check if payload_text has enough space to append details
            if (strlen(payload_text) + strlen(details) < 4096) {
                strcat(payload_text, details);
            } else {
                fprintf(stderr, "Error: payload_text buffer overflow\n");
                return;
            }
            break; // No need to continue iterating if found
        }
    }

    if (!found) {
        snprintf(details, sizeof(details),
                 "Booking not found for PNR %d\r\n", pnr);
        if (strlen(payload_text) + strlen(details) < 4096) {
            strcat(payload_text, details);
        } else {
            fprintf(stderr, "Error: payload_text buffer overflow\n");
        }
    }

}



#endif // CHECK_STATUS_V1_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 10

extern train_no, seat_num;
extern char coach[4];
extern char berth[6][4];
extern int userid;
extern char train_type[5];
extern long int pnr;
extern char status[3];
extern char names[6][30];
extern int ages[6];

void checkrac(int train_no) {
    int choice;
    printf(BRIGHT_GREEN"Do you want to go to rac \n1.yes\n2.no\nEnter 1 or 2 : "RESET);
    scanf("%d", &choice);
    if (choice == 1) {
        int rac_seats = 0, wl_seats = 0;
        handle_train_reservation(&rac_seats, &wl_seats, train_no);
    }
}

void update_night_csv(int train_no, char *class_type, int seats_to_deduct) {
    char filename[] = "C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\night_v1.csv";
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
    int i, found = 0;

    // Copy the header
    if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        fprintf(temp_file, "%s", line);
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char rline[MAX_LINE_LENGTH];
        strcpy(rline, line);

        // Split the line into fields
        fields[0] = strtok(line, ",");
        for (i = 1; i < MAX_FIELDS; i++) {
            fields[i] = strtok(NULL, ",");
            if (fields[i] == NULL) break;
        }

        if (atoi(fields[0]) == train_no) {
            found = 1;

            if (strcmp(class_type, "SL") == 0) {
                int sl_seats = atoi(fields[1]);
                if (sl_seats >= seats_to_deduct) {
                    sl_seats -= seats_to_deduct;
                    sprintf(fields[1], "%d", sl_seats);
                } else {
                    printf(BRIGHT_RED"\nWe apologize for the inconvenience! Not enough SL seats available at the moment\n"RESET);
                    checkrac(train_no);
                    fclose(file);
                    fclose(temp_file);
                    remove("temp.csv");
                    return;
                }
            } else if (strcmp(class_type, "3AC") == 0) {
                int ac3_seats = atoi(fields[2]);
                if (ac3_seats >= seats_to_deduct) {
                    ac3_seats -= seats_to_deduct;
                    sprintf(fields[2], "%d", ac3_seats);
                } else {
                    printf(BRIGHT_RED"\nWe apologize for the inconvenience! Not enough 3AC seats available at the moment\n"RESET);
                    checkrac(train_no);
                    fclose(file);
                    fclose(temp_file);
                    remove("temp.csv");
                    return;
                }
            } else if (strcmp(class_type, "2AC") == 0) {
                int ac2_seats = atoi(fields[3]);
                if (ac2_seats >= seats_to_deduct) {
                    ac2_seats -= seats_to_deduct;
                    char status="CNF";
                    sprintf(fields[3], "%d", ac2_seats);
                } else {
                    printf(BRIGHT_RED"\nWe apologize for the inconvenience! Not enough 2AC seats available at the moment\n"RESET);
                    checkrac(train_no);
                    fclose(file);
                    fclose(temp_file);
                    remove("temp.csv");
                    return;
                }
            } else if (strcmp(class_type, "1AC") == 0) {
                int ac1_seats = atoi(fields[4]);
                if (ac1_seats >= seats_to_deduct) {
                    ac1_seats -= seats_to_deduct;
                    sprintf(fields[4], "%d", ac1_seats);
                } else {
                    printf(BRIGHT_RED"\nWe apologize for the inconvenience! Not enough 1AC seats available at the moment\n"RESET);
                    checkrac(train_no);
                    fclose(file);
                    fclose(temp_file);
                    remove("temp.csv");
                    return;
                }
            }

            for (i = 0; i < MAX_FIELDS && fields[i] != NULL; i++) {
                fprintf(temp_file, "%s", fields[i]);
                if (i < MAX_FIELDS - 1 && fields[i + 1] != NULL) {
                    fprintf(temp_file, ",");
                }
                }

            fprintf(temp_file, "\n");
        } else {
            fprintf(temp_file, "%s", rline);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    remove(filename);
    rename("temp.csv", filename);
    strcpy(status,"CNF");
    for (int p=0; p<seats_to_deduct;p++){
        printf("Enter Name for seat %d: ", p + 1);
        scanf("%s", names[p]);
        printf("Enter Age for seat %d: ", p + 1);
        scanf("%d", &ages[p]);
        if (strcmp(coach, "SL") == 0 || strcmp(coach, "1AC") == 0 || strcmp(coach, "2AC") == 0) {
                printf("Enter Berth Type (LB, MB, UB, SLB, SUB): ");
                scanf("%s", berth[p]);
            } else if (strcmp(coach, "AC Chair Car") == 0 || strcmp(coach, "Executive Chair Car") == 0) {
                printf("Enter Berth Type (CC, EC): ");
                scanf("%s", berth[p]);
            } else {
                strcpy(berth[p], "Invalid");
            }
    }
    create_csv(train_no, coach, pnr, userid, berth, seat_num, names, ages, status);
    return;
}

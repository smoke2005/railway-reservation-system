#ifndef CANCEL_V1_H_INCLUDED
#define CANCEL_V1_H_INCLUDED

#include <stdio.h>
#include <string.h>
int cancel = 0;

char *cancel_ticket(int train_no, long int pnr, char name[25]) {
  cancel++;
  char booked_coach[4]; //="NA";
  char error[] = "Exit";
  char filename[20];
  int dummy = 9999;
  sprintf(filename, "%d.csv", train_no);

  FILE *file = fopen(filename, "r+");
  if (file == NULL) {
    printf("Error: Unable to create or open file %s\n", filename);
    return error;
  }

  char line[256];
  int found_pnr = 0;

  while (fgets(line, sizeof(line), file)) {
    char booked;
    char coach[4];
    int seatno;
    char berth[4];
    long int pnr_file;
    int userid;
    char status[4];
    char name_file[25];
    int age;

    sscanf(line, "%c,%3[^,],%d,%3[^,],%ld,%d,%3[^,],%24[^,],%d", &booked, coach,
           &seatno, berth, &pnr_file, &userid, status, name_file, &age);

    if (pnr_file == pnr && strcmp(name, name_file) == 0) {
      found_pnr = 1;
      fseek(file, -strlen(line), SEEK_CUR);
      fprintf(file, "U,%s,%d,%s,%ld,%d,AVL,N/A,%d\n", coach, seatno, berth,
              dummy, dummy, 0);
      strcpy(booked_coach, coach);
      break;
    }
  }
  if (found_pnr) {
        // Second pass: Update RAC to CNF and WL to RAC
        rewind(file); // Move file pointer back to the beginning of the file

        while (fgets(line, sizeof(line), file)) {
            char booked;
            char coach[4];
            int seatno;
            char berth[4];
            long int pnr_file;
            int userid;
            char status[4];
            char name_file[25];
            int age;

            // Parse the line
            sscanf(line, "%c,%3[^,],%d,%3[^,],%ld,%d,%3[^,],%24[^,],%d", &booked, coach,
                   &seatno, berth, &pnr_file, &userid, status, name_file, &age);

            // Update the first RAC ticket to CNF
            if (strcmp(status, "RAC") == 0) {
                fseek(file, -strlen(line), SEEK_CUR);
                fprintf(file, "%c,%s,%d,%s,%ld,%d,CNF,%s,%d\n", booked, coach, seatno, berth,
                        pnr_file, userid, name_file, age);
                break; // Exit the loop after updating the first RAC ticket
            }
        }

        // Third pass: Update the first WL ticket to RAC
        rewind(file); // Move file pointer back to the beginning of the file

        while (fgets(line, sizeof(line), file)) {
            char booked;
            char coach[4];
            int seatno;
            char berth[4];
            long int pnr_file;
            int userid;
            char status[4];
            char name_file[25];
            int age;

            // Parse the line
            sscanf(line, "%c,%3[^,],%d,%3[^,],%ld,%d,%3[^,],%24[^,],%d", &booked, coach,
                   &seatno, berth, &pnr_file, &userid, status, name_file, &age);

            // Update the first WL ticket to RAC
            if (strcmp(status, "WL1") == 0) {
                fseek(file, -strlen(line), SEEK_CUR);
                fprintf(file, "%c,%s,%d,%s,%ld,%d,RAC,%s,%d\n", booked, coach, seatno, berth,
                        pnr_file, userid, name_file, age);
                break; // Exit the loop after updating the first WL ticket
            }
        }

        printf(BRIGHT_GREEN"Ticket for %s is cancelled successfully\n"RESET, name);
    } else {
        printf("PNR or Name Not Found\n");
    }

  fclose(file);

  return booked_coach;
}

/*void cancel_update(int dn, int train, char coach[4]) {
  int found_tr = -1;
  char line[256];
  if (dn == 1) {
    FILE *file = open("night.csv", "r+");
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
      int train_no;
      int sl_seat;
      int ac3_seat;
      int ac2_seat;
      int ac1_seat;
      sscanf("%d,%d,%d,%d,%d", train_no, sl_seat, ac3_seat, ac2_seat, ac1_seat);
      if (train_no == train) {
        found_tr = 1;
        fseek(file, -strlen(line), SEEK_CUR);
        fprintf(file, "%d,%d,%d,%d,%d\n", train_no, sl_seat, ac3_seat, ac2_seat,
                ac1_seat);
        printf("Updated Main Night Trains");
        break;
      }
    }
  }
    else if (dn == 0) {
      FILE *file = open("day.csv", "r+");
      fgets(line, sizeof(line), file);
      while (fgets(line, sizeof(line), file)) {
        int train_no;
        int ac_seat;
        int exec_seat;
        sscanf("%d,%d,%d", train_no, ac_seat, exec_seat);
        if (train_no == train) {
          found_tr = 1;
          fseek(file, -strlen(line), SEEK_CUR);
          fprintf(file, "%d,%d,%d,%d,%d\n", train_no, ac_seat, exec_seat);
          printf("Updated Main Day Trains");
          break;
        }
      }
    }
}
*/
//char a[]="kkkkkkk";
//printf("s");
void cancel_update(int dn, int train, char coach[4]) {
  int found_tr = 0;
  char line[256];
  FILE *file;
  char filename[500];

  if (dn == 1) {
    strcpy(filename, "C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\day_v1.csv");

  } else {
    strcpy(filename, "C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\night_v1.csv");
  }

  file = fopen(filename, "r+");
  if (file == NULL) {
    printf("Error opening file\n");
    return;
  }
  printf("file read");
  fgets(line, sizeof(line), file); // skip header
  while (fgets(line, sizeof(line), file)) {
    int train_no=0;
    int sl_seat=0, ac3_seat=0, ac2_seat=0, ac1_seat=0, ac_seat=0, exec_seat=0, wl=0, rac=0;
    if (dn == 1) {
      sscanf(line, "%d,%d,%d,%d,%d,%d,%d", &train_no, &sl_seat, &ac3_seat, &ac2_seat,
             &ac1_seat, &rac, &wl);
    } else {
      int ac_seat, exec_seat;
      sscanf(line, "%d,%d,%d,%d", &train_no, &ac_seat, &exec_seat, &wl);
    }

    if (train_no == train) {
      found_tr = 1;
      if (dn == 1) {
        if (strcmp(coach, "SL") == 0) {
          sl_seat++;
        } else if (strcmp(coach, "AC3") == 0) {
          ac3_seat++;
        } else if (strcmp(coach, "AC2") == 0) {
          ac2_seat++;
        } else if (strcmp(coach, "AC1") == 0) {
          ac1_seat++;
        }
        if(wl<123 && rac<123){
          wl++;
        }
        else if(wl==123 && rac<123){
          rac++;
        }

        fseek(file, -strlen(line), SEEK_CUR);
        fprintf(file, "%d,%d,%d,%d,%d,%d,%d\n", train_no, sl_seat, ac3_seat, ac2_seat,
                ac1_seat, rac, wl);
      } else {
        strcpy(coach, "AC");
        if (strcmp(coach, "AC") == 0) {
          ac_seat++;
        } else if (strcmp(coach, "EXEC") == 0) {
          exec_seat++;
        }
        fseek(file, -strlen(line), SEEK_CUR);
        fprintf(file, "%d,%d,%d\n", train_no, ac_seat, exec_seat);
      }
      printf("Updated Main %s Trains\n", dn == 1 ? "Night" : "Day");
      break;
    }
  }

  if (!found_tr) {
    printf("Train not found\n");
  }

  fclose(file);
}



#endif // CANCEL_V1_H_INCLUDED

#include "login_v1.h"
#include "booking_v2.h"
#include "path_v1.h"
#include "payment_v1.h"
#include "mail_v1.h"
#include "check_status_v1.h"
#include "prediction_v1.h"
#include "curl.h"
#include "Night_train_book.h"
#include "nightrains_rac_wl.h"
#include "daytrain_wl.h"
#include "daytrains_book.h"
#include "cancel_v1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BRIGHT_RED  "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

#define MAX_LENGTH 50
#define MAX_USERS 100
#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 5
#define WAITING_LIST_SIZE 123
#define RAC_LIST_SIZE 123
#define NAME_LENGTH 123
#define MAX_TRAIN_NAME_LENGTH 50
#define MAX_ORIGIN_LENGTH 50
#define MAX_DESTINATION_LENGTH 50
#define MAX_DATE_LENGTH 15

int train_no=0, seat_num=0;
char coach[4]="SL";
char train_type[5]="night";
int login_flag=0;
long int pnr;
char status[3]="CNF";
char names[6][30];
int ages[6];
char berth[6][4];
int userid=0;
char payload_text[10000];

int main() {

    printf(CYAN"=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"                                         Welcome to the Train Reservation System\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN "Discover the joy of hassle-free ticket booking and embark on memorable journeys with our Railway Reservation System. Whether you're planning a leisurely getaway or a business trip, travel with ease and excitement, knowing that every journey with us is a delightful adventure waiting to unfold. Explore the world confidently, one ticket at a time, and unlock boundless possibilities for unforgettable travel memories. Start your journey with us today!\n" RESET);

    const char *filename = "user_details.csv";
    User users[MAX_USERS];

    int num_users = read_users(filename, users);

    if (num_users == -1) {
        printf("Failed to read users from file.\n");
        return 1;
    }

    printf("\n1. Login\n2. Register\n3. Forgot password ");
    int choice;
    printf("\n\nEnter your choice:");
    while (1){
        scanf("%d", &choice);
        if (choice<1 || choice>3)
            printf("Enter a valid choice!");
        else
            break;
    }

    if (choice==1) {
        userid=login(users, num_users);
    } else if (choice==2) {
        userid=register_user(users, num_users);
    } else if (choice==3) {
        forgot_password(users, num_users);
    }
    fflush(stdin);

    printf(CYAN"\n=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"                                         Here are the services we provide\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"1. Book a train\n2. Cancel your ticket\n3. Check Booking Status\n4.Exit"RESET);

    while (1){
        printf("\n\nChoose one of our services:");
        scanf("%d",&choice);
        if (choice==1){
            fflush(stdin);
            char orig[MAX_ORIGIN_LENGTH];
            char dest[MAX_DESTINATION_LENGTH];
            char dat[MAX_DATE_LENGTH];
            Train train;
            Train* filtered_trains = NULL;
            int num_filtered_trains = 0;
            FILE *ptr;

            // Get user input for origin, destination, and date
            getUserInput(orig, dest, dat);

            // Read train data from the CSV file
            ptr = fopen("trains_v1.csv", "r");
            if (ptr == NULL) {
                printf("Error opening file: modified_file1.csv\n");
                return 1;
            }
            while (1) {
                char* rec_line = readLine(ptr);
                if (rec_line == NULL) {
                    break;
                }
                if (parseTrainData(rec_line, &train) != 10) {
                    printf("Error parsing train data: %s\n", rec_line);
                    free(rec_line);
                    continue;
                }
                if (strcmp(train.origin, orig) == 0 &&
                    strcmp(train.destination, dest) == 0 &&
                    strcmp(train.date, dat) == 0) {
                    filtered_trains = realloc(filtered_trains, sizeof(Train) * (num_filtered_trains + 1));
                    filtered_trains[num_filtered_trains] = train;
                    num_filtered_trains++;
                }
                free(rec_line);
            }
            fclose(ptr);

            if (num_filtered_trains==0){
                printf("\n");
                printf(BRIGHT_GREEN"Sorry, there are currently no direct train routes between the given stations. "RESET);
                printf(BRIGHT_GREEN"However, here's an alternate path suggested between the two stations. If you wish to, you can book the connecting trains\n\n"RESET);
                Graph graph;
                graph.numStations = 0;
                graph.numEdges = 0;

                // Load graph data from CSV file
                LoadGraphFromCSV(&graph, "C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\modified_trains4.csv");
                // Find station indices for the given station names
                int sourceIndex = -1;
                int destinationIndex = -1;
                for (int i = 0; i < graph.numStations; i++) {
                    if (strcmp(graph.stations[i].name, orig) == 0) {
                    sourceIndex = i;
                    }
                    if (strcmp(graph.stations[i].name, dest) == 0) {
                    destinationIndex = i;
                    }
                }

                // Check if both source and destination stations were found
                if (sourceIndex == -1) {
                    printf("Source station not found.\n");
                    return 1;
                }
                if (destinationIndex == -1) {
                    printf("Destination station not found.\n");
                    return;
                }

                // Find connecting trains between the given stations
                FindConnectingTrains(&graph, sourceIndex, destinationIndex);
            }

            else{
                // Print filtered train data
                printf("\nHere are the trains available between the given stations:\n");
                for (int i = 0; i < num_filtered_trains; i++) {
                    printTrainData(&filtered_trains[i]);
                }

                // Free memory
                free(filtered_trains);


            strcpy(train_type,train.type);
            printf("\nEnter Train Number: ");
            scanf("%d", &train_no);

            printf("\nEnter Number of seats: ");
            scanf("%d", &seat_num);

            if (train_type == 0) {
                printf("Coaches Available:");
                printf("\nAC Chair Car\nExecutive Chair Car:\n");

            } else {
                printf(CYAN"Coaches Available:"RESET);
                printf(BRIGHT_CYAN"\n1AC\n2AC\n3AC\nSL\n"RESET);

            }
            printf("\nEnter Coach Preference : ");
            fflush(stdin);
            scanf("%s", coach);

            long int generate_pnr() {
            long int pnr = 0;

            // Generate each digit of the 9-digit number
            for (int i = 0; i < 9; ++i) {
                long int digit = rand() % 10;
                pnr = pnr * 10 + digit;
            }

            return pnr;
            }
            srand((unsigned int)time(NULL));

            // Generate and print PNRs

            pnr = generate_pnr();

            if (*train.type == 0){
                handle_WaitingList_Update(train_no,seat_num);
            }
            if (*train.type==1){
                update_night_csv(train_no,coach,seat_num);
            }


            char filename[200];
            int bookedSeats;

            printf("%d",*train.type);
/*          if (*train.type==0)
                update_csv("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\day.csv", train_no, coach, seat_num);
            if (*train.type==1)
                bookedSeats=update_csv("C:\\Users\\mokit\\OneDrive\\Documents\\SSN\\Software Development Project\\night.csv", train_no, coach, seat_num);
*/

            int isSeniorCitizen = 0;
            int isStudent = 0;
            float fare=0.0;
            int age=0;
            char name[50];
            float convenienceCharges=0;
            float concessions ;
            for (int i=0;i<seat_num;i++){

                /*if (age>60)
                    isSeniorCitizen=1;
                if (age<21)
                    isStudent=1;*/


                int totalSeats = 1232;
                UpdateBaseFareMultiplier(totalSeats, bookedSeats);
                fare = calculateTaxes(coach, seat_num) * baseFareMultiplier;
                int ch = get_payment_info();
                if (ch == -1) {
                    printf("Payment method selection failed\n");
                    return 1; // indicate error
                }
                convenienceCharges = calculateConvenienceCharges(ch);
                concessions = calculateConcessions(isSeniorCitizen, isStudent);

                fare += convenienceCharges;
                fare -= fare * concessions; // apply concessions
            }
            printf("\n");
            printf(CYAN"**************************\n"RESET);
            printf(BRIGHT_CYAN"*       Ticket Bill      *\n"RESET);
            printf(CYAN"**************************\n"RESET);
            printf(BRIGHT_CYAN"Base Fare: Rs. %.2f\n"RESET, fare);
            printf(BRIGHT_CYAN"Convenience Charges: Rs. %.2f\n"RESET, convenienceCharges);
            printf(BRIGHT_CYAN"Concessions: Rs. %.2f\n"RESET, fare * concessions);
            printf(BRIGHT_CYAN"Total Amount: Rs. %.2f\n"RESET, fare);
            printf(CYAN"**************************\n"RESET);

            char pay[3];
            printf("\nAre you sure you want to proceed with the payment: ");
            scanf("%s",pay);
            if ((strcmp(pay,"yes")==0) || (strcmp(pay,"Yes")==0)){
            const char *email = get_user_email(userid, users, num_users);
            if (email) {
                printf("Email for user ID %d: %s\n", userid, email);
            } else {
                printf("User ID %d not found.\n", userid);
                return 1;
            }


            Booking bookings[MAX_BOOKINGS];
            int num_bookings = read_csv(train_no, bookings, MAX_BOOKINGS);
            snprintf(payload_text, sizeof(payload_text),
             "Date: Mon, 29 Nov 2021 21:54:29 +0000\r\n"
             "To: keerthana.mokitha@gmail.com\r\n"
             "From: mokithapriyan@gmail.com\r\n"
             "Subject: Booking Details\r\n"
             "\r\n");

            payloadtext(pnr,bookings,num_bookings);

            char to_mail[MAX_LENGTH];
            snprintf(to_mail,sizeof(to_mail),"<%s>",email);

            struct upload_status {
                int lines_read;
            };

            CURL *curl;
            CURLcode res = CURLE_OK;
            struct upload_status upload_ctx = { 0 };

            curl = curl_easy_init();
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
                curl_easy_setopt(curl, CURLOPT_USERNAME, "mokithapriyan@gmail.com");
                curl_easy_setopt(curl, CURLOPT_PASSWORD, "rbck orpa mkuu eoym");
                curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<mokithapriyan.com>");

                struct curl_slist *recipients = NULL;
                recipients = curl_slist_append(recipients, to_mail);
                curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

                curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
                curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

                res = curl_easy_perform(curl);

                if(res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                else
                    printf("Email sent successfully\n");

                curl_slist_free_all(recipients);
                curl_easy_cleanup(curl);
            }
            printf(BRIGHT_GREEN"\nConfirmation status has been sent to your mail!"RESET);

        }

        printf(BRIGHT_GREEN"\nHere's your PNR: %ld. If you wish to, you can check your booking status with it."RESET, pnr);

        }
        }
    else if (choice==2){
        printf("Enter Train Number: ");
        if (scanf("%d", &train_no) != 1) {
            printf("Invalid input for train number.\n");
            return 1;
        }
        char name[50];
        char *coach1;
        printf("Enter PNR Number: ");
        scanf("%ld", &pnr);

        printf("Enter Passenger Name: ");
        scanf("%24s", name);

        coach1 = cancel_ticket(train_no, pnr, name);
        float baseFare = 0.0;
        if (strcmp(coach, "SL") == 0) {
            baseFare = 124.0;
        } else if (strcmp(coach, "3AC") == 0) {
            baseFare = 440.0;
        } else if (strcmp(coach, "2AC") == 0) {
            baseFare = 625.0;
        } else if (strcmp(coach, "1AC") == 0) {
            baseFare = 1059.0;
        } else if (strcmp(coach, "CC") == 0) {
            baseFare = 211.0;
        } else if (strcmp(coach, "EC") == 0) {
            baseFare = 1205.0;
        }
        float return_amt = baseFare * 0.25;
        printf("\nRefund of %f initiated.",return_amt);

        //cancel_update(1, train_no, coach1);
    }
    else if (choice==3){

        printf("Enter Train Number: ");
        if (scanf("%d", &train_no) != 1) {
          printf("Invalid input for train number.\n");
          return 1;
        }

        printf("Enter PNR Number: ");
        scanf("%d", &pnr);

        Booking bookings[MAX_BOOKINGS];
        int num_bookings = read_csv(train_no, bookings, MAX_BOOKINGS);

        if (num_bookings == -1) {
            return 1;
        }

        // Check booking status
        check_booking_status_and_print(pnr, bookings, num_bookings);
    }
    else if (choice==4){
        printf("Exitting");
        break;
    }

    else
        printf("Enter valid choice");
    }
    return 0;
}



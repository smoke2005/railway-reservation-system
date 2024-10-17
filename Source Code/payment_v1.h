#ifndef PAYMENT_V1_H_INCLUDED
#define PAYMENT_V1_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAX_RATE 0.1
#define BASE_FARE_MULTIPLIER_INCREMENT 0.1
#define MAX_BASE_FARE_MULTIPLIER 1.5

float bookedPercentage = 0.0;
float baseFareMultiplier = 1.0;

float calculateTaxes(char bookingClass[], int numSeats) {
    float baseFare = 0.0;
    if (strcmp(bookingClass, "SL") == 0) {
        baseFare = 124.0;
    } else if (strcmp(bookingClass, "3AC") == 0) {
        baseFare = 440.0;
    } else if (strcmp(bookingClass, "2AC") == 0) {
        baseFare = 625.0;
    } else if (strcmp(bookingClass, "1AC") == 0) {
        baseFare = 1059.0;
    } else if (strcmp(bookingClass, "CC") == 0) {
        baseFare = 211.0;
    } else if (strcmp(bookingClass, "EC") == 0) {
        baseFare = 1205.0;
    }
    float totalFare = baseFare * numSeats;
    return totalFare * TAX_RATE + totalFare;
}

float calculateConvenienceCharges(int ch) {
    if (ch == 1) {
        return 30.0;
    } else if (ch == 2) {
        return 30.0;
    } else if (ch == 3) {
        return 20.0;
    } else {
        return 0.0;
    }
}

float calculateConcessions(int isSeniorCitizen, int isStudent) {
    float discount = 0.0;
    if (isSeniorCitizen) {
        discount += 0.1; // 10% discount for senior citizens
    }
    if (isStudent) {
        discount += 0.05; // 5% discount for students
    }
    return discount;
}

void UpdateBaseFareMultiplier(int totalSeats, int bookedSeats) {
    float percentage = (float)bookedSeats / totalSeats * 100.0;
    bookedPercentage = percentage;
    baseFareMultiplier = 1.0 + (percentage / 10.0) * BASE_FARE_MULTIPLIER_INCREMENT;
    if (baseFareMultiplier > MAX_BASE_FARE_MULTIPLIER) {
        baseFareMultiplier = MAX_BASE_FARE_MULTIPLIER;
    }
}

int ValidateCardNumber(char CardNumber[]) {
    int len = strlen(CardNumber);
    if (len != 16)
        return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(CardNumber[i])) {
            return 0;
        }
    }
    return 1;
}

int ValidateCVV(char cvv[]) {
    int len = strlen(cvv);
    if (len != 3)
        return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(cvv[i])) {
            return 0;
        }
    }
    return 1;
}

int get_payment_info() {
    int ch;
    char CardNumber[17]; // 16 digits + null terminator
    char cvv[4];         // 3 digits + null terminator
    char pin[7];         // 6 digits + null terminator
    char bankName[50];
    char accountNumber[20];
    printf(CYAN"\n=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"                                                    Payment Gateway\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);
    printf("\n");
    printf("Available payment methods:\n");
    printf(BRIGHT_YELLOW"\t1. Credit Card\n"RESET);
    printf(BRIGHT_YELLOW"\t2. Debit Card\n"RESET);
    printf(BRIGHT_YELLOW"\t3. Net Banking\n"RESET);
    printf("\nEnter payment method: ");
    if (scanf("%d", &ch) != 1 || ch < 1 || ch > 3) {
        printf("Invalid input\n");
        return -1;
    }
    getchar(); // clear input buffer

    switch (ch) {
        case 1:
            printf("Enter card number (16 digits): ");
            gets(CardNumber); // using gets
            if (!ValidateCardNumber(CardNumber)) {
                printf("Invalid card number format\n");
                return -1;
            }

            printf("Enter CVV: ");
            gets(cvv); // using gets
            if (!ValidateCVV(cvv)) {
                printf("Invalid CVV format\n");
                return -1;
            }
            return 1;
        case 2:
            printf("Enter debit card number: ");
            gets(CardNumber); // using gets
            if (!ValidateCardNumber(CardNumber)) {
                printf("Invalid card number format\n");
                return -1;
            }

            printf("Enter CVV: ");
            gets(cvv); // using gets
            if (!ValidateCVV(cvv)) {
                printf("Invalid CVV format\n");
                return -1;
            }

            printf("Enter PIN: ");
            gets(pin); // using gets
            return 2;
        case 3:
            printf("Enter your bank name: ");
            gets(bankName); // using gets

            printf("Enter your account number: ");
            gets(accountNumber); // using gets
            return 3;
        default:
            printf("Invalid payment method\n");
            return -1;
    }
}


#endif // PAYMENT_V1_H_INCLUDED

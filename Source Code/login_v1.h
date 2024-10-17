#ifndef LOGIN_V1_H_INCLUDED
#define LOGIN_V1_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

typedef struct {
    int userid;
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char name[MAX_LENGTH];
    int age;
    char dob[MAX_LENGTH];
    char gender[MAX_LENGTH];
    char email[MAX_LENGTH];
} User;

int get_next_userid() {
    FILE *file = fopen("user_details.csv", "r");
    if (file == NULL) return 1000;

    char line[MAX_LENGTH * 8]; // Adjust size for new fields
    int max_id = 0;
    while (fgets(line, sizeof(line), file)) {
        int userid;
        sscanf(line, "%d,", &userid);
        if (userid > max_id) {
            max_id = userid;
        }
    }
    fclose(file);
    return max_id + 1;
}

// Check for valid password
int valid_password(const char *password) {
    int length = strlen(password);
    if (length < 8) {
        return 1; // Password is too short
    }
    for (int i = 0; i < length; i++) {
        if (!isdigit(password[i])) {
            return 1; // Password contains non-digit character
        }
    }
    return 0; // Password meets all criteria
}

// Check for valid username
int valid_alphabet(const char *str) {
    while (*str) {
        if (!isalpha(*str)) {
            return 1; // Return true if a non-alphabetic character is found
        }
        str++;
    }
    return 0; // Return false if only alphabetic characters are found
}

const char* get_user_email(int userid, User users[], int num_users) {
    for (int i = 0; i < num_users; i++) {
        if (users[i].userid == userid) {
            return users[i].email;
        }
    }
    return NULL; // Return NULL if userid is not found
}

// Function to parse CSV line and populate User structure
User check_user(const char *line) {
    User user;
    char *token;
    const char identifier[] = ",";

    // Tokenize the line based on commas
    token = strtok((char *)line, identifier);
    if (token != NULL) {
        user.userid = atoi(token);
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        strncpy(user.username, token, MAX_LENGTH);
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        strncpy(user.password, token, MAX_LENGTH);
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        strncpy(user.name, token, MAX_LENGTH);
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        user.age = atoi(token); // Convert string to integer
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        strncpy(user.dob, token, MAX_LENGTH);
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        strncpy(user.gender, token, MAX_LENGTH);
    }
    token = strtok(NULL, identifier);
    if (token != NULL) {
        strncpy(user.email, token, MAX_LENGTH);
    }

    return user;
}

// Function to check if username and password match
int check_match(const char *username, const char *password, User users[], int num_users, int *userid) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *userid = users[i].userid; // Set the userid of the matched user
            return 1; // Credentials match
        }
    }
    return 0; // Credentials do not match
}

// Function to read users from CSV file
int read_users(const char *filename, User users[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int num_users = 0;
    char line[MAX_LENGTH * 8]; // Adjust size for new fields

    while (fgets(line, sizeof(line), file) != NULL && num_users < MAX_USERS) {
        users[num_users++] = check_user(line);
    }

    fclose(file);
    return num_users;
}

// Function to write users to CSV file
void write_users(const char *filename, User users[], int num_users) {
    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        return;
    }

    for (int i = 0; i < num_users; i++) {
        fprintf(file, "%d,%s,%s,%s,%d,%s,%s,%s", users[i].userid, users[i].username, users[i].password, users[i].name, users[i].age, users[i].dob, users[i].gender, users[i].email);
    }

    fclose(file);
}

// Function to prompt user to login
int login(User users[], int num_users) {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    int userid = -1;

    printf(CYAN"\n=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"                                                       Login Portal\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"To login, please enter your username and password.\n"RESET);
    printf(BRIGHT_CYAN"Username should contain only alphabets.\n"RESET);
    printf(BRIGHT_CYAN"Password should be at least 8 characters long and contain only digits.\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);

    while (1) {
        printf("Enter username: ");
        scanf("%s", username);

        if (valid_alphabet(username)) {
            printf("Incorrect username. It should contain only alphabets.\n");
        } else {
            break;
        }
    }

    while (1) {
        printf("Enter password: ");
        scanf("%s", password);

        if (valid_password(password)) {
            printf(BRIGHT_RED"Invalid password. It should be at least 8 characters long and contain only digits.\n"RESET);
        } else {
            break;
        }
    }

    if (check_match(username, password, users, num_users, &userid)) {
        printf(BRIGHT_GREEN"Login successful! Welcome, %s.\n"RESET, username);
        return userid; // Return the userid after successful login
    } else {
        printf("Username and password not found.\n");
        printf("Would you like to register as a new user? (yes/no): ");
        char choice[MAX_LENGTH];
        scanf("%s", choice);
        if (strcmp(choice, "yes") == 0) {
            return register_user(users, num_users); // Call the registration function and return the new userid
        } else {
            printf("Would you like to reset your password? (yes/no): ");
            scanf("%s", choice);
            if (strcmp(choice, "yes") == 0) {
                forgot_password(users, num_users);
                return -1; // Indicate password reset process initiated
            }
        }
    }
    return -1; // Indicate login or registration failure
}

// Function to register a new user
int register_user(User users[], int num_users) {
    FILE *file;
    char filename[] = "user_details.csv";
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char name[MAX_LENGTH];
    char dob[MAX_LENGTH];
    char gender[MAX_LENGTH];
    char email[MAX_LENGTH];

    int userid = get_next_userid();

    // Open the CSV file for appending
    file = fopen(filename, "a+");
    if (file == NULL) {
        printf(BRIGHT_RED"Error opening file for writing.\n"RESET);
        return -1;
    }

    printf(CYAN"\n=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"                                                   User Registration\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"To register, please enter your details.\n"RESET);
    printf(BRIGHT_CYAN"Username should contain only alphabets.\n"RESET);
    printf(BRIGHT_CYAN"Password should be at least 8 characters long and contain only digits.\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);

    while (1) {
        printf("Enter username: ");
        scanf("%s", username);
        if (valid_alphabet(username)) {
            printf(BRIGHT_RED"Incorrect username. It should contain only alphabets.\n"RESET);
        } else {
            break;
        }
    }

    while (1) {
        printf("Enter password: ");
        scanf("%s", password);
        if (valid_password(password)) {
            printf(BRIGHT_RED"Invalid password. It should be at least 8 characters long and contain only digits.\n"RESET);
        } else {
            break;
        }
    }

    printf("Enter full name: ");
    scanf("%s", name);

    printf("Enter date of birth (dd-mm-yy): ");
    scanf("%s", dob);

    printf("Enter gender (F or M): ");
    scanf("%s", gender);

    printf("Enter email id: ");
    scanf("%s", email);

    // Write user data to the file
    fprintf(file, "%d,%s,%s,%s,0,%s,%s,%s", userid, username, password, name, dob, gender, email);
    fclose(file);
    printf(BRIGHT_GREEN"User registered successfully!\n"RESET);

    return userid; // Return the new user's userid after successful registration
}

// Function to reset password
void forgot_password(User users[], int num_users) {
    char username[MAX_LENGTH];
    char new_password[MAX_LENGTH];

    printf(CYAN"\n=====================================================================================================================================\n"RESET);
    printf("\n");
    printf(BRIGHT_CYAN"                                                     Forgot Password\n"RESET);
    printf("\n");
    printf(CYAN"=====================================================================================================================================\n"RESET);
    while (1) {
        printf("Enter your username: ");
        scanf("%s", username);

        if (valid_alphabet(username)) {
            printf(BRIGHT_RED"Incorrect username. It should contain only alphabets.\n"RESET);
        } else {
            break;
        }
    }

    int user_found = 0;
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            user_found = 1;
            while (1) {
                printf("Enter new password: ");
                scanf("%s", new_password);
                if (valid_password(new_password)) {
                    printf(BRIGHT_RED"Invalid password. It should be at least 8 characters long and contain only digits.\n"RESET);
                } else {
                    break;
                }
            }
            strncpy(users[i].password, new_password, MAX_LENGTH);
            write_users("user_details.csv", users, num_users);
            printf(BRIGHT_GREEN"Password reset successfully!\n"RESET);
            break;
        }
    }

    if (!user_found) {
        printf("Username not found. Exiting the program.\n");
    }
}

#endif // LOGIN_V1_H_INCLUDED

# Railway Reservation System

This project, built for the Fundamentals and Practice of Software Development, is an integrated train ticket booking software, that facilitates users to login and book tickets for their journey, along with several helpful features such as connected train suggestions, confirmation probability prediction and email ticket and payment confirmation.

# TABLE OF CONTENTS

1. Aim
2. Features Offered
3. Libraries Used
4. Integration/Execution Sequence
5. Compilers Used
6. Files Used

# AIM

Railway Reservation System of team B1_7 aims to give passengers booking train tickets online a smooth and user-friendly booking process, provide choices for the user to apply their preferences and give the users a comprehensive idea of their journey, to help them make appropriate decisions.

# FEATURES OFFERED

- Log in and Sign in with user details and password
- Display of available trains for journey
- Connected trains display for unconnected stations
- Seat booking based on Coach/Berth preferences
- Dynamic pricing for tickets, along with age concessions
- Realistic payment process, along with functioning emailing system
- Waiting list and RAC booking
- Probability of confirmation, in the case of waiting list, based on historic database

# LIBRARIES USED

## Standard Libraries

- ‘stdio.h’: For implementing I/O operations
- ‘string.h’: For using built-in functions for strings
- ‘stdlib.h’: For efficient memory management
- ‘curl.h’: For handling HTTP requests
- ‘ctype.h’: For character classification, to be used in error handling
- ‘math.h’: For using mathematical built-in functions

## User Defined Library Functions

- login_v1.h: User authentication module
- booking_v2.h: Booking management module
- path_v1.h: Pathfinding module
- payment_v1.h: Payment processing module
- mail_v1.h: Email notification module
- Night_train_book.h: Night train booking management
- nightrains_rac_wl.h: Night train RAC and waiting list management
- daytrain_wl.h: Day train waiting list management
- daytrains_book.h: Day train booking management
- booking_final.h: Stores reservation chart for each train, along with passenger details for each seat
- prediction.h: Probability of waiting list ticket getting confirmed
- cancelation.h: Canceling tickets, modifying available, updating reservation chart and updating waiting list and RAC list.

# INTEGRATION/EXECUTION SEQUENCE

Executed as a menu driven program that gives the user choice to course of action, the main C function, named ‘main_v2.c’ begins execution.

1. Login module, integrated into the main function by the header file ‘login_v1.h’ gets executed.
2. The login function used here gets user name and password as inputs and proceeds to nest functions. In case register option was chosen, user details are received, along with user created password and then login function gets executed. If the forgot password option was chosen, password is remade, confirmed, then the login function gets executed.
3. After login, search and display function, along with get_user_input function gets executed.
4. Path finder module, integrated through the ‘path.h’ library gets executed. Connected train suggestions (if any), train name, arrival date and time get displayed in a table-like structure.
5. Booking function, provided under header file ‘booking_v2.h’ is then executed. User input on number of tickets needed and coach class is received. Here, the total number of seats across available trains is changed accordingly, using two different CSV files for night trains and day trains, and separate functions for booking modules - "Night_train_book.h" and “daytrains_book.h” respectively.
6. If the available number of seats gets reduced to a certain threshold, the update_waiting list and add_waiting_list functions given in "nightrains_rac_wl.h" and "daytrain_wl.h", with appropriate consideration to day and night trains, get executed.
7. After waiting list updation modules execute, control flows to the probability_prediction function in prediction.h module, probability gets displayed.
8. Then payment generation module, which calculates ticket fare in dynamic fare calculation and gives the passenger a PNR number executes, along with email generation function in mail_v1.h .
9. Once successfully booked, user gets an option to exit or to cancel ticket. If cancel ticket operation is chosen, cancelation.h module executes, user needs to enter their PNR number and name for the ticket to be canceled and the corresponding ticket will be canceled.

# FILES USED

- “modified_trains(1).csv”: Data on train number, train name, train journey stations, arrival time, departure time, travel duration. This data was downloaded from IRCTC website and is used after proper preprocessing.
- “pred_data33.csv”: Historic data used for the probability prediction module.
- “day.csv”: Number of available seats in each coach appropriate for all day trains.
- “night.csv”: Number of available seats in each coach appropriate for all night trains.
- “waiting_list.csv”: User details along with waiting list number.
- “rac_list.csv”: User details along with rac number.

# COMPILERS USED

- C Compiler in terminal, accessed using command prompt.
- Code blocks application compiler.
- Replit online C compiler.

# ACKNOWLEDGEMENTS

This integrated software project implemented in C programming language was developed as a part of \[UCS2201-Fundamentals and Practice of Software Development\] course in the Second Semester of B.E. Computer Science and Engineering Degree offered by SSN institutions.

# Demo

This is a demo video of the application. Since the file size is large, download the raw video to view it.
![Demo Video](demo-rrs.mp4)

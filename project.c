#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LOADING_DELAY 90000
#define LOADING_CYCLES 24

#define MAX_USERS 10
#define MAX_SEATS 40
#define FILENAME "bus_data.txt"  // File name for saving bus data
#define USER_FILE "users.txt"    // File name for saving user data

// Structure to represent a user
struct User
{
    char username[50];
    char password[50];
    char paymentMethod[20];
    int numTickets;
    float totalPrice;
};

// Structure to represent a bus (now a node in a linked list)
struct Bus
{
    int id;
    char route[50];
    char departureTime[20];
    char arrivalTime[20];
    int availableSeats;
    float ticketPrice;
    int seats[MAX_SEATS]; // Array to track seat availability
    struct Bus *next;    // Pointer to the next bus in the list
};

// Function prototypes
void initializeBuses(struct Bus **head);                          // Initialize bus data
void displayAvailableBuses(const struct Bus *head);               // Display available buses
void bookTicket(struct Bus *head, int busId, struct User *user);  // Book a ticket
void cancelTicket(struct Bus *head, int busId, struct User *user); // Cancel a ticket
void saveBusData(const struct Bus *head);                         // Save bus data to file
void loadBusData(struct Bus **head);                              // Load bus data from file
int registerUser(struct User *users, int count);                  // Register a user
int login(struct User *users, int count);                         // Login
void saveUser(struct User *users, int count);                     // Save user data to file
void adminMenu(struct Bus **head, struct User *users, int *userCount); // Admin menu
void userMenu(struct Bus *head, struct User *users, int *userCount);  // User menu
void addBus(struct Bus **head);                                   // Add a new bus
void removeBus(struct Bus **head);                                // Remove a bus
void updateBus(struct Bus *head);                                 // Update bus details
void viewUsers(struct User *users, int userCount);                // View user information

void initializeBuses(struct Bus **head)
{
    *head = NULL;
}

void displayAvailableBuses(const struct Bus *head)
{
    printf("\nAvailable Buses:\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("| BusNumber |      Route      |     Departure     |    Arrival   | Available Seats  |  Ticket Price  |\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    const struct Bus *current = head;
    while (current != NULL)
    {
        usleep(100000);
        printf("|   %d    |  %s      |       %s       |     %s     |        %d        |     %.2f     |\n", current->id, current->route, current->departureTime,
               current->arrivalTime, current->availableSeats, current->ticketPrice);
        printf("------------------------------------------------------------------------------------------------------\n");
        current = current->next;
    }
}

void bookTicket(struct Bus *head, int busId, struct User *user)
{

    struct Bus *current = head;
    while (current != NULL && current->id != busId)
    {
        current = current->next;
    }
    if (current == NULL)
    {
        printf("Bus not found.\n");
        return;
    }

    int numTickets; // Variable to store the number of tickets
    printf("\nAvailable Seats in Bus %d:", current->id);
    for (int i = 0; i < MAX_SEATS; i++)
    {
        if (current->seats[i] == 1)
        {
            if (i % 2 == 0)
                printf("\t");
            if (i % 4 == 0)
                printf("\n");

            printf("%d ", i + 1);
        }
    }
    printf("\n\nOne man can purchase a maximum of 4 tickets");
    printf("\nEnter the number of tickets to book: ");
    scanf("%d", &numTickets); // Prompt the user for the number of tickets

    if (numTickets <= 4)
    {
        if (numTickets > 0 && numTickets <= current->availableSeats)   // Check if the number of tickets is valid
        {
            printf("\nTicket price per ticket: %.2f\n", current->ticketPrice);
            printf("Total payable price : %.2f\n", numTickets * current->ticketPrice);

            int paymentChoice;
            float TotalPrice = numTickets * current->ticketPrice ;

            printf("\nSelect Payment Method:\n");
            printf("1. Bcash\n");
            printf("2. Nagad\n");
            printf("3. Rocket\n");
            printf("4. Card\n");
            printf("Enter your choice: ");
            scanf("%d", &paymentChoice);

            switch (paymentChoice)
            {
            case 1:
                strcpy(user->paymentMethod, "Bcash");
                break;
            case 2:
                strcpy(user->paymentMethod, "Nagad");
                break;
            case 3:
                strcpy(user->paymentMethod, "Rocket");
                break;
            case 4:
                strcpy(user->paymentMethod, "Card");
                break;
            default:
                printf("Invalid payment method choice.\n");
                return;
            }
            printf("\n|-----------------------------------------------|\n");
            printf("|  *********      N B D   TRAVELS    *********  |\n");
            printf("|  Name              : %s                    |\n", user->username );
            printf("|  Bus ID            : %d                     |\n", current->id );
            printf("|  Deapture Time     : %s                    |\n",current->departureTime );
            printf("|  Number of Tickets : %d                        |\n", numTickets );
            printf("|  Total Price       : %.2f                  |\n",TotalPrice   );
            printf("|-----------------------------------------------|" );



            for (int i = 0; i < numTickets; i++)
            {
                int availableSeatIndex = -1;
                for (int j = 0; j < MAX_SEATS; j++)
                {
                    if (current->seats[j] == 1)
                    {
                        availableSeatIndex = j;
                        break;
                    }
                }
                if (availableSeatIndex != -1)
                {
                    current->seats[availableSeatIndex] = 0; // 0 indicates seat is booked
                    current->availableSeats--;
                    user->numTickets++;
                    user->totalPrice += current->ticketPrice;
                }
            }
        }
        else
        {
            printf("Invalid number of tickets or insufficient available seats.\n");
        }
    }

}

void cancelTicket(struct Bus *head, int busId, struct User *user)
{
    struct Bus *current = head;
    while (current != NULL && current->id != busId)
    {
        current = current->next;
    }
    if (current == NULL)
    {
        printf("Bus not found.\n");
        return;
    }

    int numTickets; // Variable to store the number of tickets to cancel
    printf("\nEnter the number of tickets to cancel: ");
    scanf("%d", &numTickets); // Prompt the user for the number of tickets to cancel

    if (numTickets <= 0 || numTickets > user->numTickets)
    {
        printf("Invalid number of tickets to cancel.\n");
        return;
    }

    for (int i = 0; i < numTickets; i++)
    {
        int bookedSeatIndex = -1;
        for (int j = 0; j < MAX_SEATS; j++)
        {
            if (current->seats[j] == 0)
            {
                bookedSeatIndex = j;
                break;
            }
        }
        if (bookedSeatIndex != -1)
        {
            current->seats[bookedSeatIndex] = 1; // 1 indicates seat is available again
            current->availableSeats++;
            user->numTickets--;
            user->totalPrice -= current->ticketPrice;
        }
    }

    printf("Tickets cancelled successfully.\n");
}

void saveBusData(const struct Bus *head)
{
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    const struct Bus *current = head;
    while (current != NULL)
    {
        fwrite(current, sizeof(struct Bus) - sizeof(struct Bus *), 1, fp);
        current = current->next;
    }
    fclose(fp);
}

void loadBusData(struct Bus **head)
{
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL)
    {
        initializeBuses(head);
        saveBusData(*head); // Save initial data to file
        return;
    }

    struct Bus *current = NULL, *prev = NULL;
    while (!feof(fp))
    {
        current = (struct Bus *)malloc(sizeof(struct Bus));
        if (fread(current, sizeof(struct Bus) - sizeof(struct Bus *), 1, fp))
        {
            current->next = NULL;
            if (*head == NULL)
            {
                *head = current;
            }
            else
            {
                prev->next = current;
            }
            prev = current;
        }
        else
        {
            free(current);
        }
    }
    fclose(fp);
}

int registerUser(struct User *users, int count)
{
    if (count >= MAX_USERS)
    {
        printf("Maximum number of users reached.\n");
        return count;
    }

    printf("Enter username: ");
    scanf("%s", users[count].username);
    printf("Enter password: ");
    scanf("%s", users[count].password);

    users[count].numTickets = 0;
    users[count].totalPrice = 0.0;

    count++;
    saveUser(users, count);
    printf("Registration successful.\n");
    return count;
}

int login(struct User *users, int count)
{
    char username[50];
    char password[50];

    printf("\nEnter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            printf("Login successful.\n");
            return i;
        }
    }

    printf("Invalid username or password.\n");
    return -1;
}

void saveUser(struct User *users, int count)
{
    FILE *fp = fopen(USER_FILE, "a+");
    if (fp == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(users, sizeof(struct User), count, fp);
    fclose(fp);
}

void adminMenu(struct Bus **head, struct User *users, int *userCount)
{
    int choice;
    do
    {
        printf("\nAdmin Menu\n");
        printf("1. Add Bus\n");
        printf("2. Remove Bus\n");
        printf("3. Update Bus Details\n");
        printf("4. View Users\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBus(head);
            break;
        case 2:
            removeBus(head);
            break;
        case 3:
            updateBus(*head);
            break;
        case 4:
            viewUsers(users, *userCount);
            break;
        case 5:
            saveBusData(*head);
            saveUser(users, *userCount);
            printf("Data saved. Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    while (choice != 5);
}

void userMenu(struct Bus *head, struct User *users, int *userCount)
{
    int choice;
    int userId = -1;
    do
    {
        printf("\nUser Menu\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. View Available Buses\n");
        printf("4. Book Ticket\n");
        printf("5. Cancel Ticket\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            *userCount = registerUser(users, *userCount);
            break;
        case 2:
            userId = login(users, *userCount);
            break;
        case 3:
            displayAvailableBuses(head);
            break;
        case 4:
            if (userId != -1)
            {
                int busId;
                printf("Enter bus ID to book ticket: ");
                scanf("%d", &busId);
                bookTicket(head, busId, &users[userId]);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 5:
            if (userId != -1)
            {
                int busId;
                printf("Enter bus ID to cancel ticket: ");
                scanf("%d", &busId);
                cancelTicket(head, busId, &users[userId]);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 6:
            saveBusData(head);
            saveUser(users, *userCount);
            printf("Data saved. Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    while (choice != 6);
}

void addBus(struct Bus **head)
{
    struct Bus *newBus = (struct Bus *)malloc(sizeof(struct Bus));
    if (newBus == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter bus ID: ");
    scanf("%d", &newBus->id);
    printf("Enter route: ");
    scanf("%s", newBus->route);
    printf("Enter departure time: ");
    scanf("%s", newBus->departureTime);
    printf("Enter arrival time: ");
    scanf("%s", newBus->arrivalTime);
    printf("Enter ticket price: ");
    scanf("%f", &newBus->ticketPrice);
    newBus->availableSeats = MAX_SEATS;
    for (int i = 0; i < MAX_SEATS; i++)
    {
        newBus->seats[i] = 1; // All seats are initially available
    }
    newBus->next = NULL;

    if (*head == NULL)
    {
        *head = newBus;
    }
    else
    {
        struct Bus *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newBus;
    }
    printf("Bus added successfully.\n");
}

void removeBus(struct Bus **head)
{
    if (*head == NULL)
    {
        printf("No buses to remove.\n");
        return;
    }

    int busId;
    printf("Enter bus Number to remove: ");
    scanf("%d", &busId);

    struct Bus *current = *head;
    struct Bus *previous = NULL;

    while (current != NULL && current->id != busId)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Bus not found.\n");
        return;
    }

    if (previous == NULL)
    {
        *head = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    free(current);
    printf("Bus removed successfully.\n");
}

void updateBus(struct Bus *head)
{
    int busId;
    printf("Enter bus ID to update: ");
    scanf("%d", &busId);

    struct Bus *current = head;
    while (current != NULL && current->id != busId)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Bus not found.\n");
        return;
    }

    printf("Enter new route: ");
    scanf("%s", current->route);
    printf("Enter new departure time: ");
    scanf("%s", current->departureTime);
    printf("Enter new arrival time: ");
    scanf("%s", current->arrivalTime);
    printf("Enter new ticket price: ");
    scanf("%f", &current->ticketPrice);

    printf("Bus details updated successfully.\n");
}

void viewUsers(struct User *users, int userCount)
{
    printf("\nRegistered Users:\n");
    for (int i = 0; i < userCount; i++)
    {
        printf("Username: %s, Number of Tickets: %d, Total Price: %.2f\n",
               users[i].username, users[i].numTickets, users[i].totalPrice);
    }
}

void loadingPage()
{
    system("color B");
    int i;
    printf("\n ");

    // Forward animation
    for (i = 0; i < LOADING_CYCLES; i++)
    {
        printf("    ");
        fflush(stdout); // Flush the output buffer to ensure immediate display
        usleep(LOADING_DELAY); // Delay for a short duration
    }


    printf("\n                           \n");
    printf("                             __   ___ ______  _____           ____________  ___        =      _          _========  ||       ||-----\\   \n");
    usleep(50000);
    printf("                             |\\   | | ||  \\\\  ||  \\\\               ||     ||  ))     // \\\\    \\\\        //||        ||       ||     \n");
    usleep(50000);
    printf("                             | \\  | | ||__//  ||   \\\\              ||     ||   ||   //   \\\\    \\\\      // ||        ||       ||   \n" );
    usleep(50000);
    printf("                             | |\\ | | ||--\\\\  ||   ||              ||     ||__//   //=====\\\\    \\\\    //  ||======  ||       ||=====|   \n");
    usleep(50000);
    printf("                             | | \\| | ||__//  ||   //              ||     ||  \\\\  //       \\\\    \\\\  //   ||        ||             ||  \n");
    usleep(50000);
    printf("                             |_|  \\_| |___/   ||__//               ||     ||   \\\\//         \\\\    \\\\//    |_______  |======= \\_____||  \n");


}

int main()
{

    loadingPage();
    struct Bus *busList;
    struct User users[MAX_USERS];
    int userCount = 0;

    initializeBuses(&busList);
    loadBusData(&busList);
    //loadUserData(users, &userCount); // Load user data from file

    int mainChoice;
    do
    {
        printf("\nMain Menu\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);

        switch (mainChoice)
        {
        case 1:
        {
            int adminChoice;
            printf("\nAdmin Login\n");
            printf("Enter username: ");
            char adminUsername[50];
            scanf("%s", adminUsername);
            printf("Enter password: ");
            char adminPassword[50];
            scanf("%s", adminPassword);

            if (strcmp(adminUsername, "admin") == 0 && strcmp(adminPassword, "nuhin23") == 0)
            {
                adminMenu(&busList, users, &userCount);
            }
            else
            {
                printf("Invalid admin credentials.\n");
            }
        }
        break;
        case 2:
            userMenu(busList, users, &userCount);
            break;
        case 3:
            saveBusData(busList);
            saveUser(users, userCount);
            printf("Data saved. Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    while (mainChoice != 3);

    return 0;
}


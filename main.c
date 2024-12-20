#include "pharmacy.h"
#include <conio.h>
#include <windows.h>

void clearScreen() {
    system("cls");
}

void displayHeader() {
    printf("\n+==================================+\n");
    printf("|     Pharmacy Management System     |\n");
    printf("+==================================+\n");
}

void displayMenu() {
    printf("\n+-------------- MENU --------------+\n");
    printf("|  1. Add Medicine                 |\n");
    printf("|  2. Delete Medicine              |\n");
    printf("|  3. View Medicines               |\n");
    printf("|  4. Sell Medicine                |\n");
    printf("|  5. Generate Weekly Report       |\n");
    printf("|  6. Generate Monthly Report      |\n");
    printf("|  7. Search Medicine              |\n");
    printf("|  8. Exit                         |\n");
    printf("+--------------------------------+\n");
    printf("\nEnter your choice [1-8]: ");
}

void displayFormHeader(const char* title) {
    clearScreen();
    displayHeader();
    printf("\n+--- %s ---+\n", title);
}

int main() {
    int choice;
    
    // Load data and user credentials
    loadData();
    loadUserCredentials();
    
    // Attempt login
    clearScreen();
    if (!loginUser()) {
        printf("\nExiting system due to failed login attempts...\n");
        return 1;
    }

    while (1) {
        clearScreen();
        displayHeader();
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayFormHeader("Add Medicine");
                addMedicine();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 2:
                displayFormHeader("Delete Medicine");
                deleteMedicine();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 3:
                displayFormHeader("View Medicines");
                viewMedicine();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 4:
                displayFormHeader("Sell Medicine");
                sellMedicine();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 5:
                displayFormHeader("Weekly Report");
                generateWeeklyReport();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 6:
                displayFormHeader("Monthly Report");
                generateMonthlyReport();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 7:
                displayFormHeader("Search Medicine");
                searchMedicine();
                printf("\nPress any key to continue...");
                _getch();
                break;
            case 8:
                clearScreen();
                displayHeader();
                printf("\n    Thank you for using our system!\n");
                printf("\nPress any key to exit...");
                _getch();
                return 0;
            default:
                printf("\n Invalid choice! Please try again...");
                Sleep(1500);
        }
    }
    return 0;
}
#include "pharmacy.h"
#include <ctype.h>
#include <conio.h>
#include <windows.h>

Medicine medicines[1000];
Sale sales[5000];
User currentUser;
int medicine_count = 0;
int sales_count = 0;

// Default credentials - in a real system, these would be stored securely
const char DEFAULT_USERNAME[] = "manager";
const char DEFAULT_PASSWORD[] = "pharmacy123";

void saveUserCredentials() {
    FILE *f = fopen("user.dat", "wb");
    if (f) {
        fwrite(&currentUser, sizeof(User), 1, f);
        fclose(f);
    }
}

void loadUserCredentials() {
    FILE *f = fopen("user.dat", "rb");
    if (f) {
        fread(&currentUser, sizeof(User), 1, f);
        fclose(f);
    } else {
        // If no user file exists, create default credentials
        strcpy(currentUser.username, DEFAULT_USERNAME);
        strcpy(currentUser.password, DEFAULT_PASSWORD);
        saveUserCredentials();
    }
}

int loginUser() {
    char username[50];
    char password[50];
    int attempts = 3; // Allow 3 login attempts

    while (attempts > 0) {
        printf("+================================+\n");
        printf("|           Login Page           |\n");
        printf("+================================+\n\n");
        
        printf("Username: ");
        scanf(" %[^\n]s", username);
        printf("Password: ");
        scanf(" %[^\n]s", password);

        // Check credentials
        if (strcmp(username, currentUser.username) == 0 && 
            strcmp(password, currentUser.password) == 0) {
            printf("\nLogin successful! Welcome, %s!\n", username);
            Sleep(1500);
            return 1; // Login successful
        } else {
            attempts--;
            printf("\nInvalid username or password!");
            if (attempts > 0) {
                printf(" %d attempts remaining.\n", attempts);
                printf("Press any key to try again...\n");
                _getch();
                clearScreen();
            } else {
                printf("\nToo many failed attempts. Please try again later.\n");
                Sleep(1500);
                return 0; // Login failed
            }
        }
    }
    return 0; // Login failed
}

void addMedicine() {
    Medicine med;
    printf("+================================+\n");
    printf("|        Add New Medicine        |\n");
    printf("+================================+\n\n");
    
    printf("+-------------------------------+\n");
    printf("| Medicine ID: ");
    scanf("%d", &med.id);
    printf("| Name: ");
    scanf(" %[^\n]s", med.name);
    printf("| Price: $");
    scanf("%f", &med.price);
    printf("| Quantity: ");
    scanf("%d", &med.quantity);
    printf("| Expiry Date (YYYY-MM-DD): ");
    scanf(" %[^\n]s", med.expiry_date);
    printf("+-------------------------------+\n");

    medicines[medicine_count++] = med;
    printf("\n* Medicine added successfully!\n");
    saveData();
}

void viewMedicine() {
    printf("+================================================+\n");
    printf("|               Medicine Inventory                 |\n");
    printf("+================================================+\n\n");
    
    printf("+--------+----------------+----------+----------+-----------+\n");
    printf("|   ID   |      Name     |  Price   | Quantity |Expiry Date|\n");
    printf("+--------+----------------+----------+----------+-----------+\n");
    
    for (int i = 0; i < medicine_count; i++) {
        printf("| %-6d | %-14s | $%-8.2f | %-8d | %-9s |\n",
            medicines[i].id,
            medicines[i].name,
            medicines[i].price,
            medicines[i].quantity,
            medicines[i].expiry_date);
    }
    printf("+--------+----------------+----------+----------+-----------+\n");
}

void deleteMedicine() {
    int id, found = 0;
    printf("+================================+\n");
    printf("|        Delete Medicine         |\n");
    printf("+================================+\n\n");
    
    printf("Enter Medicine ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < medicine_count; i++) {
        if (medicines[i].id == id) {
            for (int j = i; j < medicine_count - 1; j++) {
                medicines[j] = medicines[j + 1];
            }
            medicine_count--;
            found = 1;
            printf("\n* Medicine deleted successfully!\n");
            saveData();
            break;
        }
    }
    if (!found) printf("\n! Medicine not found!\n");
}

void sellMedicine() {
    int id, qty, found = 0;
    printf("+================================+\n");
    printf("|         Sell Medicine          |\n");
    printf("+================================+\n\n");
    
    printf("+-------------------------------+\n");
    printf("| Medicine ID: ");
    scanf("%d", &id);
    printf("| Quantity: ");
    scanf("%d", &qty);
    printf("+-------------------------------+\n");

    for (int i = 0; i < medicine_count; i++) {
        if (medicines[i].id == id) {
            if (medicines[i].quantity >= qty) {
                Sale sale;
                sale.medicine_id = id;
                sale.quantity = qty;
                sale.total_price = qty * medicines[i].price;
                
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                strftime(sale.date, sizeof(sale.date), "%Y-%m-%d", tm);

                medicines[i].quantity -= qty;
                sales[sales_count++] = sale;
                
                printf("\n* Sale completed!\n");
                printf("+-------------------------------+\n");
                printf("| Total Price: $%.2f\n", sale.total_price);
                printf("+-------------------------------+\n");
                saveData();
                found = 1;
                break;
            } else {
                printf("\n! Insufficient quantity in stock!\n");
                found = 1;
                break;
            }
        }
    }
    if (!found) printf("\n! Medicine not found!\n");
}

void generateWeeklyReport() {
    float total_sales = 0;
    int total_quantity = 0;
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char current_date[11];
    strftime(current_date, sizeof(current_date), "%Y-%m-%d", tm);

    printf("+================================================+\n");
    printf("|               Weekly Sales Report                |\n");
    printf("+================================================+\n\n");
    
    printf("+--------+----------+------------+------------+\n");
    printf("| Med ID | Quantity |Total Price |    Date    |\n");
    printf("+--------+----------+------------+------------+\n");

    for (int i = 0; i < sales_count; i++) {
        // Simple check for sales within last 7 days
        if (strcmp(sales[i].date, current_date) - 7 <= 0) {
            printf("| %-6d | %-8d | $%-9.2f | %-10s |\n",
                sales[i].medicine_id,
                sales[i].quantity,
                sales[i].total_price,
                sales[i].date);
            total_sales += sales[i].total_price;
            total_quantity += sales[i].quantity;
        }
    }
    printf("+--------+----------+------------+------------+\n");
    printf("+----------------------------------------+\n");
    printf("| Total Weekly Sales: $%-18.2f |\n", total_sales);
    printf("| Total Quantity Sold: %-19d |\n", total_quantity);
    printf("+----------------------------------------+\n");
}

void generateMonthlyReport() {
    float total_sales = 0;
    int total_quantity = 0;
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char current_month[8];
    strftime(current_month, sizeof(current_month), "%Y-%m", tm);

    printf("+================================================+\n");
    printf("|               Monthly Sales Report               |\n");
    printf("+================================================+\n\n");
    
    printf("+--------+----------+------------+------------+\n");
    printf("| Med ID | Quantity |Total Price |    Date    |\n");
    printf("+--------+----------+------------+------------+\n");

    for (int i = 0; i < sales_count; i++) {
        char sale_month[8];
        strncpy(sale_month, sales[i].date, 7);
        sale_month[7] = '\0';

        if (strcmp(sale_month, current_month) == 0) {
            printf("| %-6d | %-8d | $%-9.2f | %-10s |\n",
                sales[i].medicine_id,
                sales[i].quantity,
                sales[i].total_price,
                sales[i].date);
            total_sales += sales[i].total_price;
            total_quantity += sales[i].quantity;
        }
    }
    printf("+--------+----------+------------+------------+\n");
    printf("+----------------------------------------+\n");
    printf("| Total Monthly Sales: $%-17.2f |\n", total_sales);
    printf("| Total Quantity Sold: %-19d |\n", total_quantity);
    printf("+----------------------------------------+\n");
}

void searchMedicine() {
    int choice, found = 0;
    char searchTerm[50];
    int searchId;

    printf("+================================+\n");
    printf("|        Search Medicine         |\n");
    printf("+================================+\n\n");
    
    printf("Search by:\n");
    printf("1. Medicine ID\n");
    printf("2. Medicine Name\n");
    printf("Enter your choice (1-2): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("\nEnter Medicine ID: ");
            scanf("%d", &searchId);
            
            printf("\n+--------+----------------+----------+----------+-----------+\n");
            printf("|   ID   |      Name     |  Price   | Quantity |Expiry Date|\n");
            printf("+--------+----------------+----------+----------+-----------+\n");
            
            for(int i = 0; i < medicine_count; i++) {
                if(medicines[i].id == searchId) {
                    printf("| %-6d | %-14s | $%-8.2f | %-8d | %-9s |\n",
                        medicines[i].id,
                        medicines[i].name,
                        medicines[i].price,
                        medicines[i].quantity,
                        medicines[i].expiry_date);
                    found = 1;
                }
            }
            printf("+--------+----------------+----------+----------+-----------+\n");
            break;
            
        case 2:
            printf("\nEnter Medicine Name (or part of name): ");
            scanf(" %[^\n]s", searchTerm);
            
            printf("\n+--------+----------------+----------+----------+-----------+\n");
            printf("|   ID   |      Name     |  Price   | Quantity |Expiry Date|\n");
            printf("+--------+----------------+----------+----------+-----------+\n");
            
            for(int i = 0; i < medicine_count; i++) {
                // Case-insensitive search
                char medicineName[50], searchTermLower[50];
                strcpy(medicineName, medicines[i].name);
                strcpy(searchTermLower, searchTerm);
                
                // Convert both strings to lowercase for comparison
                for(int j = 0; medicineName[j]; j++) {
                    medicineName[j] = tolower(medicineName[j]);
                }
                for(int j = 0; searchTermLower[j]; j++) {
                    searchTermLower[j] = tolower(searchTermLower[j]);
                }
                
                if(strstr(medicineName, searchTermLower) != NULL) {
                    printf("| %-6d | %-14s | $%-8.2f | %-8d | %-9s |\n",
                        medicines[i].id,
                        medicines[i].name,
                        medicines[i].price,
                        medicines[i].quantity,
                        medicines[i].expiry_date);
                    found = 1;
                }
            }
            printf("+--------+----------------+----------+----------+-----------+\n");
            break;
            
        default:
            printf("\nInvalid choice!\n");
            return;
    }
    
    if (!found) {
        printf("\nNo medicines found matching your search criteria!\n");
    }
}

void saveData() {
    FILE *f = fopen("medicines.dat", "wb");
    if (f) {
        fwrite(&medicine_count, sizeof(int), 1, f);
        fwrite(medicines, sizeof(Medicine), medicine_count, f);
        fclose(f);
    }

    f = fopen("sales.dat", "wb");
    if (f) {
        fwrite(&sales_count, sizeof(int), 1, f);
        fwrite(sales, sizeof(Sale), sales_count, f);
        fclose(f);
    }
}

void loadData() {
    FILE *f = fopen("medicines.dat", "rb");
    if (f) {
        fread(&medicine_count, sizeof(int), 1, f);
        fread(medicines, sizeof(Medicine), medicine_count, f);
        fclose(f);
    }

    f = fopen("sales.dat", "rb");
    if (f) {
        fread(&sales_count, sizeof(int), 1, f);
        fread(sales, sizeof(Sale), sales_count, f);
        fclose(f);
    }
}

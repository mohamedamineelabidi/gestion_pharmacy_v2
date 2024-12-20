#ifndef PHARMACY_H
#define PHARMACY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure for medicine
typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
    char expiry_date[11];
} Medicine;

// Structure for user credentials
typedef struct {
    char username[50];
    char password[50];
} User;

// Structure for sales
typedef struct {
    int medicine_id;
    int quantity;
    float total_price;
    char date[11];
} Sale;

// Function prototypes
void clearScreen();
void addMedicine();
void deleteMedicine();
void viewMedicine();
void sellMedicine();
void generateWeeklyReport();
void generateMonthlyReport();
void searchMedicine();
void saveData();
void loadData();
int loginUser();
void saveUserCredentials();
void loadUserCredentials();

#endif

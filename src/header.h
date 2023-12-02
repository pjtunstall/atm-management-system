#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date lastChanged;
    double interest;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[100], char pass[50]);
// void registerMenu(char a[100], char pass[50]);
const char *getPassword(struct User u);
struct User checkCredentials(char *username, char *password);

// system function
void createNewAccount(struct User u);
void mainMenu(struct User u);
void checkAccount(struct User u);
void checkAllAccounts(struct User u);
void removeAccount(struct User u);
void makeTransaction(struct User u);
void update(struct User u);
void transfer(struct User u);
#include "header.h"

void createNewUser(struct User *u);

void mainMenu(struct User u)
{
    int option;
    system("clear");
haveAnotherGo:
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below. <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- View a particular account\n");
    printf("\n\t\t[4]- View all your accounts\n");
    printf("\n\t\t[5]- Make a transaction\n");
    printf("\n\t\t[6]- Remove an existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");

    if (scanf("%d", &option) != 1)
    {
        printf("\n✖ Insert a valid operation, dammit!\n");
        // Clear the input buffer
        while (getchar() != '\n')
            ;
        goto haveAnotherGo;
    }

    switch (option)
    {
    case 1:
        createNewAccount(u);
        break;
    case 2:
        update(u);
        break;
    case 3:
        checkAccount(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transfer(u);
        break;
    case 8:
        printf("\n");
        exit(0);
        break;
    default:
        printf("✖ Insert a valid operation, dammit!\n");
        goto haveAnotherGo;
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login or register; exit at your peril! <<--\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
    invalid:
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("\n✖ Insert a valid operation, dammit!\n");
            goto invalid;
        }
        struct User ru;
        char username[100];
        char password[50];
        switch (option)
        {
        case 1:
            loginMenu(username, password);
            *u = checkCredentials(username, password);
            if (u->id == -1)
            {
                printf("\n\n✖ Wrong username or password! Scram, imposter! The police have been notified.\n\n");
                exit(0);
            }
            else
            {
                printf("\n\n✔ Password Match!");
            }
            r = 1;
            break;
        case 2:
            createNewUser(u);
            r = 1;
            break;
        case 3:
            printf("\n");
            exit(0);
            break;
        default:
            printf("✖ Insert a valid operation, dammit!\n");
            goto invalid;
        }
    }
};

int main()
{
    struct User u;

    initMenu(&u);
    mainMenu(u);
    return 0;
}

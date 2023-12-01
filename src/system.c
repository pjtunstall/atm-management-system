#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[100], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        printf("\n\n✖ Record not found!\n");
    invalid:
        printf("\nEnter 1 to try again, 2 to return to main menu, or 3 to exit.\n");
        if (scanf("%d", &option) != 1)
        {
            printf("\n✖ Pick a number that you see;\n");
            printf("the only options: 1, 2, 3.\n");
            // Clear the input buffer
            while (getchar() != '\n')
                ;
            goto invalid;
        }
        if (option == 1)
            f(u);
        else if (option == 2)
            mainMenu(u);
        else if (option == 3)
        {
            printf("\n");
            exit(0);
        }
        else
        {
            printf("✖ Insert a valid operation, dammit!\n");
            goto invalid;
        }
    }
    else
    {
    chooseAgain:
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        if (scanf("%d", &option) != 1)
        {
            printf("\n✖ Invalid operation!\n");
            while (getchar() != '\n')
                ;
            goto chooseAgain;
        }
    }
    if (option == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf("\n");
        exit(0);
    }
}

void success(struct User u)
{
    int option;
invalid:
    printf("Enter 1 to go to the main menu or 0 to exit.\n");
    if (scanf("%d", &option) != 1)
    {
        printf("\n✖ Invalid operation!\n");
        while (getchar() != '\n')
            ;
        goto invalid;
    }
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        printf("\n");
        exit(0);
    }
    else
    {
        printf("✖ Insert a valid operation, dammit!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    int option;
    struct Record r;
    struct Record cr;
    char userName[100];
    FILE *pf = fopen(RECORDS, "a+");
    int i = 0;

noAccount:
    system("clear");
    printf("\t\t\t===== New Record =====\n");

getDate:
    printf("\nEnter today's date(mm/dd/yyyy): ");
    if (fscanf(stdin, "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3)
    {
        printf("✖ Invalid date!\n");
        while (getchar() != '\n')
            ;
        goto getDate;
    }

getAccountNumber:
    printf("\nEnter the account number: ");
    if (scanf("%d", &r.accountNbr) != 1)
    {
        printf("✖ Invalid account number!\n");
        while (getchar() != '\n')
            ;
        goto getAccountNumber;
    }

    rewind(pf);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
                  &cr.id,
                  &cr.userId,
                  cr.name,
                  &cr.accountNbr,
                  &cr.deposit.month,
                  &cr.deposit.day,
                  &cr.deposit.year,
                  cr.country,
                  &cr.phone,
                  &cr.amount,
                  cr.accountType) != EOF)
    {
        i++;
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists.\n");
        invalid:
            printf("\nEnter 1 to try again, 2 to return to main menu, or 3 to exit.\n");
            if (scanf("%d", &option) != 1)
            {
                printf("\n✖ Invalid operation!\n");
                while (getchar() != '\n')
                    ;
                goto invalid;
            }
            if (option == 1)
                createNewAcc(u);
            else if (option == 2)
                mainMenu(u);
            else if (option == 3)
            {
                printf("\n");
                exit(0);
            }
            else
            {
                printf("✖ Insert a valid operation, dammit!\n");
                goto invalid;
            }
        }
    }

    printf("\nEnter the country: ");
    scanf("%s", r.country);
enterPhoneNumber:
    printf("\nEnter your phone number: ");
    if (scanf("%d", &r.phone) != 1)
    {
        printf("✖ Invalid phone number!\n");
        while (getchar() != '\n')
            ;
        goto enterPhoneNumber;
    }
enterAmount:
    printf("\nEnter amount to deposit: $");
    if (scanf("%lf", &r.amount) != 1)
    {
        printf("✖ Invalid amount!\n");
        while (getchar() != '\n')
            ;
        goto enterAmount;
    }
chooseAccountType:
    printf("\nChoose the type of account:\n\n\t\t[1]- fixed01(for 1 year)\n\t\t[2]- fixed02(for 2 years)\n\t\t[3]- fixed03(for 3 years)\n\t\t[4]- saving\n\t\t[5]- current\n\nEnter your choice: ");
    if (scanf("%d", &option) != 1)
    {
        printf("\n✖ Invalid operation, dammit!\n");
        while (getchar() != '\n')
            ;
        goto chooseAccountType;
    }
    switch (option)
    {
    case 1:
        strcpy(r.accountType, "fixed01");
        break;
    case 2:
        strcpy(r.accountType, "fixed02");
        break;
    case 3:
        strcpy(r.accountType, "fixed03");
        break;
    case 4:
        strcpy(r.accountType, "saving");
        break;
    case 5:
        strcpy(r.accountType, "current");
        break;
    default:
        printf("✖ Insert a valid operation, dammit!\n");
        goto chooseAccountType;
    }

    r.id = i;
    saveAccountToFile(pf, u, r);

    fclose(pf);
    printf("\n✔ Account created successfully!\n");
    success(u);
}

void checkInterest(struct Date dateFrom, double rate, double amount)
{
    struct Date dateTo;

checkAgain:
    printf("\nEnter the date you want to check the interest for(mm/dd/yyyy): ");
    if (fscanf(stdin, "%d/%d/%d", &dateTo.month, &dateTo.day, &dateTo.year) != 3)
    {
        printf("✖ Invalid date!\n");
        while (getchar() != '\n')
            ;
        goto checkAgain;
    }

    int years = dateTo.year - dateFrom.year;
    int months = dateTo.month - dateFrom.month;
    int days = dateTo.day - dateFrom.day;

    if (years < 0 || (years == 0 && months < 0) || (years == 0 && months == 0 && days < 0))
    {
        printf("✖ Invalid date range!\n");
        goto checkAgain;
    }

    double yearly = rate * amount;
    double monthly = rate * amount / 12;

    double compound = years * yearly;
    compound += months * monthly;
    if (days < 0)
    {
        compound -= monthly;
    }

    printf("\nBy %d/%d/%d will have gained $%.2lf in interest.\n", dateTo.month, dateTo.day, dateTo.year, compound);
    return;
}

void checkAccount(struct User u)
{
    char userName[100];
    struct Record r;
    int accountNbr = 0;
    int found = 0;
    double rate = 0;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== Check Accounts =====\n\n");
    printf("\nEnter account number: ");
    scanf("%d", &accountNbr);

    while (getAccountFromFile(pf, userName, &r))
    {
        if ((strcmp(userName, u.name) == 0) && (r.accountNbr == accountNbr))
        {
            found = 1;
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.month,
                   r.deposit.day,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            if (strcmp(r.accountType, "current") == 0)
            {
                printf("You will not get interests [sic] because the account is of type current.\n");
            }
            else
            {
                if (strcmp(r.accountType, "saving") == 0)
                {
                    rate = 0.07;
                }
                else if (strcmp(r.accountType, "fixed01") == 0)
                {
                    rate = 0.04;
                }
                else if (strcmp(r.accountType, "fixed02") == 0)
                {
                    rate = 0.05;
                }
                else if (strcmp(r.accountType, "fixed03") == 0)
                {
                    rate = 0.08;
                }
                printf("Interest rate: %.2lf%%\n", rate * 100);
                printf("\nYou will get $%.2lf as interest on day %d of every month.\n",
                       (r.amount * rate / 12),
                       r.deposit.day);
                printf("\nDo you want to check how much interest you will have gained at a specific date? (y/n): ");
                char answer;
                scanf(" %c", &answer);
                if (answer == 'y' || answer == 'Y')
                {
                    checkInterest(r.deposit, rate, r.amount);
                }
                else if (answer != 'n' && answer != 'N')
                {
                    printf("\n✖ Invalid answer!\n");
                }
                while (getchar() != '\n')
                    ;
            }
        }
    }
    fclose(pf);
    if (found == 0)
    {
        stayOrReturn(found, checkAccount, u);
    }
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;
    int found = 0;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All Your Accounts, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            found = 1;
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.month,
                   r.deposit.day,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    if (found == 0)
    {
        printf("\n✖ You have no accounts! Why not mosey on down to the main menu and make one?\n");
    }
    success(u);
}

void makeTransaction(struct User u)
{
    char userName[100];
    struct Record records[50];
    int accountNbr = -1;
    int found = 0;
    int option;
    double amount;
    int numRecords = 0;
    int accountToAmend = -1;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== Make a Transaction =====\n\n");
getAccountNumber:
    printf("\nEnter account number: ");
    if (scanf("%d", &accountNbr) != 1)
    {
        printf("\n✖ Invalid account number!\n");
        while (getchar() != '\n')
            ;
        goto getAccountNumber;
    }

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
                  &records[numRecords].id,
                  &records[numRecords].userId,
                  records[numRecords].name,
                  &records[numRecords].accountNbr,
                  &records[numRecords].deposit.month,
                  &records[numRecords].deposit.day,
                  &records[numRecords].deposit.year,
                  records[numRecords].country,
                  &records[numRecords].phone,
                  &records[numRecords].amount,
                  records[numRecords].accountType) != EOF)
    {
        if (strcmp(records[numRecords].name, u.name) == 0 && records[numRecords].accountNbr == accountNbr)
        {
            found = 1;
            accountToAmend = numRecords;
        }
        numRecords++;
    }

    fclose(pf);

    if (strcmp(records[accountToAmend].accountType, "fixed01") == 0 ||
        strcmp(records[accountToAmend].accountType, "fixed02") == 0 ||
        strcmp(records[accountToAmend].accountType, "fixed03") == 0)
    {
        printf("\n✖ Dude, that's a fixed account!");
    fixed:
        printf("\nEnter 1 to try again, 2 to return to main menu, or 3 to exit.\n");
        if (scanf("%d", &option) != 1)
        {
            printf("\n✖ Invalid operation!\n");
            // Clear the input buffer
            while (getchar() != '\n')
                ;
            goto fixed;
        }
        if (option == 1)
            makeTransaction(u);
        else if (option == 2)
            mainMenu(u);
        else if (option == 3)
        {
            printf("\n");
            exit(0);
        }
        else
        {
            printf("Insert a valid operation, dammit!\n");
            goto fixed;
        }
    }

    if (found == 0)
    {
        stayOrReturn(found, makeTransaction, u);
    }

invalid:
    printf("\n\nEnter 1 to deposit, 2 to withdraw, or 3 for main menu: ");
    if (scanf("%d", &option) != 1)
    {
        printf("\n✖ Invalid operation!\n");
        // Clear the input buffer
        while (getchar() != '\n')
            ;
        goto invalid;
    }
    if (option == 1)
    {
        printf("\nEnter amount to deposit: $");
        scanf("%lf", &amount);
        records[accountToAmend].amount += amount;
        printf("\n✔ Deposited successfully!");
        printf("\nYour new balance is: $%.2lf\n", records[accountToAmend].amount);
    }
    else if (option == 2)
    {
        printf("\nEnter amount to withdraw: $");
        scanf("%lf", &amount);
        if (amount > records[accountToAmend].amount)
        {
            printf("\nInsufficient balance!");
        }
        else
        {
            records[accountToAmend].amount -= amount;
            printf("\n✔ Withdrawn successfully!");
            printf("\nYour new balance is: $%.2lf\n", records[accountToAmend].amount);
        }
    }
    else if (option == 3)
    {
        mainMenu(u);
    }
    else
    {
        printf("✖ Insert a valid operation, dammit!\n");
        goto invalid;
    }

    FILE *tempPtr;
    tempPtr = fopen("temp.txt", "w");
    if (tempPtr == NULL)
    {
        printf("✖ Error opening temporary file.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        fprintf(tempPtr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                records[i].id,
                records[i].userId,
                records[i].name,
                records[i].accountNbr,
                records[i].deposit.month,
                records[i].deposit.day,
                records[i].deposit.year,
                records[i].country,
                records[i].phone,
                records[i].amount,
                records[i].accountType);
    }

    fclose(tempPtr);
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    success(u);
}

void removeAccount(struct User u)
{
    char userName[100];
    struct Record r;
    int accountNbr;
    int accountFound = 0;
    int recordToDelete = -1;

    system("clear");
    printf("\t\t====== Remove Account =====\n\n");
getAccNbr:
    printf("Enter the number of the account you wish to delete: ");
    if (scanf("%d", &accountNbr) != 1)
    {
        printf("\n✖ Invalid account number!\n");
        while (getchar() != '\n')
            ;
        goto getAccNbr;
    }

    struct Record records[50];
    int numRecords = 0;

    // Open the file in read mode
    FILE *ptr = fopen(RECORDS, "r");
    if (ptr == NULL)
    {
        printf("✖ Error opening file.\n");
        return;
    }

    while (fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
                  &records[numRecords].id,
                  &records[numRecords].userId,
                  records[numRecords].name,
                  &records[numRecords].accountNbr,
                  &records[numRecords].deposit.month,
                  &records[numRecords].deposit.day,
                  &records[numRecords].deposit.year,
                  records[numRecords].country,
                  &records[numRecords].phone,
                  &records[numRecords].amount,
                  records[numRecords].accountType) != EOF)
    {
        if (strcmp(records[numRecords].name, u.name) == 0 && records[numRecords].accountNbr == accountNbr)
        {
            accountFound = 1;
            r = records[numRecords];
            recordToDelete = numRecords;
            printf("\nYou have asked to delete the following account:\n\n");
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.month,
                   r.deposit.day,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
        else
        {
            numRecords++;
        }
    }

    fclose(ptr);

    if (accountFound == 0)
    {
        stayOrReturn(accountFound, removeAccount, u);
    }

    printf("\nAre you sure you want to delete this account? (y/n): ");
    char answer;
    scanf(" %c", &answer);
    if (answer != 'y' && answer != 'Y')
    {
        printf("Account not deleted.\n");
        success(u);
    }

    printf("\nOk, then. On your own head be it. Deleting account...\n");

    FILE *tempPtr;
    tempPtr = fopen("temp.txt", "w");
    if (tempPtr == NULL)
    {
        printf("✖ Error opening temporary file.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        if (strcmp(r.name, u.name) != 0 || r.accountNbr != accountNbr)
        {
            fprintf(tempPtr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    records[i].id,
                    records[i].userId,
                    records[i].name,
                    records[i].accountNbr,
                    records[i].deposit.month,
                    records[i].deposit.day,
                    records[i].deposit.year,
                    records[i].country,
                    records[i].phone,
                    records[i].amount,
                    records[i].accountType);
        }
    }

    fclose(tempPtr);
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    printf("\n✔ Account deleted successfully!\n");
    success(u);
}

void update(struct User u)
{
    struct Record r;
    int accountNbr;
    int found = 0;
    struct Record records[50];
    int numRecords = 0;
    int accountToUpdate = 0;

    system("clear");
    printf("\t\t====== Update Account =====\n\n");
    printf("Enter the number of the account you wish to update: ");
    scanf("%d", &accountNbr);

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("✖ Error opening file.\n");
        return;
    }

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
                  &records[numRecords].id,
                  &records[numRecords].userId,
                  records[numRecords].name,
                  &records[numRecords].accountNbr,
                  &records[numRecords].deposit.month,
                  &records[numRecords].deposit.day,
                  &records[numRecords].deposit.year,
                  records[numRecords].country,
                  &records[numRecords].phone,
                  &records[numRecords].amount,
                  records[numRecords].accountType) != EOF)
    {
        if (strcmp(records[numRecords].name, u.name) == 0 && records[numRecords].accountNbr == accountNbr)
        {
            r = records[numRecords];
            accountToUpdate = numRecords;
            found = 1;
        }
        numRecords++;
    }

    if (found == 0)
    {
        stayOrReturn(0, update, u);
    }

    printf("\nYou have asked to update the following account:\n\n");
    printf("_____________________\n");
    printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
           r.accountNbr,
           r.deposit.month,
           r.deposit.day,
           r.deposit.year,
           r.country,
           r.phone,
           r.amount,
           r.accountType);

    int option;
updateOption:
    printf("\nWhat would you like to update?\n");
    printf("\n\t\t[1]- Country\n");
    printf("\n\t\t[2]- Phone number\n");
    printf("\n\t\t[3]- Return to main menu\n");
    if (scanf("%d", &option) != 1)
    {
        printf("\n✖ Insert a valid option, dammit!\n");
        while (getchar() != '\n')
            ;
        goto updateOption;
    }

    switch (option)
    {
    case 1:
        printf("\nEnter new country: ");
        scanf("%s", r.country);
        break;
    case 2:
        printf("\nEnter new phone number: ");
        scanf("%d", &r.phone);
        break;
    case 3:
        mainMenu(u);
        break;
    default:
        printf("✖ Enter a number 1, 2, 3;\n");
        printf("these are the options, as you see.\n");
        goto updateOption;
    }

    fclose(pf);

    FILE *tempPtr;
    tempPtr = fopen("temp.txt", "w");
    if (tempPtr == NULL)
    {
        printf("✖ Error opening temporary file.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        if (i == accountToUpdate)
        {
            fprintf(tempPtr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    r.id,
                    r.userId,
                    r.name,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
        }
        else
        {
            fprintf(tempPtr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    records[i].id,
                    records[i].userId,
                    records[i].name,
                    records[i].accountNbr,
                    records[i].deposit.month,
                    records[i].deposit.day,
                    records[i].deposit.year,
                    records[i].country,
                    records[i].phone,
                    records[i].amount,
                    records[i].accountType);
        }
    }

    fclose(tempPtr);
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    printf("\n✔ Account updated successfully!\n");
    success(u);
}

void transfer(struct User u)
{
    struct Record r;
    int accountNbr;
    int foundAccount = 0;
    int foundUser = 0;
    char userName[100];
    struct Record records[50];
    int numRecords = 0;
    int accountToTransfer = 0;
    int option;

    system("clear");
    printf("\t\t====== Transfer Owner =====\n\n");
chooseAccount:
    printf("\nEnter the number of the account you wish to transfer: ");
    if (scanf("%d", &accountNbr) != 1)
    {
        printf("\n✖ Invalid account number!\n");
        while (getchar() != '\n')
            ;
        goto chooseAccount;
    }

    printf("\nEnter the name of the user you wish to transfer the account to: ");
    scanf("%s", userName);

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("✖ Error opening file.\n");
        return;
    }

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
                  &records[numRecords].id,
                  &records[numRecords].userId,
                  records[numRecords].name,
                  &records[numRecords].accountNbr,
                  &records[numRecords].deposit.month,
                  &records[numRecords].deposit.day,
                  &records[numRecords].deposit.year,
                  records[numRecords].country,
                  &records[numRecords].phone,
                  &records[numRecords].amount,
                  records[numRecords].accountType) != EOF)
    {
        if (strcmp(records[numRecords].name, u.name) == 0 && records[numRecords].accountNbr == accountNbr)
        {
            r = records[numRecords];
            accountToTransfer = numRecords;
            foundAccount = 1;
        }
        if (strcmp(records[numRecords].name, userName) == 0)
        {
            foundUser = 1;
        }
        numRecords++;
    }

    if (foundAccount == 0)
    {
        stayOrReturn(0, update, u);
    }

    if (foundUser == 0)
    {
        printf("\n✖ User not found!\n");
        printf("\nEnter 1 to try again, 2 to return to main menu, or 3 to exit.\n");
    invalid:
        if (scanf("%d", &option) != 1)
        {
            printf("\n✖ Invalid operation!\n");
            while (getchar() != '\n')
                ;
            goto invalid;
        }
        if (option == 1)
            transfer(u);
        else if (option == 2)
            mainMenu(u);
        else if (option == 3)
        {
            printf("\n");
            exit(0);
        }
        else
        {
            printf("Dude, insert a valid operation!\n");
            goto invalid;
        }
    }

    printf("\nTransferring account %d to %s\n", r.accountNbr, userName);

    fclose(pf);

    FILE *fp;
    struct User temp;

    struct User emptyUser;
    emptyUser.id = -1;
    strcpy(emptyUser.name, "");
    strcpy(emptyUser.password, "");

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("✖ Error opening file.\n");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &temp.id, temp.name, temp.password) == 3)
    {
        if (strcmp(userName, temp.name) == 0)
        {
            foundUser = 1;
            r.userId = temp.id;
        }
    }

    fclose(fp);

    FILE *tempPtr;
    tempPtr = fopen("temp.txt", "w");
    if (tempPtr == NULL)
    {
        printf("✖ Error opening temporary file.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        if (i == accountToTransfer)
        {
            fprintf(tempPtr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    r.id,
                    r.userId,
                    userName,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
        }
        else
        {
            fprintf(tempPtr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    records[i].id,
                    records[i].userId,
                    records[i].name,
                    records[i].accountNbr,
                    records[i].deposit.month,
                    records[i].deposit.day,
                    records[i].deposit.year,
                    records[i].country,
                    records[i].phone,
                    records[i].amount,
                    records[i].accountType);
        }
    }

    fclose(tempPtr);
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    printf("\n✔ Account transferred successfully!\n");
    success(u);
}

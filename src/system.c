#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[100], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                  r->accountType,
                  &r->lastChanged.month,
                  &r->lastChanged.day,
                  &r->lastChanged.year,
                  &r->interest) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s %d/%d/%d %lf\n\n",
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
            r.accountType,
            r.lastChanged.month,
            r.lastChanged.day,
            r.lastChanged.year,
            r.interest);
}

struct Date getToday()
{
    struct Date today;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    today.day = tm.tm_mday;
    today.month = tm.tm_mon + 1;
    today.year = tm.tm_year + 1900;
    return today;
}

double getInterestIncrement(struct Record r, struct Date dateTo)
{
    double rate = 0;
    double increment = 0;

    if (strcmp(r.accountType, "current") == 0)
    {
        return 0;
    }

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

    int years = dateTo.year - r.lastChanged.year;
    int months = dateTo.month - r.lastChanged.month;
    int days = dateTo.day - r.lastChanged.day;

    if (years < 0 || (years == 0 && months < 0) || (years == 0 && months == 0 && days < 0))
    {
        return -1;
    }

    double yearly = rate * r.amount;
    double monthly = rate * r.amount / 12;

    increment += years * yearly;
    increment += months * monthly;
    if (days < 0)
    {
        increment -= monthly;
    }

    return increment;
}

void printAccount(struct Record r, struct Date today)
{
    printf("_____________________\n");
    printf("\nAccount number: %d\nCreated: %d/%d/%d \nCountry: %s \nPhone number: %d \nBalance on %d/%d/%d: $%.2lf \nType Of Account: %s\n",
           r.accountNbr,
           r.deposit.month,
           r.deposit.day,
           r.deposit.year,
           r.country,
           r.phone,
           today.month,
           today.day,
           today.year,
           r.amount + r.interest + getInterestIncrement(r, getToday()),
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

void createNewAccount(struct User u)
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

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                  cr.accountType,
                  &cr.lastChanged.month,
                  &cr.lastChanged.day,
                  &cr.lastChanged.year,
                  &cr.interest) != EOF)
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
                createNewAccount(u);
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
    r.lastChanged = r.deposit;

    saveAccountToFile(pf, u, r);

    fclose(pf);
    printf("\n✔ Account created successfully!\n");
    success(u);
}

void checkInterest(struct Record r)
{
    if (strcmp(r.accountType, "current") == 0)
    {
        printf("This bank doesn't offer interest on current accounts.\n");
        return;
    }

    struct Date dateTo;

checkAgain:
    printf("\nEnter the date you want to check the interest for (mm/dd/yyyy): ");
    if (fscanf(stdin, "%d/%d/%d", &dateTo.month, &dateTo.day, &dateTo.year) != 3)
    {
        printf("✖ Invalid date!\n");
        while (getchar() != '\n')
            ;
        goto checkAgain;
    }

    double increment = getInterestIncrement(r, dateTo);
    if (increment == -1)
    {
        printf("\n✖ Invalid date range: Choose a date after the most recent change!\n");
        goto checkAgain;
    }

    double interest = r.interest + getInterestIncrement(r, dateTo);

    printf("\n$%.2lf interest gained on this account by %d/%d/%d.\n\n", interest, dateTo.month, dateTo.day, dateTo.year);
    return;
}

void checkAccount(struct User u)
{
    char userName[100];
    struct Record r;
    int accountNbr = 0;
    int found = 0;
    double rate = 0;
    struct Date today = getToday();

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
            printAccount(r, today);
            if (strcmp(r.accountType, "current") == 0)
            {
                printf("No interest because this is a current account.\n");
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
                    checkInterest(r);
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
    struct Date today = getToday();

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All %s's Accounts =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            found = 1;
            printAccount(r, today);
        }
    }
    fclose(pf);
    if (found == 0)
    {
        printf("\n✖ You have no accounts! Why not mosey on down to the main menu and make one?\n");
    }
    printf("\n");
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

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                  records[numRecords].accountType,
                  &records[numRecords].lastChanged.month,
                  &records[numRecords].lastChanged.day,
                  &records[numRecords].lastChanged.year,
                  &records[numRecords].interest) != EOF)
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
    printf("\nEnter 1 to deposit, 2 to withdraw, or 3 for main menu: ");
    if (scanf("%d", &option) != 1)
    {
        printf("\n✖ Invalid operation!\n");
        while (getchar() != '\n')
            ;
        goto invalid;
    }
    if (option == 1)
    {
        printf("\nEnter amount to deposit: $");
        struct Date today = getToday();
        records[accountToAmend].interest += getInterestIncrement(records[accountToAmend], today);
        scanf("%lf", &amount);
        records[accountToAmend].amount += amount;
        records[accountToAmend].lastChanged = today;
        printf("\n✔ Deposited successfully!\n");
        printf("Your new balance is: $%.2lf\n\n", records[accountToAmend].amount + records[accountToAmend].interest);
    }
    else if (option == 2)
    {
        printf("\nEnter amount to withdraw: $");
        scanf("%lf", &amount);
        if (amount > records[accountToAmend].amount)
        {
            printf("\nInsufficient balance!\n");
        }
        else
        {
            struct Date today = getToday();
            records[accountToAmend].interest += getInterestIncrement(records[accountToAmend], today);
            records[accountToAmend].amount -= amount;
            records[accountToAmend].lastChanged = today;
            printf("\n✔ Withdrawn successfully!\n");
            printf("Your new balance is: $%.2lf\n\n", records[accountToAmend].amount + records[accountToAmend].interest);
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

    FILE *res;
    res = fopen(RECORDS, "w");
    if (res == NULL)
    {
        printf("✖ Error opening file to write.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        fprintf(res, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                records[i].accountType,
                records[i].lastChanged.month,
                records[i].lastChanged.day,
                records[i].lastChanged.year,
                records[i].interest);
    }

    fclose(res);
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

    FILE *ptr = fopen(RECORDS, "r");
    if (ptr == NULL)
    {
        printf("✖ Error opening file.\n");
        return;
    }

    while (fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                  records[numRecords].accountType,
                  &records[numRecords].lastChanged.month,
                  &records[numRecords].lastChanged.day,
                  &records[numRecords].lastChanged.year,
                  &records[numRecords].interest) != EOF)
    {
        if (strcmp(records[numRecords].name, u.name) == 0 && records[numRecords].accountNbr == accountNbr)
        {
            accountFound = 1;
            r = records[numRecords];
            recordToDelete = numRecords;
            printf("\nYou have asked to delete the following account:\n\n");
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nBalance: $%.2lf \nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.month,
                   r.deposit.day,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount + r.interest + getInterestIncrement(r, getToday()),
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

    FILE *res;
    res = fopen(RECORDS, "w");
    if (res == NULL)
    {
        printf("✖ Error opening file to write.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        if (strcmp(records[i].name, u.name) != 0 || records[i].accountNbr != accountNbr)
        {
            fprintf(res, "%d %d %s %d %d/%d/%d %s %d %.2lf %s %d/%d/%d %lf\n\n",
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
                    records[i].accountType,
                    records[i].lastChanged.month,
                    records[i].lastChanged.day,
                    records[i].lastChanged.year,
                    records[i].interest);
        }
    }

    fclose(res);
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

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                  records[numRecords].accountType,
                  &records[numRecords].lastChanged.month,
                  &records[numRecords].lastChanged.day,
                  &records[numRecords].lastChanged.year,
                  &records[numRecords].interest) != EOF)
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
    printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nAmount deposited: $%.2lf \nType Of Account: %s\n",
           r.accountNbr,
           r.deposit.month,
           r.deposit.day,
           r.deposit.year,
           r.country,
           r.phone,
           r.amount + r.interest + getInterestIncrement(r, getToday()),
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

    FILE *res;
    res = fopen(RECORDS, "w");
    if (res == NULL)
    {
        printf("✖ Error opening file to write.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        if (i == accountToUpdate)
        {
            fprintf(res, "%d %d %s %d %d/%d/%d %s %d %.2lf %s %d/%d/%d %lf\n\n",
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
                    r.accountType,
                    r.lastChanged.month,
                    r.lastChanged.day,
                    r.lastChanged.year,
                    r.interest);
        }
        else
        {
            fprintf(res, "%d %d %s %d %d/%d/%d %s %d %.2lf %s %d/%d/%d %lf\n\n",
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
                    records[i].accountType,
                    records[i].lastChanged.month,
                    records[i].lastChanged.day,
                    records[i].lastChanged.year,
                    records[i].interest);
        }
    }

    fclose(res);
    printf("\n✔ Account updated successfully!\n");
    success(u);
}

void transfer(struct User u)
{
    struct Record r;
    struct User recipient;
    struct User temp;
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
        goto invalid;
    }

    printf("\nEnter the name of the user you wish to transfer the account to: ");
    scanf("%s", userName);

    if (strcmp(u.name, userName) == 0)
    {
        printf("\n✖ You can't transfer an account to yourself, silly!\n");
        goto invalid;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("✖ Error opening file.\n");
        return;
    }

    FILE *fp;
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
            recipient.id = temp.id;
            strcpy(recipient.name, temp.name);
            strcpy(recipient.password, temp.password);
            foundUser = 1;
        }
    }

    fclose(fp);

    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s %d/%d/%d %lf\n\n",
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
                  records[numRecords].accountType,
                  &records[numRecords].lastChanged.month,
                  &records[numRecords].lastChanged.day,
                  &records[numRecords].lastChanged.year,
                  &records[numRecords].interest) != EOF)
    {
        if (strcmp(records[numRecords].name, u.name) == 0 && records[numRecords].accountNbr == accountNbr)
        {
            r = records[numRecords];
            accountToTransfer = numRecords;
            foundAccount = 1;
        }
        if (strcmp(records[numRecords].name, userName) == 0)
        {
            if (records[numRecords].accountNbr == accountNbr)
            {
                printf("\n✖ They already have an account of that number!\n");
                printf("You can't have two accounts of the same number.\n");
                printf("That's just not how it works.\n");
                printf("How will they tell them apart?\n");
                printf("Honestly!\n");
                goto invalid;
            }
        }
        numRecords++;
    }

    if (foundAccount == 0)
    {
        printf("\n✖ Lethal error: account not found!\n");
        goto invalid;
    }

    if (foundUser == 0)
    {
        printf("\n✖ Extremely bad error: user not found!\n");
    invalid:
        while (getchar() != '\n')
            ;
        printf("\nEnter 1 to try again, 2 to return to main menu, or 3 to exit.\n");
        if (scanf("%d", &option) != 1)
        {
            printf("\n✖ Invalid operation!\n");
            while (getchar() != '\n')
                ;
            goto invalid;
        }
        if (option == 1)
        {
            while (getchar() != '\n')
                ;
            goto chooseAccount;
        }
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
    fclose(fp);

    FILE *wf = fopen(RECORDS, "w");
    if (wf == NULL)
    {
        printf("✖ Error opening file.\n");
        return;
    }

    for (int i = 0; i < numRecords; i++)
    {
        if (i == accountToTransfer)
        {
            fprintf(wf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s %d/%d/%d %lf\n\n",
                    r.id,
                    recipient.id,
                    recipient.name,
                    accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType,
                    r.lastChanged.month,
                    r.lastChanged.day,
                    r.lastChanged.year,
                    r.interest);
        }
        else
        {
            fprintf(wf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s %d/%d/%d %lf\n\n",
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
                    records[i].accountType,
                    records[i].lastChanged.month,
                    records[i].lastChanged.day,
                    records[i].lastChanged.year,
                    records[i].interest);
        }
    }

    fclose(wf);
    printf("\n✔ Account transferred successfully!\n");
    success(u);
}

#include <termios.h>
#include "header.h"
#include <string.h>

char *USERS = "./data/users.txt";

struct User checkCredentials(char *username, char *password)
{
    FILE *fp;
    struct User temp;

    struct User emptyUser;
    emptyUser.id = -1;
    strcpy(emptyUser.name, "");
    strcpy(emptyUser.password, "");

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return temp;
    }

    while (fscanf(fp, "%d %s %s", &temp.id, temp.name, temp.password) == 3)
    {
        if (strcmp(username, temp.name) == 0 && strcmp(password, temp.password) == 0)
        {
            fclose(fp);
            return temp;
        }
    }

    fclose(fp);
    return emptyUser;
}

void loginMenu(char a[100], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User name: ");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tPassword: ");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

int generateUniqueID()
{
    FILE *fp;
    int id, maxId = 0;
    char name[100], password[50];

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error opening file.");
        return -1;
    }
    else if (fscanf(fp, "%d %s %s\n", &id, name, password) == EOF)
    {
        printf("Error: empty or invalid file.\n");
        return -1;
    }

    while (fscanf(fp, "%d %s %s\n", &id, name, password) == 3)
    {
        if (id > maxId)
        {
            maxId = id;
        }
    }

    fclose(fp);

    return maxId + 1;
}

void createNewUser(struct User *u)
{
    FILE *fp;
    struct User cu;
    char userName[100];

    if (u == NULL)
    {
        printf("Error: null pointer.");
        return;
    }

    if ((fp = fopen("./data/users.txt", "a+")) == NULL)
    {
        printf("Error! opening file\n");
        exit(1);
    }
    rewind(fp); // set pointer to beginning of file
    // Alternatively, you can use fseek(fp, 0, SEEK_SET);
    // Alternatively, "r", then close and open again as "a+"

    printf("Enter your name: ");

choosename:
    scanf("%99s", u->name);

    while (fscanf(fp, "%d %s %s\n\n",
                  &cu.id,
                  cu.name,
                  cu.password) != EOF)
    {
        if (strcmp(cu.name, u->name) == 0)
        {
            printf("Someone of this name already has an account.\n");
            printf("Please choose another name: ");
            goto choosename;
        }
    }

    printf("Choose a password: ");
    scanf("%49s", u->password);

    u->id = generateUniqueID();

    fprintf(fp, "%d %s %s\n", u->id, u->name, u->password);
    fclose(fp);
}

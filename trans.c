#include <stdio.h>
#include <stdlib.h>

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
    char dob[15];
    char nominee[30];
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchRecord(FILE *fPtr);
void displayAll(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    cfPtr = fopen("credit.dat", "rb+");

    if (cfPtr == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blank = {0, "", "", 0.0, "", ""};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }
    }

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            searchRecord(cfPtr);
            break;
        case 6:
            displayAll(cfPtr);
            break;
        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// create text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-15s%-10s%-10s%-15s%-20s\n",
            "Acct", "Last", "First", "Balance", "DOB", "Nominee");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6u%-15s%-10s%-10.2f%-15s%-20s\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance,
                    client.dob,
                    client.nominee);
        }
    }

    fclose(writePtr);
    printf("accounts.txt created.\n");
}

// update record
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Current balance: %.2f\n", client.balance);
    printf("Enter amount (+/-): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated balance: %.2f\n", client.balance);
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0, "", ""};
    unsigned int account;

    printf("Enter account number to delete: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted.\n");
}

// create record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, "", ""};
    unsigned int account;

    printf("Enter new account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account exists.\n");
        return;
    }

    printf("Enter lastName firstName balance DOB nominee:\n");
    scanf("%14s %9s %lf %14s %29s",
          client.lastName,
          client.firstName,
          &client.balance,
          client.dob,
          client.nominee);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created.\n");
}

// search record
void searchRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Not found.\n");
    }
    else
    {
        printf("\nAccount Details:\n");
        printf("Name: %s %s\n", client.firstName, client.lastName);
        printf("Balance: %.2f\n", client.balance);
        printf("DOB: %s\n", client.dob);
        printf("Nominee: %s\n", client.nominee);
    }
}

// display all
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\nAll Accounts:\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%u | %s %s | %.2f | %s | %s\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance,
                   client.dob,
                   client.nominee);
        }
    }
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Export to text file\n");
    printf("2 - Update account\n");
    printf("3 - Add account\n");
    printf("4 - Delete account\n");
    printf("5 - Search account\n");
    printf("6 - Display all accounts\n");
    printf("7 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}
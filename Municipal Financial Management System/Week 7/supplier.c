
#include <stdio.h>
#include <string.h>

#define MAX_SUPPLIERS 100
#define NAME_SIZE 100
#define EMAIL_SIZE 100
#define PHONE_SIZE 30
#define TOWN_SIZE 100
#define DESCRIPTION_SIZE 250

typedef struct {
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char phone[PHONE_SIZE];
    char town[TOWN_SIZE];
    char backup[NAME_SIZE];
    char description[DESCRIPTION_SIZE];
} Supplier;

void addSupplier(Supplier suppliers[], int *count);
void displaySuppliers(Supplier suppliers[], int count);
void searchSupplier(Supplier suppliers[], int count);
void showNameLength(Supplier suppliers[], int count);
void readString(char text[], int size);

int main(void)
{
    Supplier suppliers[MAX_SUPPLIERS];
    int count = 0;
    int choice;

    do {
        printf("\n====================================\n");
        printf("  MUNICIPAL FINANCIAL MANAGEMENT\n");
        printf("====================================\n");
        printf("1. Add Supplier\n");
        printf("2. Display Supplier\n");
        printf("3. Search Supplier\n");
        printf("4. Show Name Length\n");
        printf("5. Exit\n");
        printf("====================================\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");

            // Clear invalid input
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            continue;
        }

    
        while (getchar() != '\n') {
        }

        switch (choice) {
            case 1:
                addSupplier(suppliers, &count);
                break;

            case 2:
                displaySuppliers(suppliers, count);
                break;

            case 3:
                searchSupplier(suppliers, count);
                break;

            case 4:
                showNameLength(suppliers, count);
                break;

            case 5:
                printf("Exiting Supplier Management System.\n");
                break;

            default:
                printf("Invalid choice. Please choose 1 to 5.\n");
        }

    } while (choice != 5);

    return 0;
}


void readString(char text[], int size)
{
    if (fgets(text, size, stdin) != NULL) {
        size_t length = strlen(text);

        if (length > 0 && text[length - 1] == '\n') {
            text[length - 1] = '\0';
        }
    }
}


void addSupplier(Supplier suppliers[], int *count)
{
    if (*count >= MAX_SUPPLIERS) {
        printf("Supplier list is full.\n");
        return;
    }

    Supplier *s = &suppliers[*count];

    printf("\n--- ADD SUPPLIER ---\n");

    printf("Enter supplier name: ");
    readString(s->name, NAME_SIZE);

    printf("Enter email: ");
    readString(s->email, EMAIL_SIZE);

    printf("Enter phone number: ");
    readString(s->phone, PHONE_SIZE);

    printf("Enter town: ");
    readString(s->town, TOWN_SIZE);

    
    strcpy(s->backup, s->name);


    strcpy(s->description, s->name);
    strcat(s->description, " operates in ");
    strcat(s->description, s->town);
    strcat(s->description, ".");

    (*count)++;

    printf("\nSupplier added successfully!\n");
}


void displaySuppliers(Supplier suppliers[], int count)
{
    int i;

    if (count == 0) {
        printf("\nNo suppliers have been added yet.\n");
        return;
    }

    printf("\n--- SUPPLIER DETAILS ---\n");

    for (i = 0; i < count; i++) {
        printf("\nSupplier %d\n", i + 1);
        printf("Name        : %s\n", suppliers[i].name);
        printf("Email       : %s\n", suppliers[i].email);
        printf("Phone       : %s\n", suppliers[i].phone);
        printf("Town        : %s\n", suppliers[i].town);

        
        printf("Backup name : %s\n", suppliers[i].backup);

        
        printf("Description : %s\n", suppliers[i].description);
    }
}


void searchSupplier(Supplier suppliers[], int count)
{
    char searchName[NAME_SIZE];
    int i;
    int found = 0;

    if (count == 0) {
        printf("\nNo suppliers available to search.\n");
        return;
    }

    printf("\nEnter supplier name to search: ");
    readString(searchName, NAME_SIZE);

    for (i = 0; i < count; i++) {
        if (strcmp(searchName, suppliers[i].name) == 0) {
            printf("\nSupplier found!\n");
            printf("Name  : %s\n", suppliers[i].name);
            printf("Email : %s\n", suppliers[i].email);
            printf("Phone : %s\n", suppliers[i].phone);
            printf("Town  : %s\n", suppliers[i].town);

            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("Supplier not found.\n");
    }
}

// LAB TASK 2: String length using strlen()
void showNameLength(Supplier suppliers[], int count)
{
    char searchName[NAME_SIZE];
    int i;
    int found = 0;

    if (count == 0) {
        printf("\nNo suppliers available.\n");
        return;
    }

    printf("\nEnter supplier name: ");
    readString(searchName, NAME_SIZE);

    for (i = 0; i < count; i++) {
        if (strcmp(searchName, suppliers[i].name) == 0) {
            printf("Supplier name length: %zu characters\n",
                   strlen(suppliers[i].name));

            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("Supplier not found.\n");
    }
}

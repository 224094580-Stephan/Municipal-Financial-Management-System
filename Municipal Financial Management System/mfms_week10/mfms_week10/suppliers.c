/* suppliers.c - supplier records and tender evaluation */
#include <stdio.h>
#include <string.h>
#include "suppliers.h"
#include "utilities.h"

static Supplier suppliers[MAX_SUPPLIERS];
static int supplierCount = 0;

static int findSupplierIndex(const char *name)
{
    int i;

    for (i = 0; i < supplierCount; i++) {
        if (strcmp(name, suppliers[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void addSupplier(void)
{
    Supplier *s;

    displayHeader("ADD SUPPLIER");

    if (supplierCount >= MAX_SUPPLIERS) {
        printf("Supplier list is full.\n");
        return;
    }

    s = &suppliers[supplierCount];

    readString("Enter supplier name: ", s->name, sizeof s->name);
    readString("Enter email: ", s->email, sizeof s->email);
    readString("Enter phone number: ", s->phone, sizeof s->phone);
    readString("Enter town: ", s->town, sizeof s->town);

    /* Build the description with strcpy()/strcat(); sizes are safe because
       name (50) + " operates in " (13) + town (50) + "." fits in 200. */
    strcpy(s->description, s->name);
    strcat(s->description, " operates in ");
    strcat(s->description, s->town);
    strcat(s->description, ".");

    supplierCount++;

    printf("\nSupplier added successfully!\n");
}

void displaySuppliers(void)
{
    int i;

    displayHeader("SUPPLIER DETAILS");

    if (supplierCount == 0) {
        printf("No suppliers have been added yet.\n");
        return;
    }

    for (i = 0; i < supplierCount; i++) {
        printf("\nSupplier %d\n", i + 1);
        printf("Name        : %s\n", suppliers[i].name);
        printf("Email       : %s\n", suppliers[i].email);
        printf("Phone       : %s\n", suppliers[i].phone);
        printf("Town        : %s\n", suppliers[i].town);
        printf("Description : %s\n", suppliers[i].description);
    }
}

void searchSupplier(void)
{
    char searchName[NAME_SIZE];
    int position;

    displayHeader("SEARCH SUPPLIER");

    if (supplierCount == 0) {
        printf("No suppliers available to search.\n");
        return;
    }

    readString("Enter supplier name to search: ", searchName, sizeof searchName);
    position = findSupplierIndex(searchName);

    if (position == -1) {
        printf("Supplier not found.\n");
        return;
    }

    printf("\nSupplier found!\n");
    printf("Name  : %s\n", suppliers[position].name);
    printf("Email : %s\n", suppliers[position].email);
    printf("Phone : %s\n", suppliers[position].phone);
    printf("Town  : %s\n", suppliers[position].town);
}

void showNameLength(void)
{
    char searchName[NAME_SIZE];
    int position;

    displayHeader("SUPPLIER NAME LENGTH");

    if (supplierCount == 0) {
        printf("No suppliers available.\n");
        return;
    }

    readString("Enter supplier name: ", searchName, sizeof searchName);
    position = findSupplierIndex(searchName);

    if (position == -1) {
        printf("Supplier not found.\n");
        return;
    }

    printf("Supplier name length: %zu characters\n",
           strlen(suppliers[position].name));
}

void evaluateTender(void)
{
    char supplierName[NAME_SIZE];
    double price, budget;
    int registered, documentsComplete;
    int qualified = 1;

    displayHeader("TENDER EVALUATION");

    readString("Enter supplier name: ", supplierName, sizeof supplierName);
    price             = readDouble("Enter tender price: ", 0.0);
    budget            = readDouble("Enter available budget: ", 0.0);
    registered        = readInt("Is supplier registered? (1=Yes, 0=No): ", 0, 1);
    documentsComplete = readInt("Are all documents complete? (1=Yes, 0=No): ", 0, 1);

    printf("\nSupplier: %s\n", supplierName);

    if (registered == 0) {
        printf("Reason  : Supplier is not registered.\n");
        qualified = 0;
    }
    if (documentsComplete == 0) {
        printf("Reason  : Documents are incomplete.\n");
        qualified = 0;
    }
    if (price > budget) {
        printf("Reason  : Tender price exceeds the available budget.\n");
        qualified = 0;
    }

    printf("Status  : %s\n", qualified ? "Qualified" : "Disqualified");
}

int getSupplierCount(void)
{
    return supplierCount;
}

const Supplier *getSupplier(int index)
{
    if (index < 0 || index >= supplierCount) {
        return NULL;
    }
    return &suppliers[index];
}

void clearSuppliers(void)
{
    supplierCount = 0;
}

int addSupplierRecord(const char *name, const char *email,
                      const char *phone, const char *town)
{
    Supplier *s;

    if (supplierCount >= MAX_SUPPLIERS) {
        return 0;
    }

    s = &suppliers[supplierCount];

    snprintf(s->name, sizeof s->name, "%s", name);
    snprintf(s->email, sizeof s->email, "%s", email);
    snprintf(s->phone, sizeof s->phone, "%s", phone);
    snprintf(s->town, sizeof s->town, "%s", town);
    snprintf(s->description, sizeof s->description, "%s operates in %s.",
             name, town);

    supplierCount++;
    return 1;
}

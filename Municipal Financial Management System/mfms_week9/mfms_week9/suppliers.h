/* suppliers.h - supplier records and tender evaluation */
#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include "config.h"

typedef struct {
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char phone[PHONE_SIZE];
    char town[TOWN_SIZE];
    char description[DESCRIPTION_SIZE];
} Supplier;

void addSupplier(void);
void displaySuppliers(void);
void searchSupplier(void);
void showNameLength(void);
void evaluateTender(void);

#endif

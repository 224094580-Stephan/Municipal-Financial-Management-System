/* budget.h - budget and VAT operations */
#ifndef BUDGET_H
#define BUDGET_H

#include "config.h"

typedef struct {
    char department[NAME_SIZE];
    double amount;
} Budget;

/* Interactive operations used by the menu */
void addBudget(void);
void listBudgets(void);
void budgetCalculator(void);   /* revenue vs expenses: surplus / deficit */
void vatCalculator(void);

/* Calculations */
double calculateBudgetBalance(double revenue, double expenses);
double calculateVAT(double amount);
double calculateTotalBudget(void);

/* Access for other modules (e.g. reports, storage) */
int getBudgetCount(void);
const Budget *getBudget(int index);  /* NULL if index is out of range */

/* Used by the storage module when loading from a file */
void clearBudgets(void);
int addBudgetRecord(const Budget *b);  /* 1 = stored, 0 = rejected */

#endif

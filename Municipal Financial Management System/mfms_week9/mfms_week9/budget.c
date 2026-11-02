/* budget.c - budget and VAT operations */
#include <stdio.h>
#include "budget.h"
#include "utilities.h"

static Budget budgets[MAX_BUDGETS];
static int budgetCount = 0;

double calculateBudgetBalance(double revenue, double expenses)
{
    return revenue - expenses;
}

double calculateVAT(double amount)
{
    return amount * VAT_RATE;
}

double calculateTotalBudget(void)
{
    double total = 0.0;
    int i;

    for (i = 0; i < budgetCount; i++) {
        total += budgets[i].amount;
    }
    return total;
}

int getBudgetCount(void)
{
    return budgetCount;
}

const Budget *getBudget(int index)
{
    if (index < 0 || index >= budgetCount) {
        return NULL;
    }
    return &budgets[index];
}

void addBudget(void)
{
    Budget b;

    displayHeader("ADD DEPARTMENT BUDGET");

    if (budgetCount >= MAX_BUDGETS) {
        printf("Budget list is full (%d departments).\n", MAX_BUDGETS);
        return;
    }

    readString("Enter department name: ", b.department, sizeof b.department);
    b.amount = readDouble("Enter budget amount: ", 0.0);

    budgets[budgetCount] = b;
    budgetCount++;

    printf("\nBudget added successfully!\n");
}

void listBudgets(void)
{
    int i;

    displayHeader("DEPARTMENT BUDGETS");

    if (budgetCount == 0) {
        printf("No department budgets have been added yet.\n");
        return;
    }

    printf("%-4s %-25s %15s\n", "No.", "Department", "Budget");
    printf("-------------------------------------------------\n");

    for (i = 0; i < budgetCount; i++) {
        printf("%-4d %-25.25s %15.2f\n",
               i + 1, budgets[i].department, budgets[i].amount);
    }

    printf("\nTotal Department Budget: %.2f\n", calculateTotalBudget());
}

void budgetCalculator(void)
{
    double revenue, expenses, balance;

    displayHeader("MUNICIPAL BUDGET CALCULATOR");

    revenue  = readDouble("Enter total revenue: ", 0.0);
    expenses = readDouble("Enter total expenses: ", 0.0);

    balance = calculateBudgetBalance(revenue, expenses);

    printf("\nRevenue : %.2f\n", revenue);
    printf("Expenses: %.2f\n", expenses);

    if (balance > 0) {
        printf("Surplus : %.2f\n", balance);
    } else if (balance < 0) {
        printf("Deficit : %.2f\n", -balance);
    } else {
        printf("The budget is balanced.\n");
    }
}

void vatCalculator(void)
{
    double amount;

    displayHeader("VAT CALCULATOR");

    amount = readDouble("Enter amount: ", 0.0);

    printf("\nVAT rate : %.0f%%\n", VAT_RATE * 100.0);
    printf("VAT      : %.2f\n", calculateVAT(amount));
    printf("Total    : %.2f\n", amount + calculateVAT(amount));
}

/* reports.c - reports and summaries.
   Reads data through the employees and budget modules' public interfaces. */
#include <stdio.h>
#include "reports.h"
#include "employees.h"
#include "budget.h"
#include "utilities.h"

static double grossOf(const Employee *e)
{
    return calculateGrossSalary(e->basic, e->housing, e->transport);
}

void employeeReport(void)
{
    int count = getEmployeeCount();
    int order[MAX_EMPLOYEES];
    int i, j, key;
    double totalGross, totalNet = 0.0, average;
    const Employee *e;
    const Employee *highest;
    const Employee *lowest;

    displayHeader("EMPLOYEE SALARY REPORT");

    if (count == 0) {
        printf("No employees have been added yet.\n");
        return;
    }

    highest = getEmployee(0);
    lowest  = getEmployee(0);

    for (i = 0; i < count; i++) {
        e = getEmployee(i);
        totalNet += calculateNetSalary(grossOf(e), e->tax);
        if (grossOf(e) > grossOf(highest)) {
            highest = e;
        }
        if (grossOf(e) < grossOf(lowest)) {
            lowest = e;
        }
        order[i] = i;
    }

    totalGross = calculatePayrollTotal();
    average = totalGross / count;

    printf("Number of employees   : %d\n", count);
    printf("Total gross payroll   : %.2f\n", totalGross);
    printf("Total net payroll     : %.2f\n", totalNet);
    printf("Average gross salary  : %.2f\n", average);
    printf("Highest gross salary  : %.2f (%s)\n", grossOf(highest), highest->name);
    printf("Lowest gross salary   : %.2f (%s)\n", grossOf(lowest), lowest->name);

    /* Insertion sort of positions by gross salary, lowest to highest */
    for (i = 1; i < count; i++) {
        key = order[i];
        j = i - 1;
        while (j >= 0 && grossOf(getEmployee(order[j])) > grossOf(getEmployee(key))) {
            order[j + 1] = order[j];
            j--;
        }
        order[j + 1] = key;
    }

    printf("\nGross salaries, lowest to highest:\n");
    for (i = 0; i < count; i++) {
        e = getEmployee(order[i]);
        printf("  %-20.20s %12.2f\n", e->name, grossOf(e));
    }
}

void budgetReport(void)
{
    int count = getBudgetCount();
    int order[MAX_BUDGETS];
    int i, j, key;
    double total, average, payroll, balance;
    const Budget *b;
    const Budget *highest;
    const Budget *lowest;

    displayHeader("DEPARTMENT BUDGET REPORT");

    if (count == 0) {
        printf("No department budgets have been added yet.\n");
        return;
    }

    highest = getBudget(0);
    lowest  = getBudget(0);

    for (i = 0; i < count; i++) {
        b = getBudget(i);
        if (b->amount > highest->amount) {
            highest = b;
        }
        if (b->amount < lowest->amount) {
            lowest = b;
        }
        order[i] = i;
    }

    total = calculateTotalBudget();
    average = total / count;

    printf("Number of departments    : %d\n", count);
    printf("Total department budget  : %.2f\n", total);
    printf("Average department budget: %.2f\n", average);
    printf("Highest budget           : %.2f (%s)\n", highest->amount, highest->department);
    printf("Lowest budget            : %.2f (%s)\n", lowest->amount, lowest->department);

    for (i = 1; i < count; i++) {
        key = order[i];
        j = i - 1;
        while (j >= 0 && getBudget(order[j])->amount > getBudget(key)->amount) {
            order[j + 1] = order[j];
            j--;
        }
        order[j + 1] = key;
    }

    printf("\nBudgets, lowest to highest:\n");
    for (i = 0; i < count; i++) {
        b = getBudget(order[i]);
        printf("  %-25.25s %12.2f", b->department, b->amount);
        if (total > 0) {
            printf("  (%.1f%%)", b->amount / total * 100.0);
        }
        printf("\n");
    }

    /* Cross-module check: can the total budget cover the payroll? */
    payroll = calculatePayrollTotal();
    balance = calculateBudgetBalance(total, payroll);

    printf("\nPayroll check (budget vs total gross payroll):\n");
    printf("  Total gross payroll: %.2f\n", payroll);
    if (balance > 0) {
        printf("  Surplus after payroll: %.2f\n", balance);
    } else if (balance < 0) {
        printf("  Deficit after payroll: %.2f\n", -balance);
    } else {
        printf("  Budget exactly matches payroll.\n");
    }
}

void municipalityReport(const char *municipalityName,
                        const char *mayorName,
                        int population)
{
    displayHeader("MUNICIPAL REPORT");

    printf("Municipality Name : %s\n", municipalityName);
    printf("Mayor's Name      : %s\n", mayorName);
    printf("Population        : %d\n", population);
}

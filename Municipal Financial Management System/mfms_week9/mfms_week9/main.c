/* main.c - application flow and menu (the coordinator) */
#include <stdio.h>
#include "config.h"
#include "utilities.h"
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "reports.h"

static void displayMainMenu(void)
{
    displayHeader("MUNICIPAL FINANCIAL MANAGEMENT SYSTEM");
    printf(" EMPLOYEES\n");
    printf("   1.  Add Employee\n");
    printf("   2.  List Employees\n");
    printf("   3.  Search Employee\n");
    printf("   4.  Salary Calculator\n");
    printf(" BUDGET\n");
    printf("   5.  Add Department Budget\n");
    printf("   6.  List Department Budgets\n");
    printf("   7.  Budget Calculator (Revenue vs Expenses)\n");
    printf("   8.  VAT Calculator\n");
    printf(" SUPPLIERS\n");
    printf("   9.  Add Supplier\n");
    printf("   10. Display Suppliers\n");
    printf("   11. Search Supplier\n");
    printf("   12. Show Supplier Name Length\n");
    printf("   13. Tender Evaluation\n");
    printf(" REPORTS\n");
    printf("   14. Employee Salary Report\n");
    printf("   15. Department Budget Report\n");
    printf("   16. Municipality Profile\n");
    printf("   0.  Exit\n");
    printf("===============================================\n");
}

static void municipalityProfile(void)
{
    char municipalityName[NAME_SIZE];
    char mayorName[NAME_SIZE];
    int population;

    displayHeader("MUNICIPALITY PROFILE");

    readString("Enter Municipality Name: ", municipalityName, sizeof municipalityName);
    readString("Enter Mayor's Name: ", mayorName, sizeof mayorName);
    population = readInt("Enter Population: ", 0, 2000000000);

    municipalityReport(municipalityName, mayorName, population);
}

int main(void)
{
    int choice;

    printf("Welcome to Windhoek Municipality\n");

    do {
        displayMainMenu();
        choice = readInt("Enter choice: ", 0, 16);

        switch (choice) {
            case 1:  addEmployee();        break;
            case 2:  listEmployees();      break;
            case 3:  searchEmployee();     break;
            case 4:  salaryCalculator();   break;
            case 5:  addBudget();          break;
            case 6:  listBudgets();        break;
            case 7:  budgetCalculator();   break;
            case 8:  vatCalculator();      break;
            case 9:  addSupplier();        break;
            case 10: displaySuppliers();   break;
            case 11: searchSupplier();     break;
            case 12: showNameLength();     break;
            case 13: evaluateTender();     break;
            case 14: employeeReport();     break;
            case 15: budgetReport();       break;
            case 16: municipalityProfile(); break;
            case 0:
                printf("\nThank you for using MFMS.\n");
                break;
            default:
                break;
        }

        if (choice != 0) {
            pauseScreen();
        }

    } while (choice != 0);

    return 0;
}

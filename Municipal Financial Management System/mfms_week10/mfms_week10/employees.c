/* employees.c - employee operations */
#include <stdio.h>
#include "employees.h"
#include "utilities.h"

static Employee employees[MAX_EMPLOYEES];
static int employeeCount = 0;

double calculateGrossSalary(double basic, double housing, double transport)
{
    return basic + housing + transport;
}

double calculateNetSalary(double gross, double tax)
{
    return gross - tax;
}

int findEmployeeIndex(int id)
{
    int i;

    for (i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            return i;
        }
    }
    return -1;
}

int getEmployeeCount(void)
{
    return employeeCount;
}

const Employee *getEmployee(int index)
{
    if (index < 0 || index >= employeeCount) {
        return NULL;
    }
    return &employees[index];
}

double calculatePayrollTotal(void)
{
    double total = 0.0;
    int i;

    for (i = 0; i < employeeCount; i++) {
        total += calculateGrossSalary(employees[i].basic,
                                      employees[i].housing,
                                      employees[i].transport);
    }
    return total;
}

void addEmployee(void)
{
    Employee e;
    double gross;

    displayHeader("ADD EMPLOYEE");

    if (employeeCount >= MAX_EMPLOYEES) {
        printf("Employee list is full (%d employees).\n", MAX_EMPLOYEES);
        return;
    }

    e.id = readInt("Enter employee ID: ", 1, 999999);
    if (findEmployeeIndex(e.id) != -1) {
        printf("An employee with ID %d already exists.\n", e.id);
        return;
    }

    readString("Enter employee name: ", e.name, sizeof e.name);
    e.basic     = readDouble("Enter basic salary: ", 0.0);
    e.housing   = readDouble("Enter housing allowance: ", 0.0);
    e.transport = readDouble("Enter transport allowance: ", 0.0);
    e.tax       = readDouble("Enter tax: ", 0.0);

    gross = calculateGrossSalary(e.basic, e.housing, e.transport);
    if (e.tax > gross) {
        printf("Tax (%.2f) cannot be more than gross salary (%.2f). "
               "Employee not added.\n", e.tax, gross);
        return;
    }

    employees[employeeCount] = e;
    employeeCount++;

    printf("\nEmployee added successfully!\n");
}

void listEmployees(void)
{
    int i;
    double gross;

    displayHeader("EMPLOYEE LIST");

    if (employeeCount == 0) {
        printf("No employees have been added yet.\n");
        return;
    }

    printf("%-6s %-20s %12s %12s\n", "ID", "Name", "Gross", "Net");
    printf("---------------------------------------------------\n");

    for (i = 0; i < employeeCount; i++) {
        gross = calculateGrossSalary(employees[i].basic,
                                     employees[i].housing,
                                     employees[i].transport);
        printf("%-6d %-20.20s %12.2f %12.2f\n",
               employees[i].id, employees[i].name, gross,
               calculateNetSalary(gross, employees[i].tax));
    }

    printf("\nTotal employees: %d\n", employeeCount);
}

void searchEmployee(void)
{
    int id;
    int position;
    double gross;

    displayHeader("SEARCH EMPLOYEE");

    if (employeeCount == 0) {
        printf("No employees available to search.\n");
        return;
    }

    id = readInt("Enter employee ID: ", 1, 999999);
    position = findEmployeeIndex(id);

    if (position == -1) {
        printf("Employee not found.\n");
        return;
    }

    gross = calculateGrossSalary(employees[position].basic,
                                 employees[position].housing,
                                 employees[position].transport);

    printf("\nEmployee found at position %d.\n", position + 1);
    printf("ID        : %d\n", employees[position].id);
    printf("Name      : %s\n", employees[position].name);
    printf("Basic     : %.2f\n", employees[position].basic);
    printf("Housing   : %.2f\n", employees[position].housing);
    printf("Transport : %.2f\n", employees[position].transport);
    printf("Tax       : %.2f\n", employees[position].tax);
    printf("Gross     : %.2f\n", gross);
    printf("Net       : %.2f\n", calculateNetSalary(gross, employees[position].tax));
}

void salaryCalculator(void)
{
    double basic, housing, transport, tax, gross;

    displayHeader("EMPLOYEE SALARY CALCULATOR");

    basic     = readDouble("Enter basic salary: ", 0.0);
    housing   = readDouble("Enter housing allowance: ", 0.0);
    transport = readDouble("Enter transport allowance: ", 0.0);
    tax       = readDouble("Enter tax: ", 0.0);

    gross = calculateGrossSalary(basic, housing, transport);

    printf("\nGross Salary: %.2f\n", gross);
    printf("Net Salary  : %.2f\n", calculateNetSalary(gross, tax));
}

void clearEmployees(void)
{
    employeeCount = 0;
}

int addEmployeeRecord(const Employee *e)
{
    double gross;

    if (employeeCount >= MAX_EMPLOYEES || e->id < 1 ||
        e->basic < 0.0 || e->housing < 0.0 ||
        e->transport < 0.0 || e->tax < 0.0 ||
        findEmployeeIndex(e->id) != -1) {
        return 0;
    }

    gross = calculateGrossSalary(e->basic, e->housing, e->transport);
    if (e->tax > gross) {
        return 0;
    }

    employees[employeeCount] = *e;
    employeeCount++;
    return 1;
}

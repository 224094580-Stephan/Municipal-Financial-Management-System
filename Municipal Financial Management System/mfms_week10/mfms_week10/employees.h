/* employees.h - employee operations */
#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include "config.h"

typedef struct {
    int id;
    char name[NAME_SIZE];
    double basic;
    double housing;
    double transport;
    double tax;
} Employee;

/* Interactive operations used by the menu */
void addEmployee(void);
void listEmployees(void);
void searchEmployee(void);
void salaryCalculator(void);

/* Calculations */
double calculateGrossSalary(double basic, double housing, double transport);
double calculateNetSalary(double gross, double tax);
double calculatePayrollTotal(void);      /* total gross salary of all employees */

/* Access for other modules (e.g. reports, storage) */
int findEmployeeIndex(int id);           /* position, or -1 if not found */
int getEmployeeCount(void);
const Employee *getEmployee(int index);  /* NULL if index is out of range */

/* Used by the storage module when loading from a file */
void clearEmployees(void);
int addEmployeeRecord(const Employee *e);  /* 1 = stored, 0 = rejected */

#endif


#include <stdio.h>

void displayWelcome(void);
float calculateVAT(float amount);
float calculateSalary(float basic, float housing, float transport);
float calculateBudget(float revenue, float expenses);
void displayMenu(void);
int searchEmployee(int id, int ids[], int size);

int main(void)
{
    int choice;
    float amount, vat;
    float basic, housing, transport, grossSalary;
    float revenue, expenses, balance;

    int employeeIDs[] = {101, 102, 103, 104, 105};
    int employeeID;
    int position;

    int size = sizeof(employeeIDs) / sizeof(employeeIDs[0]);

    displayWelcome();

    do {
        displayMenu();

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
                
                printf("\nEnter amount: ");
                scanf("%f", &amount);

                vat = calculateVAT(amount);

                printf("VAT: %.2f\n", vat);
                break;

            case 2:
                
                printf("\nBasic salary: ");
                scanf("%f", &basic);

                printf("Housing allowance: ");
                scanf("%f", &housing);

                printf("Transport allowance: ");
                scanf("%f", &transport);

                grossSalary = calculateSalary(
                    basic, housing, transport
                );

                printf("Gross salary: %.2f\n", grossSalary);
                break;

            case 3:
                
                printf("\nEnter revenue: ");
                scanf("%f", &revenue);

                printf("Enter expenses: ");
                scanf("%f", &expenses);

                balance = calculateBudget(revenue, expenses);

                printf("Budget balance: %.2f\n", balance);

                if (balance > 0) {
                    printf("SURPLUS\n");
                } else if (balance < 0) {
                    printf("DEFICIT\n");
                } else {
                    printf("BALANCED\n");
                }
                break;

            case 4:
                
                printf("\nEnter employee ID: ");
                scanf("%d", &employeeID);

                position = searchEmployee(
                    employeeID, employeeIDs, size
                );

                if (position != -1) {
                    printf("Employee found at position %d.\n",
                           position);
                } else {
                    printf("Employee not found.\n");
                }
                break;

            case 5:
                printf("\nThank you for using MFMS.\n");
                break;

            default:
                printf("Invalid choice. Please choose 1 to 5.\n");
        }

    } while (choice != 5);

    return 0;
}


void displayWelcome(void)
{
    printf("Welcome to the Municipal Financial Management System\n");
}


float calculateVAT(float amount)
{
    return amount * 0.15f;
}


float calculateSalary(float basic, float housing, float transport)
{
    return basic + housing + transport;
}


float calculateBudget(float revenue, float expenses)
{
    return revenue - expenses;
}

void displayMenu(void)
{
    printf("\n==================================\n");
    printf("MUNICIPAL FINANCIAL MANAGEMENT\n");
    printf("==================================\n");
    printf("1. Calculate VAT\n");
    printf("2. Calculate Salary\n");
    printf("3. Calculate Budget\n");
    printf("4. Search Employee\n");
    printf("5. Exit\n");
}

int searchEmployee(int id, int ids[], int size)
{
    int i;

    for (i = 0; i < size; i++) {
        if (ids[i] == id) {
            return i;
        }
    }

    return -1;
}

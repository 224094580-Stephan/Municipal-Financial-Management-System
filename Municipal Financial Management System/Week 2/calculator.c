#include <stdio.h>

int main() {
    double revenue;
    double expenses;
    double balance;

    printf("\n===============================================\n");
    printf("       MUNICIPAL BUDGET CALCULATOR");
    printf("\n===============================================\n");

    printf("Enter total revenue: ");
    scanf("%lf", &revenue);

    printf("Enter total expenses: ");
    scanf("%lf", &expenses);

    balance = revenue - expenses;

    printf("\nRevenue: %.2f\n", revenue);
    printf("Expenses: %.2f\n", expenses);
 
    if (balance > 0) {
    printf("Surplus: %.2f\n", balance);
    }
    else if (balance < 0) {
    printf("Deficit: %.2f\n", -balance);
    }
    else {
    printf("The budget is balanced.\n");
    }

     return 0;
}
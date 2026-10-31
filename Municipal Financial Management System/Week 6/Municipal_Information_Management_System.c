#include <stdio.h>

int main(){
    float salaries [50];
    float budgets [10];

    float totalSalary = 0;
    float averageSalary;
    float highestSalary;
    float lowestSalary;
    float searchSalary;

    float totalBudget = 0;
    float averageBudget;
    float temp;

    int found;
    int i, j;

    printf("==================================\n");
    printf(" MUNICIPAL INFORMATION MANAGEMENT");
    printf("==================================\n");

    printf("Employee Salaries\n");
    printf("-------------------\n");

    for(i = 0; i < 50; i++){
        printf("Enter salary for employee %d: ", i + 1);
        scanf("%f", &salaries[i]);
    }

    printf("\nAll Employee Salaries:\n");

    for(i = 0; i < 50; i++){
        printf("Employee %d: %2f\n", i + 1, salaries[i]);
    }

    totalSalary = 0;

    for(i = 0; i < 50; i++){
       totalSalary = totalSalary +  salaries[i];
    }

    printf("\nTotal Salary Expenditure: %.2f\n", totalSalary);

    averageSalary = totalSalary / 50;

    printf("Average Salary: %.2f\n", averageSalary);

    highestSalary = salaries[0];

    for (i = 1; i < 50; i++)
    {
        if (salaries[i] > highestSalary)
        {
            highestSalary = salaries[i];
        }
    }

    printf("Highest Salary: %.2f\n", highestSalary);

    lowestSalary = salaries[0];

    for (i = 1; i < 50; i++)
    {
        if (salaries[i] < lowestSalary)
        {
            lowestSalary = salaries[i];
        }
    }

    printf("Lowest Salary: %.2f\n", lowestSalary);

    printf("\nEnter a salary to search for: ");
    scanf("%f", &searchSalary);

    found = 0;

    for (i = 0; i < 50; i++)
    {
        if (salaries[i] == searchSalary)
        {
            printf("Salary %.2f found at employee %d.\n",
                   searchSalary, i + 1);

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("Salary %.2f was not found.\n", searchSalary);
    }

    for (i = 0; i < 50; i++)
    {
        for (j = i + 1; j < 50; j++)
        {
            if (salaries[i] > salaries[j])
            {
                temp = salaries[i];
                salaries[i] = salaries[j];
                salaries[j] = temp;
            }
        }
    }

    printf("\nSalaries Sorted From Lowest to Highest:\n");

    for (i = 0; i < 50; i++)
    {
        printf("%.2f\n", salaries[i]);
    }

    printf("\n\nDEPARTMENT BUDGETS\n");
    printf("------------------\n");

    for (i = 0; i < 10; i++)
    {
        printf("Enter budget for department %d: ", i + 1);
        scanf("%f", &budgets[i]);
    }

    printf("\nDepartment Budgets:\n");

    for (i = 0; i < 10; i++)
    {
        printf("Department %d: %.2f\n", i + 1, budgets[i]);
    }

    totalBudget = 0;

    for (i = 0; i < 10; i++)
    {
        totalBudget = totalBudget + budgets[i];
    }

    printf("\nTotal Department Budget: %.2f\n", totalBudget);

    averageBudget = totalBudget / 10;

    printf("Average Department Budget: %.2f\n", averageBudget);

    for (i = 0; i < 10; i++)
    {
        for (j = i + 1; j < 10; j++)
        {
            if (budgets[i] > budgets[j])
            {
                temp = budgets[i];
                budgets[i] = budgets[j];
                budgets[j] = temp;
            }
        }
    }

    printf("\nBudgets Sorted From Lowest to Highest:\n");

    for (i = 0; i < 10; i++)
    {
        printf("%.2f\n", budgets[i]);
    }

    printf("\n=====================================\n");
    printf("       PROGRAM COMPLETE\n");
    printf("=====================================\n");

    return 0;

}
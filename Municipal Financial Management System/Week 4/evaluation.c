#include <stdio.h>evalu

int main() {
    char supplierName[50];
    float price;
    float budget;
    int registered;
    int documentsComplete;

    printf("\n===============================================\n");
    printf("               TENDER EVALUATION");
    printf("\n===============================================\n");

    printf("Enter supplier name: \n");
    scanf("%49s", &supplierName);

    printf("Enter tender price: ");
    scanf("%f", &price);

    printf("Enter available budget: ");
    scanf("%f", &budget);

    printf("Is supplier registered? (1=Yes, 0=No): ");
    scanf("%d", &registered);

    printf("Are all documents complete? (1=Yes, 0=No): ");
    scanf("%d", &documentsComplete);

    if (registered == 0 || documentsComplete == 0)
    {
        printf("\nSupplier: %s\n", supplierName);
        printf("Status: Disqualified\n");
    }
    else if (price > budget)
    {
        printf("\nSupplier: %s\n", supplierName);
        printf("Status: Disqualified\n");
    }
    else
    {
        printf("\nSupplier: %s\n", supplierName);
        printf("Status: Qualified\n");
    }

    return 0;
}
#include <stdio.h>

int main() {
    char municipalityName[50];
    char mayorName[50];
    int population;
   
    printf("MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");

    printf("Welcome to Windhoek Municipality\n\n");

    printf("Enter Municipality Name: ");
    fgets(municipalityName, sizeof(municipalityName), stdin);

    printf("Enter Mayor's Name: ");
    fgets(mayorName, sizeof(mayorName), stdin);

    printf("Enter Population: ");
    scanf("%d", &population);

    printf("\n===============================================\n");
    printf("              MUNICIPAL REPORT");
    printf("\n===============================================\n");

    printf("Municipality Name : %s", municipalityName);
    printf("Mayor's Name      : %s", mayorName);
    printf("Population        : %d\n\n", population);

    return 0;
}
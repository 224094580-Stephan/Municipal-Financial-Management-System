#include <stdio.h>

int main()
{
    char municipalityName;
    char mayorName;
    int population;

    printf("MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");

    printf("Welcome to Windhoek Municipality\n");

    printf("Enter Municipality Name: ");
    scanf(" %c", &municipalityName);

    printf("Enter Mayor's Name: ");
    scanf(" %c", &mayorName);

    printf("Enter Population: ");
    scanf("%d", &population);

    printf("MUNICIPALITY INFORMATION\n");

    printf("Municipality Name : %c", municipalityName);
    printf("Mayor's Name      : %c", mayorName);
    printf("Population        : %d", population);

    return 0;
}
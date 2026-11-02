/* assets.c - municipal asset register */
#include <stdio.h>
#include "assets.h"
#include "utilities.h"

static Asset assets[MAX_ASSETS];
static int assetCount = 0;

static int findAssetIndex(int id)
{
    int i;

    for (i = 0; i < assetCount; i++) {
        if (assets[i].id == id) {
            return i;
        }
    }
    return -1;
}

int getAssetCount(void)
{
    return assetCount;
}

const Asset *getAsset(int index)
{
    if (index < 0 || index >= assetCount) {
        return NULL;
    }
    return &assets[index];
}

double calculateTotalAssetValue(void)
{
    double total = 0.0;
    int i;

    for (i = 0; i < assetCount; i++) {
        total += assets[i].value;
    }
    return total;
}

void clearAssets(void)
{
    assetCount = 0;
}

int addAssetRecord(const Asset *a)
{
    if (assetCount >= MAX_ASSETS || a->id < 1 || a->value < 0.0 ||
        findAssetIndex(a->id) != -1) {
        return 0;
    }
    assets[assetCount] = *a;
    assetCount++;
    return 1;
}

void addAsset(void)
{
    Asset a;

    displayHeader("ADD ASSET");

    if (assetCount >= MAX_ASSETS) {
        printf("Asset list is full (%d assets).\n", MAX_ASSETS);
        return;
    }

    a.id = readInt("Enter asset ID: ", 1, 999999);
    if (findAssetIndex(a.id) != -1) {
        printf("An asset with ID %d already exists.\n", a.id);
        return;
    }

    readString("Enter asset name: ", a.name, sizeof a.name);
    a.value = readDouble("Enter asset value: ", 0.0);

    addAssetRecord(&a);
    printf("\nAsset added successfully!\n");
}

void listAssets(void)
{
    int i;

    displayHeader("ASSET REGISTER");

    if (assetCount == 0) {
        printf("No assets have been added yet.\n");
        return;
    }

    printf("%-6s %-25s %15s\n", "ID", "Name", "Value");
    printf("---------------------------------------------------\n");

    for (i = 0; i < assetCount; i++) {
        printf("%-6d %-25.25s %15.2f\n",
               assets[i].id, assets[i].name, assets[i].value);
    }

    printf("\nTotal assets: %d   Total value: %.2f\n",
           assetCount, calculateTotalAssetValue());
}

/* assets.h - municipal asset register */
#ifndef ASSETS_H
#define ASSETS_H

#include "config.h"

typedef struct {
    int id;
    char name[NAME_SIZE];
    double value;
} Asset;

void addAsset(void);
void listAssets(void);

double calculateTotalAssetValue(void);

/* Access for other modules (e.g. storage) */
int getAssetCount(void);
const Asset *getAsset(int index);  /* NULL if index is out of range */

/* Used by the storage module when loading from a file */
void clearAssets(void);
int addAssetRecord(const Asset *a);  /* 1 = stored, 0 = rejected */

#endif

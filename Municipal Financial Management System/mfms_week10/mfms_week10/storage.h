/* storage.h - persistent storage: text files, binary files, backup */
#ifndef STORAGE_H
#define STORAGE_H

/* Text files (employees, suppliers, assets, budgets) */
void saveAllText(void);
void loadAllText(void);

/* Binary file (employees only) */
void saveEmployeesBinary(void);
void loadEmployeesBinary(void);

/* Copy every existing data file into data/backup/ */
void backupData(void);

/* Show size and bytes-per-record of employees.txt versus employees.dat */
void compareEmployeeFiles(void);

#endif

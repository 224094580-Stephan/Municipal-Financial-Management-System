/* config.h - shared application constants for the MFMS */
#ifndef CONFIG_H
#define CONFIG_H

#define MAX_EMPLOYEES     50
#define MAX_BUDGETS       10
#define MAX_SUPPLIERS     100
#define MAX_ASSETS        100

#define NAME_SIZE         50
#define EMAIL_SIZE        100
#define PHONE_SIZE        30
#define TOWN_SIZE         50
#define DESCRIPTION_SIZE  200

#define VAT_RATE          0.15

/* Week 10: persistent storage */
#define DATA_DIR          "data"
#define BACKUP_DIR        "data/backup"
#define EMPLOYEES_TXT     "data/employees.txt"
#define SUPPLIERS_TXT     "data/suppliers.txt"
#define ASSETS_TXT        "data/assets.txt"
#define BUDGETS_TXT       "data/budgets.txt"
#define EMPLOYEES_DAT     "data/employees.dat"

#endif

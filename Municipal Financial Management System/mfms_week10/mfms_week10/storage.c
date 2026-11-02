/* storage.c - persistent storage for the MFMS.

   Text format: one record per line, fields separated by '|'.
     employees.txt : id|name|basic|housing|transport|tax
     suppliers.txt : name|email|phone|town
     assets.txt    : id|name|value
     budgets.txt   : department|amount
   Binary format: employees.dat holds raw Employee structs, one after another.

   Saving never overwrites a good file with a half-written one: data is written
   to "<file>.tmp" first and only renamed over the real file once every write
   and the fclose() succeeded. */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "storage.h"
#include "config.h"
#include "utilities.h"
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "assets.h"

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MAKE_DIR(path) mkdir((path), 0755)
#endif

#define LINE_SIZE 512

/* ---------- helpers ---------- */

/* Create a directory if it does not exist. Returns 1 if it is usable. */
static int makeDirectory(const char *path)
{
    if (MAKE_DIR(path) != 0 && errno != EEXIST) {
        perror(path);
        return 0;
    }
    return 1;
}

static int ensureDataDirectory(void)
{
    return makeDirectory(DATA_DIR);
}

/* Size of a file in bytes, or -1 if it cannot be opened. */
static long fileSize(const char *path)
{
    FILE *fp = fopen(path, "rb");
    long size;

    if (fp == NULL) {
        return -1L;
    }
    if (fseek(fp, 0L, SEEK_END) != 0) {
        fclose(fp);
        return -1L;
    }
    size = ftell(fp);
    fclose(fp);
    return size;
}

static int fileExists(const char *path)
{
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        return 0;
    }
    fclose(fp);
    return 1;
}

/* Remove the trailing newline (and carriage return) from a line. */
static void trimLine(char *line)
{
    size_t length = strlen(line);

    while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
        line[--length] = '\0';
    }
}

static int isBlankLine(const char *line)
{
    while (*line != '\0') {
        if (*line != ' ' && *line != '\t') {
            return 0;
        }
        line++;
    }
    return 1;
}

/* ---------- safe saving ---------- */

/* A writer writes every record to fp and returns how many it wrote,
   or -1 if any write failed. */
typedef int (*WriteFn)(FILE *fp);

/* Write via a temporary file, then rename it over the real file.
   Returns the number of records saved, or -1 on error. */
static int safeSave(const char *path, const char *mode, WriteFn writer)
{
    char tmpPath[128];
    FILE *fp;
    int written;

    snprintf(tmpPath, sizeof tmpPath, "%s.tmp", path);

    fp = fopen(tmpPath, mode);
    if (fp == NULL) {
        perror(tmpPath);
        return -1;
    }

    written = writer(fp);

    if (fclose(fp) != 0 || written < 0) {
        fprintf(stderr, "Error: could not write %s completely. "
                        "The existing file was not changed.\n", path);
        remove(tmpPath);
        return -1;
    }

#ifdef _WIN32
    /* On Windows rename() fails if the target exists. */
    remove(path);
#endif
    if (rename(tmpPath, path) != 0) {
        perror(path);
        remove(tmpPath);
        return -1;
    }

    return written;
}

static int writeEmployeesText(FILE *fp)
{
    int i, count = getEmployeeCount();
    const Employee *e;

    for (i = 0; i < count; i++) {
        e = getEmployee(i);
        if (fprintf(fp, "%d|%s|%.2f|%.2f|%.2f|%.2f\n",
                    e->id, e->name, e->basic, e->housing,
                    e->transport, e->tax) < 0) {
            return -1;
        }
    }
    return count;
}

static int writeSuppliersText(FILE *fp)
{
    int i, count = getSupplierCount();
    const Supplier *s;

    for (i = 0; i < count; i++) {
        s = getSupplier(i);
        if (fprintf(fp, "%s|%s|%s|%s\n",
                    s->name, s->email, s->phone, s->town) < 0) {
            return -1;
        }
    }
    return count;
}

static int writeAssetsText(FILE *fp)
{
    int i, count = getAssetCount();
    const Asset *a;

    for (i = 0; i < count; i++) {
        a = getAsset(i);
        if (fprintf(fp, "%d|%s|%.2f\n", a->id, a->name, a->value) < 0) {
            return -1;
        }
    }
    return count;
}

static int writeBudgetsText(FILE *fp)
{
    int i, count = getBudgetCount();
    const Budget *b;

    for (i = 0; i < count; i++) {
        b = getBudget(i);
        if (fprintf(fp, "%s|%.2f\n", b->department, b->amount) < 0) {
            return -1;
        }
    }
    return count;
}

static int writeEmployeesBinary(FILE *fp)
{
    int i, count = getEmployeeCount();
    const Employee *e;
    Employee out;

    for (i = 0; i < count; i++) {
        e = getEmployee(i);

        /* Zero the whole record first so unused name bytes and struct
           padding do not leak leftover memory into the file. */
        memset(&out, 0, sizeof out);
        out.id = e->id;
        snprintf(out.name, sizeof out.name, "%s", e->name);
        out.basic = e->basic;
        out.housing = e->housing;
        out.transport = e->transport;
        out.tax = e->tax;

        if (fwrite(&out, sizeof out, 1, fp) != 1) {
            return -1;
        }
    }
    return count;
}

/* Save one entity to a text file. An empty list is never saved, so that
   pressing Save before Load cannot wipe the records already on disk. */
static void saveEntity(const char *label, const char *path,
                       int count, WriteFn writer)
{
    int saved;

    if (count == 0) {
        if (fileExists(path)) {
            printf("  %-10s nothing in memory - existing %s kept.\n", label, path);
        } else {
            printf("  %-10s nothing to save.\n", label);
        }
        return;
    }

    saved = safeSave(path, "w", writer);
    if (saved >= 0) {
        printf("  %-10s saved %d record(s) to %s\n", label, saved, path);
    } else {
        printf("  %-10s SAVE FAILED.\n", label);
    }
}

void saveAllText(void)
{
    displayHeader("SAVE DATA (TEXT FILES)");

    if (!ensureDataDirectory()) {
        printf("Cannot use the %s directory. Nothing was saved.\n", DATA_DIR);
        return;
    }

    saveEntity("Employees", EMPLOYEES_TXT, getEmployeeCount(), writeEmployeesText);
    saveEntity("Suppliers", SUPPLIERS_TXT, getSupplierCount(), writeSuppliersText);
    saveEntity("Assets",    ASSETS_TXT,    getAssetCount(),    writeAssetsText);
    saveEntity("Budgets",   BUDGETS_TXT,   getBudgetCount(),   writeBudgetsText);
}

void saveEmployeesBinary(void)
{
    int saved;

    displayHeader("SAVE EMPLOYEES (BINARY FILE)");

    if (!ensureDataDirectory()) {
        printf("Cannot use the %s directory. Nothing was saved.\n", DATA_DIR);
        return;
    }

    if (getEmployeeCount() == 0) {
        printf("No employees in memory - nothing saved.\n");
        return;
    }

    saved = safeSave(EMPLOYEES_DAT, "wb", writeEmployeesBinary);
    if (saved >= 0) {
        printf("Saved %d employee record(s) to %s (%lu bytes each).\n",
               saved, EMPLOYEES_DAT, (unsigned long)sizeof(Employee));
    } else {
        printf("SAVE FAILED.\n");
    }
}

/* ---------- loading ---------- */

/* A parser turns one text line into a stored record.
   Returns 1 if the record was stored, 0 if it was rejected. */
typedef int (*ParseFn)(const char *line);

static int parseEmployee(const char *line)
{
    Employee e;

    if (sscanf(line, "%d|%49[^|]|%lf|%lf|%lf|%lf",
               &e.id, e.name, &e.basic, &e.housing,
               &e.transport, &e.tax) != 6) {
        return 0;
    }
    return addEmployeeRecord(&e);
}

static int parseSupplier(const char *line)
{
    char name[NAME_SIZE], email[EMAIL_SIZE], phone[PHONE_SIZE], town[TOWN_SIZE];

    if (sscanf(line, "%49[^|]|%99[^|]|%29[^|]|%49[^\n]",
               name, email, phone, town) != 4) {
        return 0;
    }
    return addSupplierRecord(name, email, phone, town);
}

static int parseAsset(const char *line)
{
    Asset a;

    if (sscanf(line, "%d|%49[^|]|%lf", &a.id, a.name, &a.value) != 3) {
        return 0;
    }
    return addAssetRecord(&a);
}

static int parseBudget(const char *line)
{
    Budget b;

    if (sscanf(line, "%49[^|]|%lf", b.department, &b.amount) != 2) {
        return 0;
    }
    return addBudgetRecord(&b);
}

/* Load one text file. Memory is replaced only if the file could be opened.
   A missing file is not an error: the user simply has not saved yet. */
static void loadEntity(const char *label, const char *path,
                       ParseFn parse, void (*clear)(void))
{
    FILE *fp;
    char line[LINE_SIZE];
    int lineNumber = 0, loaded = 0, skipped = 0;

    fp = fopen(path, "r");
    if (fp == NULL) {
        if (errno == ENOENT) {
            printf("  %-10s no saved file yet (%s) - memory unchanged.\n",
                   label, path);
        } else {
            perror(path);
            printf("  %-10s LOAD FAILED - memory unchanged.\n", label);
        }
        return;
    }

    /* First pass: make sure the whole file can be read before touching
       memory. (fopen() can succeed on something that cannot be read, such
       as a directory.) */
    while (fgets(line, sizeof line, fp) != NULL) {
    }
    if (ferror(fp)) {
        perror(path);
        printf("  %-10s LOAD FAILED - memory unchanged.\n", label);
        fclose(fp);
        return;
    }
    rewind(fp);

    clear();

    while (fgets(line, sizeof line, fp) != NULL) {
        lineNumber++;
        trimLine(line);

        if (isBlankLine(line)) {
            continue;
        }
        if (parse(line)) {
            loaded++;
        } else {
            skipped++;
            printf("  %-10s line %d skipped (malformed, duplicate or invalid)\n",
                   label, lineNumber);
        }
    }

    if (ferror(fp)) {
        printf("  %-10s read error after line %d - data may be incomplete.\n",
               label, lineNumber);
    }
    fclose(fp);

    printf("  %-10s loaded %d record(s), skipped %d, from %s\n",
           label, loaded, skipped, path);
}

void loadAllText(void)
{
    displayHeader("LOAD DATA (TEXT FILES)");
    printf("Loading replaces the records currently in memory.\n\n");

    loadEntity("Employees", EMPLOYEES_TXT, parseEmployee, clearEmployees);
    loadEntity("Suppliers", SUPPLIERS_TXT, parseSupplier, clearSuppliers);
    loadEntity("Assets",    ASSETS_TXT,    parseAsset,    clearAssets);
    loadEntity("Budgets",   BUDGETS_TXT,   parseBudget,   clearBudgets);
}

void loadEmployeesBinary(void)
{
    FILE *fp;
    Employee e;
    long size;
    int loaded = 0, skipped = 0;

    displayHeader("LOAD EMPLOYEES (BINARY FILE)");
    printf("Loading replaces the employees currently in memory.\n\n");

    fp = fopen(EMPLOYEES_DAT, "rb");
    if (fp == NULL) {
        if (errno == ENOENT) {
            printf("No saved file yet (%s) - memory unchanged.\n", EMPLOYEES_DAT);
        } else {
            perror(EMPLOYEES_DAT);
            printf("LOAD FAILED - memory unchanged.\n");
        }
        return;
    }

    /* First pass: confirm the file can be read before replacing memory. */
    while (fread(&e, sizeof e, 1, fp) == 1) {
    }
    if (ferror(fp)) {
        perror(EMPLOYEES_DAT);
        printf("LOAD FAILED - memory unchanged.\n");
        fclose(fp);
        return;
    }
    rewind(fp);

    size = fileSize(EMPLOYEES_DAT);
    if (size >= 0 && size % (long)sizeof(Employee) != 0) {
        printf("Warning: file size (%ld bytes) is not a multiple of the record "
               "size (%lu bytes).\n"
               "The file may be damaged; incomplete trailing data is ignored.\n",
               size, (unsigned long)sizeof(Employee));
    }

    clearEmployees();

    while (fread(&e, sizeof e, 1, fp) == 1) {
        e.name[NAME_SIZE - 1] = '\0';   /* never trust a string from a file */
        if (addEmployeeRecord(&e)) {
            loaded++;
        } else {
            skipped++;
        }
    }

    if (ferror(fp)) {
        printf("Read error - data may be incomplete.\n");
    }
    fclose(fp);

    printf("Loaded %d employee record(s), skipped %d, from %s\n",
           loaded, skipped, EMPLOYEES_DAT);
}

/* ---------- backup ---------- */

/* Copy src to dst. Returns bytes copied, or -1 on error. */
static long copyFile(const char *src, const char *dst)
{
    FILE *in, *out;
    char buffer[4096];
    size_t n;
    long total = 0;

    in = fopen(src, "rb");
    if (in == NULL) {
        perror(src);
        return -1L;
    }

    out = fopen(dst, "wb");
    if (out == NULL) {
        perror(dst);
        fclose(in);
        return -1L;
    }

    while ((n = fread(buffer, 1, sizeof buffer, in)) > 0) {
        if (fwrite(buffer, 1, n, out) != n) {
            perror(dst);
            fclose(in);
            fclose(out);
            return -1L;
        }
        total += (long)n;
    }

    if (ferror(in)) {
        perror(src);
        fclose(in);
        fclose(out);
        return -1L;
    }

    fclose(in);
    if (fclose(out) != 0) {
        perror(dst);
        return -1L;
    }
    return total;
}

void backupData(void)
{
    const char *files[] = { EMPLOYEES_TXT, SUPPLIERS_TXT, ASSETS_TXT,
                            BUDGETS_TXT, EMPLOYEES_DAT };
    const int fileCount = (int)(sizeof files / sizeof files[0]);
    char dst[160];
    long bytes;
    int i, copied = 0, missing = 0, failed = 0;

    displayHeader("BACKUP DATA");

    if (!ensureDataDirectory() || !makeDirectory(BACKUP_DIR)) {
        printf("Cannot create the backup directory. Nothing was backed up.\n");
        return;
    }

    for (i = 0; i < fileCount; i++) {
        if (!fileExists(files[i])) {
            printf("  %-20s not found - skipped\n", files[i]);
            missing++;
            continue;
        }

        /* backup name: data/backup/<name>.bak (skip the "data/" prefix) */
        snprintf(dst, sizeof dst, "%s/%s.bak", BACKUP_DIR,
                 files[i] + strlen(DATA_DIR) + 1);

        bytes = copyFile(files[i], dst);
        if (bytes < 0) {
            failed++;
        } else {
            printf("  %-20s -> %s (%ld bytes)\n", files[i], dst, bytes);
            copied++;
        }
    }

    printf("\nBackup finished: %d copied, %d not found, %d failed.\n",
           copied, missing, failed);
}

/* ---------- text vs binary comparison ---------- */

static int countTextRecords(const char *path)
{
    FILE *fp = fopen(path, "r");
    char line[LINE_SIZE];
    int count = 0;

    if (fp == NULL) {
        return -1;
    }
    while (fgets(line, sizeof line, fp) != NULL) {
        trimLine(line);
        if (!isBlankLine(line)) {
            count++;
        }
    }
    fclose(fp);
    return count;
}

void compareEmployeeFiles(void)
{
    long textSize = fileSize(EMPLOYEES_TXT);
    long binSize = fileSize(EMPLOYEES_DAT);
    int textRecords = countTextRecords(EMPLOYEES_TXT);
    long binRecords;

    displayHeader("TEXT VS BINARY: EMPLOYEE FILES");

    if (textSize < 0 || binSize < 0) {
        printf("Both %s and %s must exist.\n"
               "Use options 19 (Save Data) and 21 (Save Employees Binary) first.\n",
               EMPLOYEES_TXT, EMPLOYEES_DAT);
        return;
    }

    binRecords = binSize / (long)sizeof(Employee);

    printf("%-20s %10s %9s %15s\n", "File", "Size", "Records", "Bytes/record");
    printf("--------------------------------------------------------\n");
    printf("%-20s %10ld %9d %15.1f\n", EMPLOYEES_TXT, textSize, textRecords,
           textRecords > 0 ? (double)textSize / textRecords : 0.0);
    printf("%-20s %10ld %9ld %15.1f\n", EMPLOYEES_DAT, binSize, binRecords,
           binRecords > 0 ? (double)binSize / (double)binRecords : 0.0);

    printf("\nText   : human-readable, record size varies with the data, "
           "needs parsing.\n");
    printf("Binary : every record is exactly %lu bytes (names are padded to %d "
           "bytes), not readable,\n"
           "         and layout depends on this compiler and computer.\n",
           (unsigned long)sizeof(Employee), NAME_SIZE);
}

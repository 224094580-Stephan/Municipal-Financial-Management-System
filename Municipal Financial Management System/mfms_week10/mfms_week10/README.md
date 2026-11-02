# MFMS – Week 10 Laboratory: Persistent Storage (File Handling)

PAP521S – Programming in Practice. This project extends the Week 9 modular MFMS
so that Employees, Suppliers, Assets and Budgets survive program termination.
Records can be saved to and loaded from text files, and employees can also be
stored in a binary file.

## What was added to Week 9

| Change | Where |
|--------|-------|
| **Assets** (the lab needs them; Week 9 had none) | `assets.c / assets.h`, menu 17–18 |
| **All file handling** | `storage.c / storage.h` |
| Record-level functions so loaded data can be put back into each module | `clearX()` and `addXRecord()` in `employees`, `budget`, `suppliers`, `assets` |
| File and data-directory constants | `config.h` |
| `'|'` rejected in typed text (it is the field separator in the files) | `utilities.c: readString` |
| New menu options 17–24 | `main.c` |

## Menu (new options)

```
ASSETS         17 Add Asset                 18 List Assets
DATA STORAGE   19 Save Data (text files)    20 Load Data (text files)
               21 Save Employees (binary)   22 Load Employees (binary)
               23 Backup Data               24 Compare Text vs Binary Employee Files
```

Options 1–16 are unchanged from Week 9.

## Build

```
gcc -std=c99 -Wall -Wextra -pedantic -c main.c
gcc -std=c99 -Wall -Wextra -pedantic -c employees.c
gcc -std=c99 -Wall -Wextra -pedantic -c budget.c
gcc -std=c99 -Wall -Wextra -pedantic -c utilities.c
gcc -std=c99 -Wall -Wextra -pedantic -c reports.c
gcc -std=c99 -Wall -Wextra -pedantic -c suppliers.c
gcc -std=c99 -Wall -Wextra -pedantic -c assets.c
gcc -std=c99 -Wall -Wextra -pedantic -c storage.c
gcc main.o employees.o budget.o utilities.o reports.o suppliers.o assets.o storage.o -o mfms
./mfms
```

Or run `./build.sh`. No compiler warnings. Run the program from the project folder
so the relative `data/` path resolves.

## Data directory and file formats

All files live in `data/` (the program creates it if it is missing; an empty one is
included). Backups go to `data/backup/`.

| Entity | Text file | Line format |
|--------|-----------|-------------|
| Employees | `data/employees.txt` | `id\|name\|basic\|housing\|transport\|tax` |
| Suppliers | `data/suppliers.txt` | `name\|email\|phone\|town` |
| Assets | `data/assets.txt` | `id\|name\|value` |
| Budgets | `data/budgets.txt` | `department\|amount` |
| Employees (binary) | `data/employees.dat` | raw `Employee` structs, 88 bytes each |

Example (`tests/sample_data/employees.txt`):

```
101|Maria Shikongo|18500.50|3000.00|1500.00|2500.00
102|Simon Haufiku|17200.00|2500.00|1200.00|2000.00
```

**The separator problem.** A name containing `|` would break the format, so
`readString` refuses `|` when the user types it. Names with spaces are fine
because loading uses `%49[^|]` (read up to the next `|`) rather than `%s`.

**Why `fgets` + `sscanf` instead of `fscanf`.** Each line is read whole, then
parsed. A bad line can be reported by number and skipped, and the next line is
still read correctly. With `fscanf` a malformed record would leave the stream in
the middle of a line and every later record would be misread.

## How saving and loading behave

- **Save writes a temporary file first** (`employees.txt.tmp`), checks every write
  and the `fclose()`, and only then renames it over the real file. If anything
  fails, the old file is untouched. This avoids the danger of `"w"` truncating a
  good file halfway through a failed save.
- **An empty list is never saved.** Pressing Save before Load would otherwise wipe
  the records on disk. The program says "existing file kept" instead.
- **Load replaces memory, but only if the file can be read.** The whole file is
  read once first; if that fails (for example the path is a directory), memory is
  left as it was.
- **A missing file is not an error.** It means nothing has been saved yet; memory is
  unchanged and a message says so.
- **Every load prints a record count** (loaded / skipped), and every save prints how
  many records were written. Skipped lines are reported by line number.
- **Rejected on load:** malformed lines, duplicate employee or asset IDs, negative
  amounts, tax greater than gross salary, and anything beyond the array capacity.
  The same rules apply as when typing the record in.
- **Binary load** checks that the file size is a multiple of the record size and
  warns about damage; incomplete trailing bytes are ignored, and each name is
  force-terminated because a damaged file cannot be trusted to contain a `'\0'`.
- **Backup** copies every existing data file to `data/backup/<name>.bak`.

## Lab task checklist

| Lab task | Done in |
|----------|---------|
| 7. Create a data directory | `data/` (also created by the program) |
| 8. Text save for employees | `saveAllText` → `writeEmployeesText` |
| 9. Text load for employees | `loadAllText` → `parseEmployee` |
| 10. Repeat for suppliers, assets, budgets | same pattern, three more writer/parser pairs |
| 11. `fopen()` error handling | every `fopen` is checked; `perror()` reports the cause |
| 12–13. Save / Load menu options | menu 19, 20 |
| 14. Enter, save, close, reopen, load | Tests 1 and 2 (two separate program runs) |
| 15. Binary version for at least one entity | employees: menu 21, 22 |
| 16. Compare text and binary | menu 24 and the section below |
| Challenge: Backup Data | menu 23 |
| Challenge: Search loaded records | menu 3 after loading |
| Challenge: record count after loading | printed by every load |
| Challenge: file does not exist yet | handled, see Test 3 |
| Challenge: confirmation of records saved/loaded | printed by every save and load |
| Challenge: test empty, missing, malformed, duplicate | Test 3 |

## Text vs binary: what the program measured

Same three employees, saved both ways (`tests/test1_output.txt`, option 24):

| File | Size | Records | Bytes per record |
|------|-----:|--------:|-----------------:|
| `employees.txt` | 155 bytes | 3 | 51.7 (varies) |
| `employees.dat` | 264 bytes | 3 | 88 (always) |

| | Text | Binary |
|-|------|--------|
| Readable / editable by hand | Yes | No |
| Record size | Varies with the data | Fixed (`sizeof(Employee)` = 88) |
| Size for short names | Smaller | Larger: the name always takes 50 bytes |
| Code | Format and parse each field | One `fwrite` / `fread` per record |
| Damage handling | A bad line can be skipped | A bad byte shifts every later record |
| Portable between computers | Mostly | No: depends on compiler, type sizes, struct padding and byte order |

In `employees.dat` each 88-byte record is: `id` (4 bytes) + `name` (50) + 2 bytes of
padding the compiler inserts so the `double` fields line up + 4 doubles (32).
Before writing, each record is zeroed (`memset`) so leftover memory is never
written into the file; the first version of this lab leaked stack garbage into
the `.dat` file, which is visible in a hex dump.

Binary is convenient here because the lab runs one program on one machine, but it
should not be used as an interchange format. The text files are the safer format
for moving data and for debugging.

## Test evidence (`tests/`)

Run each test from a folder containing `mfms` (and, for tests 3 and 4, the prepared
`data/`): `./mfms < tests/test1_input.txt`.

| Test | Input / Output | What it shows |
|------|----------------|---------------|
| 1 | `test1_input.txt`, `test1_output.txt` | Enter 3 employees (a name with `\|` is rejected and re-asked), 2 budgets, 2 suppliers, 2 assets. Save text, save binary, compare, backup. |
| 2 | `test2_input.txt`, `test2_output.txt` | **A new program run with empty memory**: the list is empty, then Load restores everything (counts printed); search, reports and binary load work on the loaded data. |
| 3 | `test3_input.txt`, `test3_output.txt`, `test3_data/` | Employees file with a duplicate ID, a broken line, tax above gross, and a blank line (3 skipped, 3 loaded); a malformed supplier line; an **empty** budgets file; a **missing** assets file; a **damaged** binary file (5 stray bytes); Save does not overwrite budgets with nothing. |
| 4 | `test4_input.txt`, `test4_output.txt` | `fopen`/read failure: `data/employees.txt` and `data/employees.dat` are directories. Load reports the OS error and keeps memory; Save fails safely without losing the in-memory records; other files still save. |

`tests/sample_data/` holds the files produced by tests 1 and 2.

The program also runs with no errors under `-fsanitize=address,undefined` for tests 1–3.

## File operations and how each can fail

| Operation | Can fail because | What the program does |
|-----------|------------------|-----------------------|
| `fopen(tmp, "w"/"wb")` | Folder missing, no permission, disk full or read-only | `perror`, save aborted, old file untouched |
| `fprintf` / `fwrite` | Disk full, I/O error | Detected, temp file removed, old file kept |
| `fclose` after writing | Buffered data could not be flushed | Treated as a failed save |
| `rename(tmp, path)` | Target is a directory or locked | `perror`, temp file removed |
| `fopen(path, "r"/"rb")` | File missing (normal first run) | "No saved file yet", memory unchanged |
| | Permission denied or other error | `perror`, memory unchanged |
| `fgets` / `fread` | Read error (e.g. path is a directory) | Caught by the first pass; memory unchanged |
| `sscanf` | Line malformed | Line skipped and reported |
| `fread` | File truncated or damaged | Size warning; incomplete trailing bytes ignored |
| `mkdir` | Cannot create `data/` | `perror`, nothing saved |
| `fopen(backup)` / copy | Source or destination unavailable | Reported; other files still backed up |

## Module dependencies

```
main.c      -> utilities.h employees.h budget.h suppliers.h assets.h reports.h storage.h config.h
storage.c   -> storage.h config.h utilities.h employees.h budget.h suppliers.h assets.h
reports.c   -> reports.h employees.h budget.h utilities.h
assets.c    -> assets.h utilities.h        (assets.h -> config.h)
employees.c, budget.c, suppliers.c -> own header + utilities.h   (headers -> config.h)
```

`storage.c` is the only module that touches files. The data modules know nothing
about the file format; they only offer `clear` and `add record` functions.

## Known limits

- The `'|'` character cannot be used inside names, emails or towns.
- Load replaces the records in memory; there is no merge, and no confirmation prompt.
- The binary file is only valid for the same compiler and computer that wrote it.
- Only employees have a binary version, as the lab requires.
- `mkdir` and the Windows `rename` fallback are written for Linux/macOS and
  Windows but were only tested on Linux.

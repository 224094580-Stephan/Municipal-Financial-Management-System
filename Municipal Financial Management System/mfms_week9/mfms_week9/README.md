# MFMS – Week 9 Laboratory: Modularised Municipal Financial Management System

PAP521S – Programming in Practice. This project converts the earlier single-file
programs (main.c, calculator.c, evaluation.c, EmployeeSalaryCalaculator.c,
Municipal_Information_Management_System.c, salaryAnalysis.c, supplier.c and
mfms_functions.c) into one multi-file application.

## Modules

| Module | Responsibility | Public functions |
|--------|----------------|------------------|
| `main.c` | Application flow and menu only | `main()` (menu and municipality profile are private) |
| `employees.c / .h` | Employee records, salary calculations, search | `addEmployee`, `listEmployees`, `searchEmployee`, `salaryCalculator`, `calculateGrossSalary`, `calculateNetSalary`, `calculatePayrollTotal`, `findEmployeeIndex`, `getEmployeeCount`, `getEmployee` |
| `budget.c / .h` | Department budgets, revenue vs expenses, VAT | `addBudget`, `listBudgets`, `budgetCalculator`, `vatCalculator`, `calculateBudgetBalance`, `calculateVAT`, `calculateTotalBudget`, `getBudgetCount`, `getBudget` |
| `utilities.c / .h` | Reusable, validated input helpers | `readInt`, `readDouble`, `readString`, `pauseScreen`, `displayHeader` |
| `reports.c / .h` | Reports and summaries (calls the employees and budget modules) | `employeeReport`, `budgetReport`, `municipalityReport` |
| `suppliers.c / .h` | Challenge: supplier records and tender evaluation | `addSupplier`, `displaySuppliers`, `searchSupplier`, `showNameLength`, `evaluateTender` |
| `config.h` | Challenge: single shared header of application constants | `MAX_EMPLOYEES`, `MAX_BUDGETS`, `MAX_SUPPLIERS`, `NAME_SIZE`, `VAT_RATE`, … |

Employee, budget and supplier data live in `static` arrays inside their own
`.c` files. Other modules reach them only through the functions above, so a
module can change how it stores data without breaking the rest.

### Where the earlier programs went

| Earlier program | Now in |
|-----------------|--------|
| `main.c` (municipality name, mayor, population) | `main.c` option 16 + `reports.c: municipalityReport` |
| `calculator.c` | `budget.c: budgetCalculator` |
| `evaluation.c` | `suppliers.c: evaluateTender` |
| `EmployeeSalaryCalaculator.c` | `employees.c: salaryCalculator` |
| `Municipal_Information_Management_System.c` and `salaryAnalysis.c` (total, average, highest, lowest, sorting) | `reports.c: employeeReport`, `budgetReport` |
| `supplier.c` | `suppliers.c` |
| `mfms_functions.c` (VAT, salary, budget, search) | `budget.c`, `employees.c` |

## Build

Compile each source file separately, then link (the lab's commands):

```
gcc -std=c99 -Wall -Wextra -pedantic -c main.c
gcc -std=c99 -Wall -Wextra -pedantic -c employees.c
gcc -std=c99 -Wall -Wextra -pedantic -c budget.c
gcc -std=c99 -Wall -Wextra -pedantic -c utilities.c
gcc -std=c99 -Wall -Wextra -pedantic -c reports.c
gcc -std=c99 -Wall -Wextra -pedantic -c suppliers.c
gcc main.o employees.o budget.o utilities.o reports.o suppliers.o -o mfms
./mfms
```

Or run `./build.sh`, which does the same. On Windows the executable is `mfms.exe`.
All files compile with no warnings.

## Header files

Every header has an include guard (`#ifndef NAME_H` / `#define NAME_H` / `#endif`)
and contains only prototypes, typedefs and constants. No function bodies are in
headers, which avoids "multiple definition" linker errors.

## Test evidence (in `tests/`)

| Run | Input file | Output file | What it covers |
|-----|------------|-------------|----------------|
| 1 | `input1.txt` | `output1.txt` | Add employees, duplicate ID rejected, list, search (found / not found), salary calculator, employee report |
| 2 | `input2.txt` | `output2.txt` | Invalid menu input, add budgets, list, surplus and deficit, VAT, budget report with payroll cross-check |
| 3 | `input3.txt` | `output3.txt` | Add suppliers, display, search (found / not found), name length, tender evaluation (qualified / disqualified), municipality profile |

Reproduce a run with: `./mfms < tests/input1.txt`

## Linker-error demonstration (challenge)

`tests/linker_error_demo.txt` shows the result of linking **without** `employees.o`:

```
gcc main.o budget.o utilities.o reports.o suppliers.o -o mfms_broken
```

Each file compiled on its own because the header `employees.h` declares the
functions, so the compiler trusts they exist. The linker then cannot find the
machine code for `addEmployee`, `calculatePayrollTotal` and others, so it
reports `undefined reference`. A prototype promises a function exists; only the
object file that contains it keeps that promise.

## Module dependency diagram

```
main.c      -> config.h  utilities.h  employees.h  budget.h  suppliers.h  reports.h
reports.c   -> reports.h employees.h  budget.h  utilities.h
employees.c -> employees.h utilities.h  (employees.h -> config.h)
budget.c    -> budget.h utilities.h     (budget.h    -> config.h)
suppliers.c -> suppliers.h utilities.h  (suppliers.h -> config.h)
utilities.c -> utilities.h
```

## Menu

```
EMPLOYEES  1 Add   2 List   3 Search   4 Salary Calculator
BUDGET     5 Add Dept Budget   6 List   7 Budget Calculator   8 VAT Calculator
SUPPLIERS  9 Add   10 Display   11 Search   12 Name Length   13 Tender Evaluation
REPORTS    14 Employee Report   15 Budget Report   16 Municipality Profile
           0 Exit
```

## Improvements made while modularising

- All input goes through `readInt`, `readDouble` and `readString`, which reject
  letters, empty names, negative amounts and out-of-range values, and never
  overflow a buffer.
- Fixed bugs in the earlier code: `evaluation.c` had a stray `evalu` after
  `#include <stdio.h>`; `Municipal_Information_Management_System.c` printed
  salaries with `%2f` instead of `%.2f`; `salaryAnalysis.c` recalculated the
  average inside the loop; scanf calls were unchecked.
- Duplicate employee IDs are rejected, and tax larger than gross salary is refused.
- Tender evaluation now prints the reason(s) for disqualification.
- The Week 10 file handling (save/load) is not part of this lab and is not included.

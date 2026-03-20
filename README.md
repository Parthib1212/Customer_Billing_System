# Customer Billing System (C Language)

An intermediate-level CLI application developed in C to manage customer records and generate invoices with automated tax calculations.

## 🚀 Features
* **Customer Management:** Add, search, and delete customer profiles.
* **Automated Billing:** Calculates subtotals and applies a **19% VAT** (Standard German/Bayern rate).
* **Data Persistence:** Uses binary file handling (`.dat`) to save records permanently on the disk.
* **Detailed Search:** Retrieve full itemized invoices by searching for a specific Customer ID.
* **Record Management:** Built-in logic to edit existing profiles or safely remove them using temporary file swapping.

## 🛠️ Tech Stack
* **Language:** C
* **Compiler:** Clang/LLVM
* **Environment:** macOS / VS Code
* **Concepts Used:** Structs, File I/O (Binary), String manipulation, Pointers.

## 📋 How to Run
1. Clone this repository to your MacBook.
2. Open the folder in Terminal.
3. Compile the code:
   ```bash
   clang main.c -o billing
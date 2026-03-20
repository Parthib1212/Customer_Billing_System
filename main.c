#include <stdio.h>
#include <string.h>

// 1. Structure for Date
struct Date {
    int day;
    int month;
    int year;
};

// 2. Structure for individual Items in a bill
struct Item {
    char itemName[50];
    int quantity;
    float unitPrice;
};

// 3. Main Customer Structure
struct Customer {
    char customerID[10];
    char name[50];
    char address[100];
    char contact[15];
    struct Date purchaseDate; // Nested structure
    struct Item items[10];    // Array of items (max 10 per bill for now)
    int itemCount;            // To track how many items are actually in the bill
    float totalAmount;
};
void showMenu() {
    printf("\n===== CUSTOMER BILLING SYSTEM =====");
    printf("\n1. Add New Customer & Generate Bill");
    printf("\n2. Display All Bills");
    printf("\n3. Search for a Customer Bill");
    printf("\n4. Edit/Delete Bill (Intermediate Task)");
    printf("\n5. Exit");
    printf("\n====================================");
    printf("\nSelect an option: ");
}

void addCustomer() {
    struct Customer c;
    c.itemCount = 0;
    c.totalAmount = 0;
    float subtotal = 0;

    printf("\n--- New Billing Entry ---\n");
    printf("Enter Customer ID: ");
    scanf("%s", c.customerID);
    getchar(); // Clean buffer

    printf("Enter Customer Name: ");
    fgets(c.name, 50, stdin);
    c.name[strcspn(c.name, "\n")] = 0; // Remove the trailing newline

    printf("Enter Address: ");
    fgets(c.address, 100, stdin);
    c.address[strcspn(c.address, "\n")] = 0;

    printf("Enter Contact: ");
    scanf("%s", c.contact);

    printf("Enter Date (dd mm yyyy): ");
    scanf("%d %d %d", &c.purchaseDate.day, &c.purchaseDate.month, &c.purchaseDate.year);

    // Adding Items
    char addMore;
    do {
        printf("\nEnter Item Name: ");
        getchar(); // Clean buffer
        fgets(c.items[c.itemCount].itemName, 50, stdin);
        c.items[c.itemCount].itemName[strcspn(c.items[c.itemCount].itemName, "\n")] = 0;

        printf("Enter Quantity: ");
        scanf("%d", &c.items[c.itemCount].quantity);

        printf("Enter Unit Price: ");
        scanf("%f", &c.items[c.itemCount].unitPrice);

        subtotal += (c.items[c.itemCount].quantity * c.items[c.itemCount].unitPrice);
        c.itemCount++;

        printf("Add another item? (y/n): ");
        scanf(" %c", &addMore);
    } while (addMore == 'y' && c.itemCount < 10);

    // Calculations (Germany/Bayern 19% Tax)
    float tax = subtotal * 0.19;
    c.totalAmount = subtotal + tax;

    printf("\n--- Bill Summary ---");
    printf("\nSubtotal: %.2f", subtotal);
    printf("\nVAT (19%%): %.2f", tax);
    printf("\nTotal Payable: %.2f\n", c.totalAmount);

    //  SAVE TO FILE ---
    FILE *fp = fopen("bills.dat", "ab"); // 'ab' means Append Binary
    if (fp == NULL) {
        printf("\nError: Could not open file for saving.\n");
    } else {
        fwrite(&c, sizeof(struct Customer), 1, fp);
        fclose(fp);
        printf("\nBill saved successfully to 'bills.dat'!\n");
    }
}

void displayAllBills() {
    struct Customer c;
    FILE *fp = fopen("bills.dat", "rb");

    if (fp == NULL) {
        printf("\nNo records found. Please add a customer first.\n");
        return;
    }

    printf("\n--- ALL REGISTERED BILLS ---\n");
    printf("%-10s %-20s %-15s %-12s %-10s\n", "ID", "Name", "Contact", "Date", "Total (€)");
    printf("--------------------------------------------------------------------------\n");

    // fread returns 1 as long as it successfully reads one 'struct Customer'
    while (fread(&c, sizeof(struct Customer), 1, fp)) {
        printf("%-10s %-20s %-15s %02d/%02d/%-6d %.2f\n", 
               c.customerID, 
               c.name, 
               c.contact, 
               c.purchaseDate.day, 
               c.purchaseDate.month, 
               c.purchaseDate.year, 
               c.totalAmount);
    }

    printf("--------------------------------------------------------------------------\n");
    fclose(fp);
    getchar();
    getchar();
}

void searchBill() {
    struct Customer c;
    char id[10];
    int found = 0;
    FILE *fp = fopen("bills.dat", "rb");

    if (fp == NULL) {
        printf("\nNo records found.");
        return;
    }

    printf("\nEnter Customer ID to search: ");
    scanf("%s", id);

    while (fread(&c, sizeof(struct Customer), 1, fp)) {
        if (strcmp(c.customerID, id) == 0) { // strcmp returns 0 if they match
            found = 1;
            printf("\n--- Detailed Invoice for %s ---", c.name);
            printf("\nAddress: %s", c.address);
            printf("\nContact: %s", c.contact);
            printf("\nDate: %02d/%02d/%d", c.purchaseDate.day, c.purchaseDate.month, c.purchaseDate.year);
            printf("\n---------------------------------");
            printf("\n%-20s %-10s %-10s", "Item", "Qty", "Price");
            
            for(int i = 0; i < c.itemCount; i++) {
                printf("\n%-20s %-10d %-10.2f", c.items[i].itemName, c.items[i].quantity, c.items[i].unitPrice);
            }
            
            printf("\n---------------------------------");
            printf("\nTotal Amount (incl. 19%% VAT): %.2f€\n", c.totalAmount);
        }
    }

    if (!found) {
        printf("\nInvoice with ID %s not found.\n", id);
    }

    fclose(fp);

    // --- REFINED PAUSE LOGIC ---
    printf("\nPress Enter twice to return to Main Menu...");
    getchar(); // This handles the newline left by the 'scanf' for the ID
    getchar(); // This actually waits for your Enter key
}

void editBill() {
    struct Customer c;
    char id[10];
    int found = 0;
    FILE *fp = fopen("bills.dat", "rb");
    FILE *ft = fopen("temp.dat", "wb"); // Temporary file

    if (fp == NULL) {
        printf("\nNo records found.");
        return;
    }

    printf("\nEnter Customer ID to Edit: ");
    scanf("%s", id);

    while (fread(&c, sizeof(struct Customer), 1, fp)) {
        if (strcmp(c.customerID, id) == 0) {
            found = 1;
            printf("\n--- Record Found. Enter New Details ---");
            
            printf("\nNew Customer Name: ");
            getchar(); // Clean buffer
            fgets(c.name, 50, stdin);
            c.name[strcspn(c.name, "\n")] = 0;

            printf("New Contact Number: ");
            scanf("%s", c.contact);
            
            // We keep the items and total the same for this edit, 
            // but you could add logic to change items here too!
        }
        // Write either the updated record or the original record to temp file
        fwrite(&c, sizeof(struct Customer), 1, ft);
    }

    fclose(fp);
    fclose(ft);

    if (found) {
        remove("bills.dat");        // Delete old file
        rename("temp.dat", "bills.dat"); // Rename temp to original
        printf("\nRecord updated successfully!");
    } else {
        remove("temp.dat"); // Clean up temp file
        printf("\nRecord not found.");
    }

    printf("\nPress Enter to return...");
    getchar(); getchar();
}

void deleteBill() {
    struct Customer c;
    char id[10];
    int found = 0;
    FILE *fp = fopen("bills.dat", "rb");
    FILE *ft = fopen("temp.dat", "wb");

    if (fp == NULL) {
        printf("\nNo records found.");
        return;
    }

    printf("\nEnter Customer ID to Delete: ");
    scanf("%s", id);

    while (fread(&c, sizeof(struct Customer), 1, fp)) {
        if (strcmp(c.customerID, id) == 0) {
            found = 1;
            printf("\nRecord for %s has been removed from the system.", c.name);
            // Notice: We do NOT write to 'ft' here. This effectively deletes it.
        } else {
            fwrite(&c, sizeof(struct Customer), 1, ft);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("bills.dat");
    rename("temp.dat", "bills.dat");

    if (!found) {
        printf("\nRecord with ID %s not found.", id);
    }

    printf("\nPress Enter to return...");
    getchar(); getchar();
}






int main()
 {
    int choice;

    do {
        showMenu();
        scanf("%d", &choice);
        
        // This clears the input buffer to prevent errors with strings later
        getchar(); 

        switch(choice) {
            case 1:
                addCustomer();
                break;
            case 2:
            
                displayAllBills();
                break;
            case 3:
                searchBill();
                break;
         case 4: { // Using curly braces here allows us to define local variables
                int subChoice;
                printf("\n--- Manage Records ---");
                printf("\n1. Edit a Bill");
                printf("\n2. Delete a Bill");
                printf("\n3. Back to Main Menu");
                printf("\nSelect an option: ");
                scanf("%d", &subChoice);
                getchar(); // Clean buffer

                if (subChoice == 1) {
                    editBill();
                } else if (subChoice == 2) {
                    deleteBill();
                }
                break;
            }
            case 5:
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 5);

    return 0;
}
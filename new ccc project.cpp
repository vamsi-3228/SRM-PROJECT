#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CREDENTIAL_FILE "credentials.txt"
#define STUDENT_FILE    "students.dat"

#define MAX_USERNAME  20
#define MAX_PASSWORD  20
#define MAX_ROLE      10

typedef struct {
    int  id;
    char name[30];  
    int  marks;
} Student;

char currentRole[MAX_ROLE];
char currentUser[MAX_USERNAME];

int  loginSystem(void);
void mainMenu(void);
void adminMenu(void);
void userMenu(void);
void guestMenu(void);
void staffMenu(void);

void addstudent(void);
void displaystudent(void);
void Searchstudent(void);
void updatestudent(void);
void deletestudent(void);

void printLine(char ch, int count);
void pressEnterToContinue(void);

int main(void) {
    int loggedIn = 0;
    int attempts = 0;
    const int maxAttempts = 3;

    printLine('=', 50);
    printf("      SIMPLE LOGIN & STUDENT MANAGEMENT SYSTEM\n");
    printLine('=', 50);

    while (!loggedIn && attempts < maxAttempts) {
        if (loginSystem()) {
            loggedIn = 1;
        } else {
            attempts++;
            if (attempts < maxAttempts) {
                printf("\n[!] Invalid username or password. Attempts left: %d\n\n",
                       maxAttempts - attempts);
            }
        }
    }

    if (loggedIn) {
        mainMenu();
    } else {
        printf("\n[!] Too many failed attempts. Exiting program...\n");
    }

    return 0;
}

void printLine(char ch, int count) {
    for (int i = 0; i < count; ++i) {
        putchar(ch);
    }
    putchar('\n');
}

void pressEnterToContinue(void) {
    int c;
    printf("\nPress ENTER to continue...");
    while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();
}

int loginSystem(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];

    char fileUser[MAX_USERNAME];
    char filePass[MAX_PASSWORD];
    char fileRole[MAX_ROLE];

    printf("\n============== LOGIN SCREEN ==============\n");
    printf("Enter Username : ");
    scanf("%19s", username);
    printf("Enter Password : ");
    scanf("%19s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (fp == NULL) {
        printf("\n[ERROR] Cannot open '%s'. Make sure the file exists.\n",
               CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%19s %19s %9s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 &&
            strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);

            fclose(fp);
            printf("\n[?] Login successful! Welcome, %s.\n", currentUser);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void mainMenu(void) {
    printLine('-', 50);
    printf("Logged in as: %s   [Role: %s]\n", currentUser, currentRole);
    printLine('-', 50);

    if (strcmp(currentRole, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(currentRole, "GUEST") == 0) {
        guestMenu();
    } else if (strcmp(currentRole, "STAFF") == 0) {
        staffMenu();
    } else {
        userMenu();
    }
}

void adminMenu(void) {
    int choice;

    while (1) {
        printf("\n================= ADMIN MENU =================\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Record by ID\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Record\n");
        printf("6. Logout\n");
        printLine('-', 45);
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addstudent();      break;
            case 2: displaystudent();  break;
            case 3: Searchstudent();   break;
            case 4: updatestudent();   break;
            case 5: deletestudent();   break;
            case 6:
                printf("\n[+] Logging out from ADMIN account...\n");
                return;
            default:
                printf("[!] Invalid choice (%d). Please choose between 1 and 6.\n",
                       choice);
        }
    }
}

void addstudent(void) {
    Student s;
    FILE *fp = fopen(STUDENT_FILE, "ab");
    if (fp == NULL) {
        printf("\n[ERROR] Could not open '%s' for writing.\n", STUDENT_FILE);
        return;
    }

    printf("\n---------- Add New Student ----------\n");
    printf("Enter Student ID: ");
    scanf("%d", &s.id);
    printf("Enter Name (no spaces): ");
    scanf("%29s", s.name);
    printf("Enter Marks: ");
    scanf("%d", &s.marks);

    if (fwrite(&s, sizeof(Student), 1, fp) == 1) {
        printf("\n[?] Student added successfully!\n");
    } else {
        printf("\n[ERROR] Failed to write student record.\n");
    }

    fclose(fp);
    pressEnterToContinue();
}

void displaystudent(void) {
    Student s;
    FILE *fp = fopen(STUDENT_FILE, "rb");
    if (fp == NULL) {
        printf("\n[INFO] No student records found yet.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n---------- All Student Records ----------\n");
    printLine('=', 50);
    printf("%-6s %-15s %-10s\n", "ID", "Name", "Marks");
    printLine('-', 50);

    int count = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-6d %-15s %-10d\n", s.id, s.name, s.marks);
        count++;
    }

    if (count == 0) {
        printf("\n[INFO] No records in file.\n");
    }

    fclose(fp);
    printLine('=', 50);
    pressEnterToContinue();
}

void Searchstudent(void) {
    int searchId;
    Student s;
    int found = 0;

    FILE *fp = fopen(STUDENT_FILE, "rb");
    if (fp == NULL) {
        printf("\n[INFO] No student records found.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n---------- Search Student ----------\n");
    printf("Enter Student ID to search: ");
    scanf("%d", &searchId);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == searchId) {
            printf("\n[?] Record found:\n");
            printLine('-', 40);
            printf("ID     : %d\n", s.id);
            printf("Name   : %s\n", s.name);
            printf("Marks  : %d\n", s.marks);
            printLine('-', 40);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\n[!] No record found with ID %d.\n", searchId);
    }

    fclose(fp);
    pressEnterToContinue();
}

void updatestudent(void) {
    int updateId;
    Student s;
    int found = 0;

    FILE *fp = fopen(STUDENT_FILE, "rb+");
    if (fp == NULL) {
        printf("\n[INFO] No student records found.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n---------- Update Student ----------\n");
    printf("Enter Student ID to update: ");
    scanf("%d", &updateId);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == updateId) {
            printf("\n[?] Current details:\n");
            printLine('-', 40);
            printf("ID     : %d\n", s.id);
            printf("Name   : %s\n", s.name);
            printf("Marks  : %d\n", s.marks);
            printLine('-', 40);

            printf("\nEnter NEW details:\n");
            printf("New Name (no spaces): ");
            scanf("%29s", s.name);
            printf("New Marks: ");
            scanf("%d", &s.marks);

            /* Move file pointer back by one Student record */
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            if (fwrite(&s, sizeof(Student), 1, fp) == 1) {
                printf("\n[?] Record updated successfully.\n");
            } else {
                printf("\n[ERROR] Failed to update record.\n");
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\n[!] No record found with ID %d.\n", updateId);
    }

    fclose(fp);
    pressEnterToContinue();
}

void deletestudent(void) {
    int deleteId;
    Student s;
    int found = 0;

    FILE *fp = fopen(STUDENT_FILE, "rb");
    if (fp == NULL) {
        printf("\n[INFO] No student records found.\n");
        pressEnterToContinue();
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("\n[ERROR] Could not create temporary file.\n");
        fclose(fp);
        return;
    }

    printf("\n---------- Delete Student ----------\n");
    printf("Enter Student ID to delete: ");
    scanf("%d", &deleteId);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == deleteId) {
            found = 1;
            continue;
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.dat", STUDENT_FILE);
        printf("\n[?] Record with ID %d deleted successfully.\n", deleteId);
    } else {
        remove("temp.dat");
        printf("\n[!] No record found with ID %d.\n", deleteId);
    }

    pressEnterToContinue();
}

/* ================= OTHER ROLES ================= */

void userMenu(void) {
    printf("\n[USER MENU]\n");
    printLine('-', 30);
    printf("Hello %s!\n", currentUser);
    printf("User-specific operations are not implemented yet.\n");
    printLine('-', 30);
}

void guestMenu(void) {
    printf("\n[GUEST MENU]\n");
    printLine('-', 30);
    printf("Welcome, %s.\n", currentUser);
    printf("As a GUEST you may have read-only access (not implemented yet).\n");
    printLine('-', 30);
}

void staffMenu(void) {
    printf("\n[STAFF MENU]\n");
    printLine('-', 30);
    printf("Hi %s!\n", currentUser);
    printf("Staff operations will be added in future versions.\n");
    printLine('-', 30);
}


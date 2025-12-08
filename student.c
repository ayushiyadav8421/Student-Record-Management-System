
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE  "students.txt"
#define CRE_FILE   "credentials.txt"

char currentUser[50];
char currentRole[20];

int login(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void deleteStudent(void);
void updateStudent(void);
void adminMenu(void);
void staffMenu(void);
void guestMenu(void);

int main(void) {
    printf("STUDENT MANAGEMENT SYSTEM\n");
    if (!login()) {
        printf("Invalid login! Exiting...\n");
        return 0;
    }
    printf("Logged in as role: %s\n", currentRole);
    if (strcmp(currentRole, "admin") == 0) adminMenu();
    else if (strcmp(currentRole, "staff") == 0) staffMenu();
    else guestMenu();
    return 0;
}

int login(void) {
    char fileUser[50], filePass[50], fileRole[20];
    char inUser[50], inPass[50];
    FILE *fp;
    printf("USERNAME: ");
    if (scanf("%49s", inUser) != 1) return 0;
    printf("PASSWORD: ");
    if (scanf("%49s", inPass) != 1) return 0;
    fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf("Credential file missing!\n");
        return 0;
    }
    while (fscanf(fp, "%49s %49s %19s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(inUser, fileUser) == 0 && strcmp(inPass, filePass) == 0) {
            strcpy(currentUser, fileUser);
            strcpy(currentRole, fileRole);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addStudent(void) {
    int roll;
    char name[50];
    float mark;
    FILE *fp = fopen(STUD_FILE, "a");
    if (!fp) {
        printf("Unable to open student file for appending.\n");
        return;
    }
    printf("Roll: ");
    if (scanf("%d", &roll) != 1) { fclose(fp); return; }
    printf("Name: ");
    getchar(); /* consume newline */
    if (!fgets(name, sizeof(name), stdin)) { fclose(fp); return; }
    /* remove trailing newline */
    name[strcspn(name, "\n")] = '\0';
    printf("Mark: ");
    if (scanf("%f", &mark) != 1) { fclose(fp); return; }
    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);
    printf("Student added!\n");
}

void displayStudents(void) {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) { printf("No student file found!\n"); return; }
    int roll;
    char name[50];
    float mark;
    printf("\nRoll\tName\tMark\n");
    printf("-------------------------\n");
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        printf("%d\t%s\t%.2f\n", roll, name, mark);
    }
    fclose(fp);
}

void searchStudent(void) {
    int targetRoll, roll;
    char name[50];
    float mark;
    printf("Enter roll to search: ");
    if (scanf("%d", &targetRoll) != 1) return;
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) { printf("No data found!\n"); return; }
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll == targetRoll) {
            printf("Found: %d %s %.2f\n", roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Student not found!\n");
}

void deleteStudent(void) {
    int delRoll, roll, found = 0;
    char name[50];
    float mark;
    printf("Enter roll to delete: ");
    if (scanf("%d", &delRoll) != 1) return;
    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) { printf("File error!\n"); if (fp) fclose(fp); if (temp) fclose(temp); return; }
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        else found = 1;
    }
    fclose(fp); fclose(temp);
    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);
    if (found) printf("Student deleted!\n"); else printf("Roll not found!\n");
}

void updateStudent(void) {
    int updateRoll, roll, found = 0;
    char name[50], newName[50];
    float mark, newMark;
    printf("Enter roll to update: ");
    if (scanf("%d", &updateRoll) != 1) return;
    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) { printf("File error!\n"); if (fp) fclose(fp); if (temp) fclose(temp); return; }
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            printf("New Name: ");
            getchar(); /* consume newline */
            if (!fgets(newName, sizeof(newName), stdin)) { strcpy(newName, name); }
            newName[strcspn(newName, "\n")] = '\0';
            printf("New Mark: ");
            if (scanf("%f", &newMark) != 1) newMark = mark;
            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }
    fclose(fp); fclose(temp);
    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);
    if (found) printf("Student updated!\n"); else printf("Roll not found!\n");
}

void adminMenu(void) {
    int c;
    while (1) {
        printf("\nADMIN MENU\n");
        printf("1. Add\n2. Display\n3. Search\n4. Update\n5. Delete\n6. Logout\nChoice: ");
        if (scanf("%d", &c) != 1) break;
        switch (c) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            default: return;
        }
    }
}

void staffMenu(void) {
    int c;
    while (1) {
        printf("\nSTAFF MENU\n");
        printf("1. Add\n2. Display\n3. Search\n4. Update\n5. Logout\nChoice: ");
        if (scanf("%d", &c) != 1) break;
        switch (c) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            default: return;
        }
    }
}

void guestMenu(void) {
    int c;
    while (1) {
        printf("\nGUEST MENU\n");
        printf("1. Display\n2. Search\n3. Logout\nChoice: ");
        if (scanf("%d", &c) != 1) break;
        switch (c) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            default: return;
        }
    }
}

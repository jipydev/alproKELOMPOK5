#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/* ======================
   BERSIHKAN LAYAR
   ====================== */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ======================
   JEDA LAYAR
   ====================== */
void pauseScreen() {
    printf("\nTekan ENTER untuk melanjutkan...");
    getchar();
}

/* ======================
   INPUT INTEGER ONLY
   ====================== */
int inputInt(const char *msg) {
    char buffer[50];

    while (1) {
        int valid = 1;
        printf("%s", msg);
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            printf("Input harus angka!\n");
            continue;
        }

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            return atoi(buffer);
        }

        printf("Input harus angka!\n");
    }
}

/* ======================
   INPUT FLOAT 
   ====================== */
float inputFloat(const char *msg) {
    char buffer[50];
    float value;
    char extra;

    while (1) {
        printf("%s", msg);
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%f %c", &value, &extra) == 1)
            return value;

        printf("Input harus angka desimal!\n");
    }
}

/* ======================
   INPUT STRING
   ====================== */
void inputString(const char *msg, char *buffer, int size) {
    printf("%s", msg);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

/* ======================
   VALIDASI TANGGAL TANGGAL
   ====================== */
int validTanggal(const char *tgl) {
    if (strlen(tgl) != 10) return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (tgl[i] != '-') return 0;
        } else {
            if (!isdigit(tgl[i])) return 0;
        }
    }
    return 1;
}

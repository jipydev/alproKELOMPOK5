#include <stdio.h>

int main() {
    int umur;
    char jk;
    float tinggi;
    float median, sd, z;

    printf("=== CEK STUNTING BALITA (WHO Z-SCORE) ===\n");
    printf("Jenis kelamin (L/P): ");
    scanf(" %c", &jk);

    printf("Umur balita (bulan): ");
    scanf("%d", &umur);

    printf("Tinggi badan (cm): ");
    scanf("%f", &tinggi);

    // Data WHO berdasarkan jenis kelamin
    if (jk == 'L' || jk == 'l') {
        if (umur <= 12) {
            median = 75.7; sd = 2.9;
        } else if (umur <= 24) {
            median = 87.1; sd = 3.2;
        } else if (umur <= 36) {
            median = 96.1; sd = 3.4;
        } else if (umur <= 48) {
            median = 103.3; sd = 3.7;
        } else {
            median = 110.0; sd = 4.0;
        }
    } else if (jk == 'P' || jk == 'p') {
        if (umur <= 12) {
            median = 74.0; sd = 2.8;
        } else if (umur <= 24) {
            median = 85.7; sd = 3.1;
        } else if (umur <= 36) {
            median = 95.1; sd = 3.3;
        } else if (umur <= 48) {
            median = 102.7; sd = 3.6;
        } else {
            median = 109.4; sd = 3.9;
        }
    } else {
        printf("Jenis kelamin tidak valid!\n");
        return 0;
    }

    // Hitung Z-score
    z = (tinggi - median) / sd;

    printf("\nMedian WHO : %.1f cm\n", median);
    printf("Z-Score   : %.2f\n", z);

    // Klasifikasi stunting
    if (z < -3) {
        printf("Status: SANGAT PENDEK (STUNTING BERAT)\n");
    } else if (z < -2) {
        printf("Status: PENDEK (STUNTING)\n");
    } else if (z <= 2) {
        printf("Status: NORMAL\n");
    } else {
        printf("Status: TINGGI\n");
    }

    return 0;
}

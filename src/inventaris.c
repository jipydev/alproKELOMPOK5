#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventaris.h"
#include "utils.h"

#define BATAS 100
#define FILE_INV  "inventaris.txt"

typedef struct 
{
    int id;
    char namaBarang[50];
    int stok;
}inventaris;

int generateId() {
    FILE *fp = fopen(FILE_INV, "r");
    if (!fp) return 1;

    inventaris i;
    int lastId = 0;

    while (fscanf(fp, "%d %s %d", &i.id, i.namaBarang, &i.stok) != BATAS) {
        if (i.id > lastId)
            lastId = i.id;
    }

    fclose(fp);
    return lastId + 1;
}

//Fitur tambah 
void tambahInv() {
    FILE *fp = fopen(FILE_INV, "r");
    int count = 0;
    inventaris temp;

    if (fp) {
        while (fscanf(fp, "%d %s %d", &temp.id, temp.namBarang, &temp.stok) != BATAS)
            count++;
        fclose(fp);
    }

    if (count >= BATAS) {
        printf("Inventory penuh!\n");
        return;
    }

    inventaris i;
    i.id = generateId();

    printf("Nama barang ): ");
    scanf("%s", i.stok);

    printf("Jumlah: ");
    scanf("%d", &i.stok);

    fp = fopen(FILE_INV, "a");
    fprintf(fp, "%d %s %d\n", i.id, i.namaBarang, i.stok);
    fclose(fp);

}







//Menu utama 
void menuInventaris(){

printf("|=========================|\n");
printf("|       Inventory         |\n");
printf("|=========================|\n");
printf("|1.Lihat Barang           |\n");
printf("|-------------------------|\n");
printf("|2.Tambah Barang          |\n");
printf("|-------------------------|\n");
printf("|3.Edit Barang            |\n");
printf("|-------------------------|\n");
printf("|4.Cari Barang            |\n");
printf("|-------------------------|\n");
printf("|5.Hapus Barang           |\n");
printf("|-------------------------|\n");
}








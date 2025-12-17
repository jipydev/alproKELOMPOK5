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
        while (fscanf(fp, "%d %s %d", &temp.id, temp.namaBarang, &temp.stok) != BATAS)
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
// Tampilkan semua data
void tampilkanSemuaInv() {
    FILE *fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Belum ada data.\n");
        return;
    }

    inventaris i;
    printf("\n=== DATA INVENTARIS ===\n");

   for (int x ; x<BATAS ; x++) {
    int cek = fscanf(fp, "%d %s %d",
                     &i.id, i.namaBarang, &i.stok);

    if (cek != 3) {
        break; 
    }
        printf("ID:%d | %s | %d pcs\n", i.id, i.namaBarang, i.stok);
    }

    fclose(fp);
}
void editInv(){

    FILE *fp, *temp;
    inventaris i;
    int idCari;
    int found = 0;

    printf("Masukkan ID yang ingin diedit: ");
    scanf("%d", &idCari);

    fp = fopen("db.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    for (int x = 0; x < BATAS; x++) {

        if (fscanf(fp, "%d %s %d",
                   &i.id, i.barang, &i.jumlah) != 3) {
            break; 
        }

        if (i.id == idCari) {
            printf("Data ditemukan!\n");
            printf("Nama baru: ");
            scanf("%s", i.barang);
            printf("Jumlah baru: ");
            scanf("%d", &i.jumlah);



            
            found = 1;
        }

        fprintf(temp, "%d %s %d\n",
                i.id, i.barang, i.jumlah);
    }

    fclose(fp);
    fclose(temp);

    remove("db.txt");
    rename("temp.txt", "db.txt");

    if (found)
        printf("Data berhasil diedit!\n");
    else
        printf("ID tidak ditemukan!\n");
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








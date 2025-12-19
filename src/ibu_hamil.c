#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ibu_hamil.h"
#include "utils.h"

#define FILE_IBU "db/data_ibu_hamil.txt"
#define FILE_TMP "db/tmp_ibu_hamil.txt"
#define MAX_DATA 100

/* =======================
   STRUCT DATA
   ======================= */
typedef struct {
    int id;
    char nama[50];
    int usiaKehamilan;   // minggu
    float beratBadan;   // kg
    char resiko[30];
} IbuHamil;

/* =======================
   HITUNG RESIKO
   ======================= */
void tentukanResiko(IbuHamil *i) {
    if (i->usiaKehamilan < 12 || i->beratBadan < 45)
        strcpy(i->resiko, "TINGGI");
    else
        strcpy(i->resiko, "NORMAL");
}

/* =======================
   TAMBAH DATA
   ======================= */
void tambahIbuHamil() {
    clearScreen();
    printf("=== TAMBAH DATA IBU HAMIL ===\n");

    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_IBU, "r");
    IbuHamil i;

    /* Cegah ID duplikat */
    if (f) {
        while (fscanf(f, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                      &i.id, i.nama,
                      &i.usiaKehamilan,
                      &i.beratBadan,
                      i.resiko) != EOF) {
            if (i.id == id) {
                printf("ID sudah terdaftar!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

    f = fopen(FILE_IBU, "a");
    if (!f) return;

    i.id = id;
    inputString("Nama Ibu            : ", i.nama, sizeof(i.nama));
    i.usiaKehamilan = inputInt("Usia Kehamilan (mg): ");
    i.beratBadan = inputFloat("Berat Badan (kg)  : ");

    tentukanResiko(&i);

    fprintf(f, "%d|%s|%d|%.2f|%s\n",
            i.id, i.nama,
            i.usiaKehamilan,
            i.beratBadan,
            i.resiko);

    fclose(f);
    printf("Data ibu hamil berhasil ditambahkan!\n");
    pauseScreen();
}

/* =======================
   TAMPILKAN SEMUA
   ======================= */
void tampilkanSemuaIbuHamil() {
    clearScreen();
    FILE *f = fopen(FILE_IBU, "r");
    IbuHamil data[MAX_DATA];
    int n = 0;

    if (!f) {
        printf("Belum ada data ibu hamil.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                  &data[n].id,
                  data[n].nama,
                  &data[n].usiaKehamilan,
                  &data[n].beratBadan,
                  data[n].resiko) != EOF) {
        n++;
    }
    fclose(f);

    /* Bubble Sort by Nama */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(data[j].nama, data[j + 1].nama) > 0) {
                IbuHamil tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }

printf("\n===================== DATA IBU HAMIL ==============================\n");
printf("-------------------------------------------------------------------\n");
printf("| %-3s | %-15s | %-10s | %-10s | %-10s    |\n",
       "ID", "Nama", "Usia (mg)", "Berat (kg)", "RESIKO");
printf("------------------------------------------------------------------\n");

for (int i = 0; i < n; i++) {
printf("| %-3d | %-15s | %-10d | %-10.2f | %-10s |\n",
           data[i].id,
           data[i].nama,
           data[i].usiaKehamilan,
           data[i].beratBadan,
           data[i].resiko);
}

printf("------------------------------------------------------------------\n");

    pauseScreen();
}

/* =======================
   CARI BY ID
   ======================= */
void cariIbuHamilById() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = kembali): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_IBU, "r");
    IbuHamil i;
    int found = 0;

    if (!f) {
        printf("Data kosong.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                  &i.id, i.nama,
                  &i.usiaKehamilan,
                  &i.beratBadan,
                  i.resiko) != EOF) {
        if (i.id == id) {
            printf("\nDATA DITEMUKAN\n");
            printf("Nama           : %s\n", i.nama);
            printf("Usia Kehamilan : %d minggu\n", i.usiaKehamilan);
            printf("Berat Badan    : %.2f kg\n", i.beratBadan);
            printf("Resiko         : %s\n", i.resiko);
            found = 1;
            break;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   CARI BY NAMA
   ======================= */
void cariIbuHamilByNama() {
    clearScreen();
    char key[50];
    inputString("Masukkan nama: ", key, sizeof(key));

    FILE *f = fopen(FILE_IBU, "r");
    IbuHamil i;
    int found = 0;

    if (!f) {
        printf("Data kosong.\n");
        pauseScreen();
        return;
    }

    printf("\nHASIL PENCARIAN\n");
    while (fscanf(f, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                  &i.id, i.nama,
                  &i.usiaKehamilan,
                  &i.beratBadan,
                  i.resiko) != EOF) {
        if (strstr(i.nama, key)) {
            printf("ID:%d | %s | %s\n",
                   i.id, i.nama, i.resiko);
            found = 1;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   SUBMENU LIHAT
   ======================= */
void lihatIbuHamil() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Data Ibu Hamil    |\n");
printf("|=========================|\n");
printf("|1.Tampilkan Semua        |\n");
printf("|-------------------------|\n");
printf("|2.Cari By ID             |\n");
printf("|-------------------------|\n");
printf("|3.Cari By Nama           |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaIbuHamil();
        else if (p == 2) cariIbuHamilById();
        else if (p == 3) cariIbuHamilByNama();

    } while (p != 0);
}

/* =======================
   EDIT DATA
   ======================= */
void editIbuHamil() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_IBU, "r");
    FILE *tmp = fopen(FILE_TMP, "w");
    IbuHamil i;
    int found = 0;

    if (!f || !tmp) return;

    while (fscanf(f, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                  &i.id, i.nama,
                  &i.usiaKehamilan,
                  &i.beratBadan,
                  i.resiko) != EOF) {
        if (i.id == id) {
            found = 1;
            inputString("Nama baru            : ", i.nama, sizeof(i.nama));
            i.usiaKehamilan = inputInt("Usia Kehamilan baru  : ");
            i.beratBadan = inputFloat("Berat Badan baru (kg): ");
            tentukanResiko(&i);
        }
        fprintf(tmp, "%d|%s|%d|%.2f|%s\n",
                i.id, i.nama,
                i.usiaKehamilan,
                i.beratBadan,
                i.resiko);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_IBU);
    rename(FILE_TMP, FILE_IBU);

    printf(found ? "Data berhasil diubah!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =======================
   HAPUS DATA
   ======================= */
void hapusIbuHamil() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_IBU, "r");
    FILE *tmp = fopen(FILE_TMP, "w");
    IbuHamil i;
    int found = 0;

    if (!f || !tmp) return;

    while (fscanf(f, "%d|%49[^|]|%d|%f|%29[^\n]\n",
                  &i.id, i.nama,
                  &i.usiaKehamilan,
                  &i.beratBadan,
                  i.resiko) != EOF) {
        if (i.id == id) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d|%s|%d|%.2f|%s\n",
                i.id, i.nama,
                i.usiaKehamilan,
                i.beratBadan,
                i.resiko);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_IBU);
    rename(FILE_TMP, FILE_IBU);

    printf(found ? "Data berhasil dihapus!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =======================
   MENU UTAMA
   ======================= */
void menuIbuHamil() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Data Ibu Hamil    |\n");
printf("|=========================|\n");
printf("|1.Tambah Data            |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Data             |\n");
printf("|-------------------------|\n");
printf("|3.Edit Data              |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Data             |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahIbuHamil();
        else if (p == 2) lihatIbuHamil();
        else if (p == 3) editIbuHamil();
        else if (p == 4) hapusIbuHamil();

    } while (p != 0);
}

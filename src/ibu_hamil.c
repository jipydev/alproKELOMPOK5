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
    int umur;              // tahun
    int usiaKehamilan;     // minggu
    float beratBadan;      // kg
    float tinggiBadan;     // cm
    float hb;              // g/dL
    int sistol;            // mmHg
    int diastol;           // mmHg
    float bmi;
    char statusBMI[30];
    char statusHB[30];
    char resiko[40];       // ringkas
    char detailResiko[200]; // detail risiko
} IbuHamil;

float hitungBMI(float bb, float tb_cm) {
    float tb_m = tb_cm / 100.0f;
    if (tb_m <= 0.0f) return 0.0f;
    return bb / (tb_m * tb_m);
}

void tentukanStatusBMI(float bmi, char *out) {
    if (bmi < 18.5f) strcpy(out, "Underweight");
    else if (bmi < 25.0f) strcpy(out, "Normal");
    else if (bmi < 30.0f) strcpy(out, "Overweight");
    else strcpy(out, "Obese");
}

void tentukanStatusHB(float hb, char *out) {
    if (hb < 11.0f) strcpy(out, "Anemia");
    else if (hb > 15.0f) strcpy(out, "HB Tinggi");
    else strcpy(out, "Normal");
}

/* =======================
   RESIKO ALL
   ======================= */
void tentukanResiko(IbuHamil *i) {
    int skor = 0;
    int adaRisiko = 0;
    char hasil[200] = "";

    /* BMI */
    if (strcmp(i->statusBMI, "Underweight") == 0 ||
        strcmp(i->statusBMI, "Obese") == 0) {
        skor++;
        strcat(hasil, "RISIKO BMI; ");
        adaRisiko = 1;
    }

    /* HB */
    if (strcmp(i->statusHB, "Anemia") == 0) {
        skor++;
        strcat(hasil, "ANEMIA; ");
        adaRisiko = 1;
    }

    /* Usia Kehamilan */
    if (i->usiaKehamilan < 12) {
        skor++;
        strcat(hasil, "KEHAMILAN TRIMESTER AWAL; ");
        adaRisiko = 1;
    }

    /*  UMUR  */
    if (i->umur < 20) {
        strcat(hasil, "RISIKO KEHAMILAN USIA MUDA; ");
        adaRisiko = 1;
    } else if (i->umur >= 35) {
        strcat(hasil, "RISIKO KEHAMILAN USIA LANJUT; ");
        adaRisiko = 1;
    }

    /*  TEKANAN DARAH BERBASIS UMUR  */
    if (i->umur < 20) {
        if (i->sistol < 90 || i->diastol < 60)
            strcat(hasil, "HIPOTENSI; "), adaRisiko = 1;
        else if (i->sistol >= 130 || i->diastol >= 85)
            strcat(hasil, "HIPERTENSI; "), adaRisiko = 1;
    } else if (i->umur <= 34) {
        if (i->sistol < 95 || i->diastol < 65)
            strcat(hasil, "HIPOTENSI; "), adaRisiko = 1;
        else if (i->sistol >= 140 || i->diastol >= 90)
            strcat(hasil, "HIPERTENSI; "), adaRisiko = 1;
    } else {
        if (i->sistol < 100 || i->diastol < 70)
            strcat(hasil, "HIPOTENSI; "), adaRisiko = 1;
        else if (i->sistol >= 150 || i->diastol >= 95)
            strcat(hasil, "HIPERTENSI; "), adaRisiko = 1;
    }

    if (skor >= 3) strcpy(i->resiko, "RESIKO TINGGI");
    else if (skor >= 1) strcpy(i->resiko, "RESIKO SEDANG");
    else strcpy(i->resiko, "NORMAL");

    if (!adaRisiko) strcpy(hasil, "TIDAK ADA RISIKO KHUSUS");
    strcpy(i->detailResiko, hasil);
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

    if (f) {
        while (fscanf(f,
            "%d|%49[^|]|%d|%d|%f|%f|%f|%d|%d|%f|%29[^|]|%29[^|]|%39[^|]|%199[^\n]",
            &i.id, i.nama, &i.umur, &i.usiaKehamilan,
            &i.beratBadan, &i.tinggiBadan,
            &i.hb, &i.sistol, &i.diastol, &i.bmi,
            i.statusBMI, i.statusHB, i.resiko, i.detailResiko) == 14) {
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
    i.tinggiBadan = inputFloat("Tinggi Badan (cm): ");
    i.hb = inputFloat("HB (g/dL)         : ");
    i.umur = inputInt("Umur Ibu (thn)     : ");
    i.sistol = inputInt("Tekanan Sistol    : ");
    i.diastol = inputInt("Tekanan Diastol   : ");


    i.bmi = hitungBMI(i.beratBadan, i.tinggiBadan);
    tentukanStatusBMI(i.bmi, i.statusBMI);
    tentukanStatusHB(i.hb, i.statusHB);
    tentukanResiko(&i);

    fprintf(f,
        "%d|%s|%d|%d|%.2f|%.2f|%.2f|%d|%d|%.2f|%s|%s|%s|%s\n",
        i.id, i.nama, i.umur, i.usiaKehamilan,
        i.beratBadan, i.tinggiBadan, i.hb,
        i.sistol, i.diastol, i.bmi,
        i.statusBMI, i.statusHB, i.resiko, i.detailResiko);

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

while (fscanf(f,
 "%d|%49[^|]|%d|%d|%f|%f|%f|%d|%d|%f|%29[^|]|%29[^|]|%39[^|]|%199[^\n]",
 &data[n].id, data[n].nama, &data[n].umur, &data[n].usiaKehamilan,
 &data[n].beratBadan, &data[n].tinggiBadan,
 &data[n].hb, &data[n].sistol, &data[n].diastol, &data[n].bmi,
 data[n].statusBMI, data[n].statusHB,
 data[n].resiko, data[n].detailResiko
) == 14) {
    n++;
}

    fclose(f);

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (strcmp(data[j].nama, data[j + 1].nama) > 0) {
                IbuHamil tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }

    printf("=== DATA IBU HAMIL ===\n");
    for (int i = 0; i < n; i++) {
        printf(
                "ID:%d | %s | Umur:%d th | %d mg | TD %d/%d | BMI %.2f (%s) | HB %.2f (%s)\n"
                "Resiko: %s\n"
                "Detail: %s\n\n",
                data[i].id,
                data[i].nama,
                data[i].umur,
                data[i].usiaKehamilan,
                data[i].sistol,
                data[i].diastol,
                data[i].bmi,
                data[i].statusBMI,
                data[i].hb,
                data[i].statusHB,
                data[i].resiko,
                data[i].detailResiko
                );

    }

    pauseScreen();
}

/* =======================
   CARI ID
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

   while (fscanf(f,
 "%d|%49[^|]|%d|%d|%f|%f|%f|%d|%d|%f|%29[^|]|%29[^|]|%39[^|]|%199[^\n]",
 &i.id, i.nama, &i.umur, &i.usiaKehamilan,
 &i.beratBadan, &i.tinggiBadan,
 &i.hb, &i.sistol, &i.diastol, &i.bmi,
 i.statusBMI, i.statusHB, i.resiko, i.detailResiko
) == 14) {


        if (i.id == id) {
            printf("Nama: %s\nBMI: %.2f (%s)\nHB: %.2f (%s)\nResiko: %s\n",
                   i.nama, i.bmi, i.statusBMI, i.hb, i.statusHB, i.resiko);
            found = 1;
            break;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   CARI NAMA
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
    while (fscanf(f,
 "%d|%49[^|]|%d|%d|%f|%f|%f|%d|%d|%f|%29[^|]|%29[^|]|%39[^|]|%199[^\n]",
 &i.id, i.nama, &i.umur, &i.usiaKehamilan,
 &i.beratBadan, &i.tinggiBadan,
 &i.hb, &i.sistol, &i.diastol, &i.bmi,
 i.statusBMI, i.statusHB, i.resiko, i.detailResiko
) == 14) {


        if (strstr(i.nama, key)) {
            printf("ID:%d | %s | %s\n", i.id, i.nama, i.resiko);
            found = 1;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
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

    while (fscanf(f,
        "%d|%49[^|]|%d|%d|%f|%f|%f|%d|%d|%f|%29[^|]|%29[^|]|%39[^|]|%199[^\n]"
,
        &i.id, i.nama, &i.usiaKehamilan,
        &i.beratBadan, &i.tinggiBadan,
        &i.hb, &i.bmi,
        i.statusBMI, i.statusHB, i.resiko) == 10) {

        if (i.id == id) {
            found = 1;
            inputString("Nama baru            : ", i.nama, sizeof(i.nama));
            i.usiaKehamilan = inputInt("Usia Kehamilan baru  : ");
            i.beratBadan = inputFloat("Berat Badan baru (kg): ");
            i.tinggiBadan = inputFloat("Tinggi Badan baru(cm): ");
            i.hb = inputFloat("HB baru (g/dL)       : ");
            i.umur = inputInt("Umur baru (thn)     : ");
            i.sistol = inputInt("Sistol baru (mmHg) : ");
            i.diastol = inputInt("Diastol baru(mmHg) : ");


            i.bmi = hitungBMI(i.beratBadan, i.tinggiBadan);
            tentukanStatusBMI(i.bmi, i.statusBMI);
            tentukanStatusHB(i.hb, i.statusHB);
            tentukanResiko(&i);
        }

        fprintf(tmp,
            "%d|%s|%d|%d|%.2f|%.2f|%.2f|%d|%d|%.2f|%s|%s|%s|%s\n",
            i.id, i.nama, i.umur, i.usiaKehamilan,
            i.beratBadan, i.tinggiBadan,
            i.hb, i.sistol, i.diastol, i.bmi,
            i.statusBMI, i.statusHB, i.resiko, i.detailResiko);
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

    while (fscanf(f,
        "%d|%49[^|]|%d|%d|%f|%f|%f|%d|%d|%f|%29[^|]|%29[^|]|%39[^|]|%199[^\n]",
        &i.id, i.nama, &i.umur, &i.usiaKehamilan,
        &i.beratBadan, &i.tinggiBadan,
        &i.hb, &i.sistol, &i.diastol, &i.bmi,
        i.statusBMI, i.statusHB, i.resiko, i.detailResiko) == 14) {

        if (i.id == id) {
            found = 1;
            continue;
        }

        fprintf(tmp,
            "%d|%s|%d|%d|%.2f|%.2f|%.2f|%d|%d|%.2f|%s|%s|%s|%s\n",
            i.id, i.nama, i.umur, i.usiaKehamilan,
            i.beratBadan, i.tinggiBadan,
            i.hb, i.sistol, i.diastol, i.bmi,
            i.statusBMI, i.statusHB, i.resiko, i.detailResiko);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_IBU);
    rename(FILE_TMP, FILE_IBU);

    printf(found ? "Data berhasil dihapus!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =======================
   SUBMENU & MENU UTAMA
   ======================= */
void lihatIbuHamil() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|     Data Ibu Hamil      |\n");
printf("|=========================|\n");
printf("|1.Lihat Semua            |\n");
printf("|-------------------------|\n");
printf("|2.Cari Data By Id        |\n");
printf("|-------------------------|\n");
printf("|3.Cari Data By Nama      |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaIbuHamil();
        else if (p == 2) cariIbuHamilById();
        else if (p == 3) cariIbuHamilByNama();
    } while (p != 0);
}

void menuIbuHamil() {
    int p;
    do {
        clearScreen();
        printf("=== MENU IBU HAMIL ===\n");
        printf("1. Tambah Data\n");
        printf("2. Lihat Data\n");
        printf("3. Edit Data\n");
        printf("4. Hapus Data\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahIbuHamil();
        else if (p == 2) lihatIbuHamil();
        else if (p == 3) editIbuHamil();
        else if (p == 4) hapusIbuHamil();
    } while (p != 0);
}
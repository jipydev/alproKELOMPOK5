#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rekam_medis.h"
#include "utils.h"

#define FILE_REKAM "db/rekam_medis.txt"
#define FILE_TEMP  "db/temp.txt"

/* ================= TAMBAH ================= */
void tambahRekam() {
    clearScreen();
    printf("=== TAMBAH REKAM MEDIS ===\n");

    int id = inputInt("Masukkan ID Rekam (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_REKAM, "r");
    Rekam r;

    /* Cek ID duplikat */
    if (f) {
        while (fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                       &r.id, r.nama, r.keluhan) != EOF) {
            if (r.id == id) {
                printf("ID sudah ada!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

    f = fopen(FILE_REKAM, "a");
    if (!f) return;

    r.id = id;
    inputString("Nama Pasien: ", r.nama, sizeof(r.nama));
    inputString("Keluhan: ", r.keluhan, sizeof(r.keluhan));

    fprintf(f, "%d|%s|%s\n", r.id, r.nama, r.keluhan);
    fclose(f);

    printf("Rekam medis berhasil ditambahkan!\n");
    pauseScreen();
}

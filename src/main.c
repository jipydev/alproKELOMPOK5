#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "data_warga.h"
#include "rekam_medis.h"
#include "cek_stunting.h"
#include "ibu_hamil.h"
#include "jadwal_kegiatan.h"
#include "inventaris.h"
#include "laporan_keuangan.h"

int main() {
    int pilih;

    while (1) {
        clearScreen();
        printf("===================================================\n");
        printf("SIPANDU (Sistem Informasi Posyandu Digital Terpadu)\n");
        printf("===================================================\n");
        printf("1. Data Warga\n");
        printf("2. Rekam Medis\n");
        printf("3. Cek Stunting Balita\n");
        printf("4. Cek Kesehatan Ibu Hamil\n");
        printf("5. Jadwal Kegiatan\n");
        printf("6. Inventaris\n");
        printf("7. Laporan Keuangan\n");
        printf("0. Keluar\n");

        pilih = inputInt("Pilih menu: ");

        switch (pilih) {
            case 1: menuWarga(); break;
            case 2: menuRekam(); break;
            case 3: menuStunting(); break;
            case 4: menuIbuHamil(); break;
            case 5: menuJadwal(); break;
            case 6: menuInventaris(); break;
            case 7: menuKeuangan(); break;
            case 0: exit(0);
            default:
                printf("Pilihan tidak ada!\n");
                pauseScreen();
        }
    }
}

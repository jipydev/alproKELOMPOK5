#include <stdio.h>
#include <string.h>
#define MAX 50
struct inventory

{
    int id;
    char jenis[20];
    char namaBarang[20];
    int stok;

};
struct inventory barang[MAX];
int jumlah = 0;
char y;
//nambah barang
void tambahBarang(){

printf ("Isi Dengan Seksama Ya\n");
printf("Masukan Jenis Barang (obat/alat) : ");
scanf("%s",barang[jumlah].jenis);
printf("Masukan Nama Barang : ");
scanf("%s",barang[jumlah].namaBarang);
printf("Masukan stok Barang : ");
scanf("%d",&barang[jumlah].stok);

jumlah++;
printf("Berhasil Ditambahkan :)\n");
printf("Tambahkan Lagi Y/T : ");
scanf(" %c",&y);

while (y!='T'){
printf ("Isi Dengan Seksama Ya\n");
printf("Masukan Jenis Barang (obat/alat) : ");
scanf("%s",barang[jumlah].jenis);
printf("Masukan Nama Barang : ");
scanf("%s",barang[jumlah].namaBarang);
printf("Masukan stok Barang : ");
scanf("%d",&barang[jumlah].stok);

jumlah++;
printf("Berhasil Ditambahkan :)\n");
printf("Tambahkan Lagi Y/T   : ");
scanf(" %c",&y);
}
}
//liat barang
void liatBarang(){
    int id= -1;
for ( int i= 0; i<jumlah; i++){

    printf(" \nId Barang    : %d \n",barang[i].id);
    printf(" Jenis Barang    : %s \n",barang[i].jenis);
    printf(" Jenis Nama Barang : %s \n",barang[i].namaBarang);
    printf(" Jenis Stok        : %d \n",barang[i].stok);
    
//Edit Barang
}
}
void editBarang(){
    int search;
  printf("Ketik Id Barang Yang Ingin Diubah: ");
scanf("%d",&search);
    for(int i=0 ; i<jumlah; i++){
        if (search == barang[i].id){
printf("Masukan Jenis Barang  Baru(obat/alat) : ");
scanf("%s",barang[i].jenis);
printf("Masukan Nama Barang Baru : ");
scanf("%s",barang[i].namaBarang);
printf("Masukan stok Barang Baru : ");
scanf("%d",&barang[i].stok);


        }

      }
    

}











int main(){


tambahBarang();
liatBarang();
editBarang();
}




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char isim[20];
    char soyisim[20];
    int yas;
    int maas;
    struct Node *next;
    struct Node *onceki;
} Node;

Node *ilk = NULL;

// Yardýmcý fonksiyonlar
Node *yeniDugumOlustur(char isim[], char soyisim[], int yas, int maas);
void ekleSiraliTum(Node *yeni);
void sil(char isim[], char soyisim[]);
void araVeYazdir(char isim[], char soyisim[]);
void listele();
void Listeleme();
void kurumBilgisi();
void personelKayit();

int listeBoyutu = 0; // Liste boyutunu global olarak tanýmladim

int main() 
{
    FILE *dosya = fopen("kayit.txt", "r");

    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return 1;
    }

    char isim[20], soyisim[20];
    int yas, maas;

    while (1) {
    	printf("\n1) Ekleme 2) Silme 3) Arama 4) Listeleme 5) Kurum Bilgisi 6) Personel Kayit 7) Cikis");
        printf("\nBu sayilardan birini seciniz: 1, 2, 3, 4, 5, 6, 7\n");
        int secim;
        scanf("%d", &secim);

        switch (secim) {
            case 1:
				printf("Bagli Listeye ekleme islemi yapilmistir. Asagidaki gibi gorunmektedir.\n\n");
    			while (fscanf(dosya, "%s %s %d %d", isim, soyisim, &yas, &maas) != EOF) {
       				Node *yeni = yeniDugumOlustur(isim, soyisim, yas, maas);
        			ekleSiraliTum(yeni);
        			listeBoyutu++; // Liste boyutunu güncelledim
    			}		
				listele();
				printf("\nIsimlerin A dan Zye siralanmis hali:\n");
				Listeleme();
			    fclose(dosya);		
			    break;
			    
            case 2: {
                char isim[20], soyisim[20];
                printf("Bir isim giriniz: ");
                scanf("%s", isim);
                printf("Bir soyisim giriniz: ");
                scanf("%s", soyisim);
                sil(isim, soyisim);
                Listeleme();
                break;
            }
            case 3: {
                char isim[20], soyisim[20];
                printf("Bir isim giriniz: ");
                scanf("%s", isim);
                printf("Bir soyisim giriniz: ");
                scanf("%s", soyisim);
                araVeYazdir(isim, soyisim);
                break;
            }
            case 4:
                Listeleme();
                break;
            case 5:
                kurumBilgisi();
                break;
            case 6:
                personelKayit();
            	break;
            case 7:
            	return 0;
            default:
                printf("Istenen sayilardan birini secmediniz!");
        }
    }
    return 0;
}

// Yeni bir düðüm oluþturdum
Node *yeniDugumOlustur(char isim[], char soyisim[], int yas, int maas) {
    Node *yeni = (Node *)malloc(sizeof(Node));
    strcpy(yeni->isim, isim);
    strcpy(yeni->soyisim, soyisim);
    yeni->yas = yas;
    yeni->maas = maas;
    yeni->next = NULL;
    return yeni;
}

//Düðümü yaþ, maaþ ve isim ve soyisime göre sýralý bir þekilde ekledim
//Eðer yaþlar ve maaþlar eþitse, isimler karþýlaþtýrýlýr. Eðer yeni elemanýn ismi, ilk elemanýn isminden alfabetik olarak önce geliyorsa, koþul doðrudur.
/*strcmp fonksiyonu, iki stringi karþýlaþtýrýr ve sonucunu döndürür. Eðer karþýlaþtýrýlan stringler birbirine eþitse, sonuç 0 olur. Eðer birinci string,
 ikinci stringden alfabetik olarak önce geliyorsa, sonuç negatif bir deðer olur. 
Eðer birinci string, ikinci stringden alfabetik olarak sonra geliyorsa, sonuç pozitif bir deðer olur.*/
void ekleSiraliTum(Node *yeni) {
    if (ilk == NULL) {
        ilk = yeni;
        yeni->next = ilk;
    } else if (yeni->yas < ilk->yas ||
               (yeni->yas == ilk->yas && yeni->maas < ilk->maas) ||
               (yeni->yas == ilk->yas && yeni->maas == ilk->maas && strcmp(yeni->isim, ilk->isim) < 0) ||
               (strcmp(yeni->isim, ilk->isim) == 0 && strcmp(yeni->soyisim, ilk->soyisim) < 0)) {
			  
        yeni->next = ilk;
        Node *gecici = ilk;
        while (gecici->next != ilk) {
            gecici = gecici->next;
        }
        gecici->next = yeni;
        ilk = yeni;
    } else {
        Node *gecici = ilk;
        while (gecici->next != ilk &&
               (yeni->yas > gecici->next->yas ||
                (yeni->yas == gecici->next->yas && yeni->maas > gecici->next->maas) ||
                (yeni->yas == gecici->next->yas && yeni->maas == gecici->next->maas &&
                 strcmp(yeni->isim, gecici->next->isim) > 0 ||
                 (strcmp(yeni->isim, gecici->next->isim) == 0 &&
                  strcmp(yeni->soyisim, gecici->next->soyisim) > 0)))) {
            gecici = gecici->next;
        }
        yeni->next = gecici->next;
        gecici->next = yeni;
    }
}

// Ýsim ve soyisime göre düðüm siler
void sil(char isim[], char soyisim[]) {
    Node *gecici = ilk, *onceki = NULL;

    while (gecici->next != ilk &&
           (strcmp(gecici->isim, isim)!= 0 || strcmp(gecici->soyisim, soyisim)!= 0)) {
        onceki = gecici;
        gecici = gecici->next;
    }

    if (strcmp(gecici->isim, isim) == 0 && strcmp(gecici->soyisim, soyisim) == 0) {
        if (onceki != NULL) {
            onceki->next = gecici->next;
        } else {
            ilk = gecici->next;
        }
        free(gecici);
        listeBoyutu--;
        printf("%s %s listeden silinmistir.\n", isim, soyisim);
    } 
	else {
        printf("%s %s kisi listede mevcut degildir!\n", isim, soyisim);
    }
}

// Ýsim ve soyisime göre düðüm aradim ve bilgilerini yazdýrdim
void araVeYazdir(char isim[], char soyisim[]) {
    Node *gecici = ilk;
    Node *gecici2 = ilk;
    float ortalamaMaas = 0;
    int toplamMaas = 0;

    while (gecici->next != ilk &&
           (strcmp(gecici->isim, isim) != 0 || strcmp(gecici->soyisim, soyisim) != 0)) {
        gecici = gecici->next;
    }

    if (strcmp(gecici->isim, isim) == 0 && strcmp(gecici->soyisim, soyisim) == 0) {
        
        while (gecici2->next != ilk) {
            toplamMaas += gecici2->maas;
            gecici2 = gecici2->next;
        }
        //printf("liste boyutu = %d\n",listeBoyutu);
        //printf("Toplam Maas = %d\n",toplamMaas);
        
        //Ortalama maaþ hesabi yaptým
        ortalamaMaas = (float)(toplamMaas/listeBoyutu);
        printf("Ortalama Maas: %.3f", ortalamaMaas);
        //printf(", Gelir durumu: %s gelirli\n", (gecici->maas > ortalamaMaas) ? "ust" : "alt");
        printf("\n%s %s listede mevcuttur. Yasi: %d, Maasi: %d, Gelir durumu: %s gelirli\n",
               gecici->isim, gecici->soyisim, gecici->yas, gecici->maas,
               (gecici->maas > ortalamaMaas) ? "ust" : "alt");
    } 
	else {
        printf("%s %s listede mevcut degildir.\n", isim, soyisim);
    }
}
// Kurum bilgisini hesapladým ve ekrana yazdýrdým
void kurumBilgisi() {
    if (ilk == NULL) {
        printf("Listede kayit bulunmamaktadir.\n");
        return;
    }
    Node *gecici = ilk;
    int toplamMaas = 0;

    // Her bir düðümün maaþ deðerini topladým
    do {
        toplamMaas += gecici->maas;
        gecici = gecici->next;
    }while (gecici != ilk);

    // Toplam personel sayýsýný ve maaþý ekrana yazdýrdim
    printf("Toplam Personel Maasi: %d TL\n", toplamMaas);
    printf("Ortalama Personel Maasi: %.2f TL\n", (float)toplamMaas / listeBoyutu);
    printf("Toplam Personel Sayisi: %d\n", listeBoyutu);
}
// Düðümleri A-Z'ye göre listeleyen fonksiyon
/*strcmp fonksiyonu, iki stringi karþýlaþtýrýr ve sonucunu döndürür. Eðer karþýlaþtýrýlan stringler birbirine eþitse, sonuç 0 olur. Eðer birinci string,
 ikinci stringden alfabetik olarak önce geliyorsa, sonuç negatif bir deðer olur. 
Eðer birinci string, ikinci stringden alfabetik olarak sonra geliyorsa, sonuç pozitif bir deðer olur.*/
void Listeleme() {
	        Node *gecici2 = ilk;
	        Node *gecici = ilk;
    if (ilk == NULL) {
        printf("Listede kayit bulunmamaktadir.\n");
        return;
    }
    // Düðümleri A-Z'ye göre sýraladým
    do {
        do {
            if (strcmp(gecici2->isim, gecici->isim) > 0 ||
                (strcmp(gecici2->isim, gecici->isim) == 0 &&
                 strcmp(gecici2->soyisim, gecici->soyisim) > 0)) { 
                // Swap
                char tempIsim[20];
                char tempSoyisim[20];
                int tempYas, tempMaas;
                
				//Gecici olarak gecici elemanýn bilgilerini sakla
                strcpy(tempIsim, gecici->isim);
                strcpy(tempSoyisim, gecici->soyisim);
                tempYas = gecici->yas;
                tempMaas = gecici->maas;
                
				// gecici elemanýn bilgilerini gecici2 elemanýna kopyala
                strcpy(gecici->isim, gecici2->isim);
                strcpy(gecici->soyisim, gecici2->soyisim);
                gecici->yas = gecici2->yas;
                gecici->maas = gecici2->maas;
                
				// gecici2 elemanýnýn bilgilerini temp elemanýna kopyala
                strcpy(gecici2->isim, tempIsim);
                strcpy(gecici2->soyisim, tempSoyisim);
                gecici2->yas = tempYas;
                gecici2->maas = tempMaas;
            }

            gecici2 = gecici2->next;
        } while (gecici2 != ilk);

        gecici = gecici->next;
    } while (gecici != ilk);

    // Sýralý listeyi yazdýrdým
    printf("\nIsim\t\tSoyisim\t\tYas\tMaas\n\n");
    gecici = ilk;
    do {
        printf("%s\t\t%s\t\t%d\t%d\n", gecici->isim, gecici->soyisim, gecici->yas, gecici->maas);
        gecici = gecici->next;
    }while (gecici != ilk);
}
// Listeyi ekrana yazdýrdým
void listele() {
	if (ilk == NULL) {
	printf("Listede kayit bulunmamaktadir.\n");
	return;
	}
	Node *gecici = ilk;
	do {
        printf("%s\t\t%s\t\t%d\t%d\n", gecici->isim, gecici->soyisim, gecici->yas, gecici->maas);
	    gecici = gecici->next;
	} while (gecici != ilk);
}

// Personel kayýtlarýný dosyaya yazdýrdým
void personelKayit() {
    if (ilk == NULL) {
        printf("Listede kayit bulunmamaktadir.\n");
        return;
    }

    FILE *dosya = fopen("personelKayit.txt", "w");
    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    Node *gecici = ilk;

    // Her bir düðümün bilgilerini dosyaya yazma
    do {
        fprintf(dosya, "%s %s %d %d\n", gecici->isim, gecici->soyisim, gecici->yas, gecici->maas);
        gecici = gecici->next;
    } while (gecici != ilk);

    fclose(dosya);
    printf("Personel kayitlari 'personelKayit.txt' dosyasina yazdirilmistir.\n");
}

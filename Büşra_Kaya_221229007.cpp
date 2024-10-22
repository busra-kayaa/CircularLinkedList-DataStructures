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

// Yard�mc� fonksiyonlar
Node *yeniDugumOlustur(char isim[], char soyisim[], int yas, int maas);
void ekleSiraliTum(Node *yeni);
void sil(char isim[], char soyisim[]);
void araVeYazdir(char isim[], char soyisim[]);
void listele();
void Listeleme();
void kurumBilgisi();
void personelKayit();

int listeBoyutu = 0; // Liste boyutunu global olarak tan�mladim

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
        			listeBoyutu++; // Liste boyutunu g�ncelledim
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

// Yeni bir d���m olu�turdum
Node *yeniDugumOlustur(char isim[], char soyisim[], int yas, int maas) {
    Node *yeni = (Node *)malloc(sizeof(Node));
    strcpy(yeni->isim, isim);
    strcpy(yeni->soyisim, soyisim);
    yeni->yas = yas;
    yeni->maas = maas;
    yeni->next = NULL;
    return yeni;
}

//D���m� ya�, maa� ve isim ve soyisime g�re s�ral� bir �ekilde ekledim
//E�er ya�lar ve maa�lar e�itse, isimler kar��la�t�r�l�r. E�er yeni eleman�n ismi, ilk eleman�n isminden alfabetik olarak �nce geliyorsa, ko�ul do�rudur.
/*strcmp fonksiyonu, iki stringi kar��la�t�r�r ve sonucunu d�nd�r�r. E�er kar��la�t�r�lan stringler birbirine e�itse, sonu� 0 olur. E�er birinci string,
 ikinci stringden alfabetik olarak �nce geliyorsa, sonu� negatif bir de�er olur. 
E�er birinci string, ikinci stringden alfabetik olarak sonra geliyorsa, sonu� pozitif bir de�er olur.*/
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

// �sim ve soyisime g�re d���m siler
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

// �sim ve soyisime g�re d���m aradim ve bilgilerini yazd�rdim
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
        
        //Ortalama maa� hesabi yapt�m
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
// Kurum bilgisini hesaplad�m ve ekrana yazd�rd�m
void kurumBilgisi() {
    if (ilk == NULL) {
        printf("Listede kayit bulunmamaktadir.\n");
        return;
    }
    Node *gecici = ilk;
    int toplamMaas = 0;

    // Her bir d���m�n maa� de�erini toplad�m
    do {
        toplamMaas += gecici->maas;
        gecici = gecici->next;
    }while (gecici != ilk);

    // Toplam personel say�s�n� ve maa�� ekrana yazd�rdim
    printf("Toplam Personel Maasi: %d TL\n", toplamMaas);
    printf("Ortalama Personel Maasi: %.2f TL\n", (float)toplamMaas / listeBoyutu);
    printf("Toplam Personel Sayisi: %d\n", listeBoyutu);
}
// D���mleri A-Z'ye g�re listeleyen fonksiyon
/*strcmp fonksiyonu, iki stringi kar��la�t�r�r ve sonucunu d�nd�r�r. E�er kar��la�t�r�lan stringler birbirine e�itse, sonu� 0 olur. E�er birinci string,
 ikinci stringden alfabetik olarak �nce geliyorsa, sonu� negatif bir de�er olur. 
E�er birinci string, ikinci stringden alfabetik olarak sonra geliyorsa, sonu� pozitif bir de�er olur.*/
void Listeleme() {
	        Node *gecici2 = ilk;
	        Node *gecici = ilk;
    if (ilk == NULL) {
        printf("Listede kayit bulunmamaktadir.\n");
        return;
    }
    // D���mleri A-Z'ye g�re s�ralad�m
    do {
        do {
            if (strcmp(gecici2->isim, gecici->isim) > 0 ||
                (strcmp(gecici2->isim, gecici->isim) == 0 &&
                 strcmp(gecici2->soyisim, gecici->soyisim) > 0)) { 
                // Swap
                char tempIsim[20];
                char tempSoyisim[20];
                int tempYas, tempMaas;
                
				//Gecici olarak gecici eleman�n bilgilerini sakla
                strcpy(tempIsim, gecici->isim);
                strcpy(tempSoyisim, gecici->soyisim);
                tempYas = gecici->yas;
                tempMaas = gecici->maas;
                
				// gecici eleman�n bilgilerini gecici2 eleman�na kopyala
                strcpy(gecici->isim, gecici2->isim);
                strcpy(gecici->soyisim, gecici2->soyisim);
                gecici->yas = gecici2->yas;
                gecici->maas = gecici2->maas;
                
				// gecici2 eleman�n�n bilgilerini temp eleman�na kopyala
                strcpy(gecici2->isim, tempIsim);
                strcpy(gecici2->soyisim, tempSoyisim);
                gecici2->yas = tempYas;
                gecici2->maas = tempMaas;
            }

            gecici2 = gecici2->next;
        } while (gecici2 != ilk);

        gecici = gecici->next;
    } while (gecici != ilk);

    // S�ral� listeyi yazd�rd�m
    printf("\nIsim\t\tSoyisim\t\tYas\tMaas\n\n");
    gecici = ilk;
    do {
        printf("%s\t\t%s\t\t%d\t%d\n", gecici->isim, gecici->soyisim, gecici->yas, gecici->maas);
        gecici = gecici->next;
    }while (gecici != ilk);
}
// Listeyi ekrana yazd�rd�m
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

// Personel kay�tlar�n� dosyaya yazd�rd�m
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

    // Her bir d���m�n bilgilerini dosyaya yazma
    do {
        fprintf(dosya, "%s %s %d %d\n", gecici->isim, gecici->soyisim, gecici->yas, gecici->maas);
        gecici = gecici->next;
    } while (gecici != ilk);

    fclose(dosya);
    printf("Personel kayitlari 'personelKayit.txt' dosyasina yazdirilmistir.\n");
}

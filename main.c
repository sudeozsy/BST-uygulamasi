// SUDE OZSOY  -  22100011074
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct tree
{
    int no;
    char ad[50];
    struct tree *right;
    struct tree *left;
};
struct tree* agac = NULL;
struct tree* iter = NULL;

int sayac=0,top=0,yuks=0;  //sayac, toplam kisiyi tutuyor. Top, ortalama icin toplami hesapliyor
// yuks, yukseklik hesaplarken her uzakliktaki maximum uzakligi atiyorum.
struct tree* ekle (struct tree* agac,char ad[],int no)
{
    if(agac == NULL)  //baktigi yerde hic yaprak yoksa ekler.
    {
        struct tree *newl= (struct tree*)malloc(sizeof(struct tree));
        newl->left =NULL;
        newl->right =NULL;
        strcpy(newl->ad, ad);
        newl->no = no;
        sayac++;
        return newl;
    }
    if(agac->no <= no)  //eklenecek no agactaki no'dan buyuk veya esitse agacin sagini tekrar cagirir ve sagina atar.
    {
        agac->right = ekle(agac->right,ad,no);
        return agac;
    }
    agac->left = ekle(agac->left,ad,no);
    return agac;
}
void listele(struct tree* agac)
{
    if(agac == NULL)
        return;
    listele(agac->left);
    printf("  %s - %d\n",agac->ad,agac->no);
    listele(agac->right);
}
void ortalama(struct tree* agac)
{
    if(agac == NULL)
        return;
    top += agac->no;   // her elemanin nosunu globaldeki top degiskenine atar.
    ortalama(agac->left);
    ortalama(agac->right);
}
void yukseklik(struct tree* agac,int icyuks)
{
    if(agac == NULL)
    {
        if(icyuks > yuks)  //hesapladigi yukseklik globaldeki yukseklikten yuksekse globaldekine atar.
        {
            yuks = icyuks;
        }
        return;
    }
    icyuks++;  //her baktigi elemanda agac NULL donenen kadar yuksekligi bir artirir.
    yukseklik(agac->left,icyuks);
    yukseklik(agac->right,icyuks);
}
char* max(struct tree* agac)  //baktigi agactaki max no'yu hesaplar.
{
    while(agac ->right != NULL)
        agac = agac->right;
    return agac->ad;
}
char* min(struct tree* agac)  //baktigi agactaki min no'yu hesaplar.
{
    while(agac ->left != NULL)
        agac = agac->left;
    return agac->ad;
}
struct tree* silme(struct tree* agac,int aranan)
{
    char ad[50];
    if(agac == NULL)
    {
        sayac--;
        return NULL;
    }

    if(agac->no == aranan)
    {
        if(agac->left ==NULL && agac->right== NULL)  //altında hic yaprak yoksa siler.
        {
            agac = silme(agac->left,aranan);
            return agac;
        }
        if(agac->right != NULL)
        {
            strcpy(agac->ad, min(agac->right));  // agactaki min degeri alir ve silinenin yerine koyar
            agac->no = arama(agac->right,min(agac->right));  //arama fonksiyonuna adini atayarak no'sunu bulur ve silinen yere atanır.
            agac->right = silme(agac->right,arama(agac->right,min(agac->right)));  //min degerin oldugu yaprak silinmesi icin silme fonks tekrar cagirilir.
            return agac;
        }
        strcpy(agac->ad, max(agac->left));
        agac->no = arama(agac->left,max(agac->left));
        agac->left = silme(agac->left,arama(agac->left,max(agac->left)));
        return agac;
    }
    else if(agac->no < aranan)
    {
        agac->right = silme(agac->right,aranan);
        return agac;
    }
    else
    {
        agac->left = silme(agac->left,aranan);
        return agac;
    }

}
int arama(struct tree* agac,char ad[])  //aranan isim girilir o kisinin no'su dondurulur.
{
    if(agac == NULL)
        return -1;
    if(strcmp(agac->ad,ad) == 0)
    {
        return agac->no;
    }
    if(arama(agac->right,ad) != -1)
    {
        return arama(agac->right,ad);
    }
    if(arama(agac->left,ad)  != -1)
    {
        return arama(agac->left,ad);
    }
    return -1;
}
void ayniBul(struct tree* agac1)
{
    if(agac1 == NULL)
        return;

    ayniBul2(agac,agac1);  //ikinci recursive fonks cagirilir ve burda baktigi degerle tum degerler karsilastirilir.
    ayniBul(agac1->left);
    ayniBul(agac1->right);
}
void ayniBul2(struct tree* agac2,struct tree* kontrol)  //kontrol hic degismez onceki fonksiyondaki eleman tutulur.
{
    if(agac2 == NULL)
        return;

    if(strcmp(agac2->ad,kontrol->ad)==0 && kontrol->no != agac2->no)
    {  //onceki fonksiyondaki elemanla suan bakilanin esitligi sorgulanir,bi daha aynisini yazdirmamak icin no kontrolu de yapilir.
        printf("\t%s - %d ",agac2->ad,agac2->no);
        printf("ayni: %s - %d\n",kontrol->ad,kontrol->no);
        agac = silme(agac,agac2->no);  //ayni isimdekiler silinir.
        agac = silme(agac,kontrol->no);
    }
    ayniBul2(agac2->left,kontrol);
    ayniBul2(agac2->right,kontrol);
}
int main()
{
    int no,secim,arananno;
    char ad[50],aranan[50];

    FILE *bilgi;
    bilgi = fopen("tree.txt","r");
    while(!feof(bilgi))
    {
        fscanf(bilgi,"%s",ad);
        fscanf(bilgi,"%d",&no);
        agac=ekle(agac,ad,no); //dosya okunur ve ekleme fonksiyonu cagirilir.
    }
    fclose(bilgi);
tekrar:
    printf("\n1-Ekleme\n"
           "2-Silme(Ad'a gore)\n"
           "3-Listeleme\n"
           "4-Ortalama\n"
           "5-Agacin Yuksekligi\n"
           "6-Ayni Ad Olanlari Listele ve Sil\n"
           "7-Cikis\n"
           "\tBir secim yapin:");
    scanf("%d",&secim);

    switch(secim)
    {
    case 1:
        system("cls");
        printf("Eklenecek ad: ");
        scanf("%s",ad);
        printf("Eklenecek no: ");
        scanf("%d",&no);
        ekle(agac,ad,no);
        goto tekrar;
        break;
    case 2:
        system("cls");
        printf("Silinecek kisinin adini girin: ");
        scanf("%s",aranan);
        agac = silme(agac,arama(agac,aranan));
        printf("\nIstenilen kisi silindi.");
        goto tekrar;
        break;
    case 3:
        system("cls");
        printf("Inorder Listeleme:\n");
        listele(agac);
        goto tekrar;
        break;
    case 4:
        system("cls");
        ortalama(agac);
        printf("Ortalama: %d\n",(top/sayac));
        top=0;
        goto tekrar;
        break;
    case 5:
        system("cls");
        yukseklik(agac,-1);
        printf("Yukseklik: %d\n",yuks);
        yuks=0;
        goto tekrar;
        break;
    case 6:
        system("cls");
        printf("Ayni isme sahip olanlar:\n");
        ayniBul(agac);
        goto tekrar;
        break;
    case 7:
        exit(0);
    default:
        printf("\nYanlis secim,tekrar dogru secim yapin.");
        goto tekrar;
        break;
    }
    return 0;
}

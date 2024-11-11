#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node{
    int vred; int indeks;
} cvor;

typedef struct elem{
    double x; double y; double r;
    int vred; int indeks;
} indElem;

typedef struct graph{
    cvor *grane;
    indElem *indeksi;
} graf;

graf *kopirajGraf(int *n, int *brGrana,graf* original) {
    graf *kopija = malloc(sizeof(graf));

    kopija->grane = malloc(*brGrana*sizeof(cvor));
    kopija->indeksi = malloc((*n+1)*sizeof(indElem));

    memcpy(kopija->grane,original->grane,*brGrana*sizeof(cvor));
    memcpy(kopija->indeksi,original->indeksi,(*n+1)*sizeof(indElem));

    return kopija;
}

graf *napraviGraf(int *n) {
    indElem *indeksi = NULL; cvor *grane = NULL;
    if (*n>0){
        for (int i=0; i<*n+1; i++) {
            indeksi = realloc(indeksi,(i+1)*sizeof(indElem));
            if (indeksi == NULL){
                printf("Greska pri alokaciji memorije.");
                exit(1);
            }
            indeksi[i].indeks = i;
            if (i!=*n) indeksi[i].vred = -1;
            else indeksi[i].vred = 0;
        }
        graf *mojGraf = malloc(sizeof(graf));
        mojGraf->indeksi = indeksi; mojGraf->grane = grane;
        return mojGraf;
    }
    else printf("Broj n mora biti pozitivan, jer on predstavlja broj cvorova. Pokusajte ponovo\n");
    return NULL;
}

void ispisiGraf(int n, int brGrana, graf *graf) {
    cvor *grane = graf->grane;
    indElem *indeksi = graf->indeksi;
    printf(" GRANE:   ");
    for (int i=0; i<brGrana; i++) printf("%4d ", grane[i].vred);
    printf("\nINDEKSI:  ");
    for (int i=0; i<n+1; i++) printf("%4d ", indeksi[i].vred);
    printf("\n\n");
}

void dodajCvor(int *n, graf *graf){
    indElem *indeksi = graf->indeksi; ++(*n);
    indeksi = realloc(indeksi,(*n+1)*sizeof(indElem));
    if (indeksi == NULL){
        printf("Greska pri alokaciji memorije.");
        exit(1);
    }
    indeksi[*n].indeks = *n; indeksi[*n].vred = indeksi[*n-1].vred;
    indeksi[*n-1].vred = -1;
    graf->indeksi = indeksi;
}

void dodajGranu (int *n, int *brGrana, int od, int ka ,graf *graf){
    cvor *grane = graf->grane; indElem *indeksi = graf->indeksi;
    int i,poz,gran,ind;

    grane = realloc(grane,(*brGrana+1)*sizeof(cvor));
    if (grane == NULL){
        printf("Greska pri alokaciji memorije.");
        exit(1);
    }

    if (indeksi[*n-1].indeks>=od>=indeksi[0].indeks && indeksi[*n-1].indeks>=ka>=indeksi[0].indeks){
        ++(*brGrana); grane[*brGrana-1].indeks = *brGrana-1;
        if (indeksi[od].vred == -1){
            for (i=indeksi[od+1].indeks;i<=*n;i++){
                if (indeksi[i].vred != -1){
                    poz = indeksi[i].vred;
                    break;
                }
            }
        }
        else {
            for (i=indeksi[od+1].indeks;i<=*n;i++){
                if (indeksi[i].vred != -1){
                    gran = indeksi[i].vred;
                    break;
                }
            }
            for (i=gran-1;i>=indeksi[od].vred;i--){
                if (ka>grane[i].vred){
                    poz = grane[i].indeks+1;
                    break;
                }
            }
        }
        for (i=*brGrana-1;i>poz;i--) grane[i].vred = grane[i-1].vred;
        grane[poz].vred = ka;
        for (i=*n;i>od;i--) if (indeksi[i].vred != -1) indeksi[i].vred++;
        if (indeksi[od].vred == -1) indeksi[od].vred = poz;
        graf->indeksi = indeksi; graf->grane = grane;
    }
    else printf("Grana nije uspesno dodata, indeksi nisu korektnih vrednosti.");
}

void ukloniGranu(int *n, int *brGrana, int od, int ka ,graf *graf){
    cvor *grane = graf->grane; indElem *indeksi = graf->indeksi;
    int i,gran,fl=0,poz;
    if (indeksi[*n-1].indeks >= od && od >= indeksi[0].indeks &&
        indeksi[*n-1].indeks >= ka && ka >= indeksi[0].indeks){
        if (indeksi[od].vred == -1) {
            printf("Grana nije uspesno obrisana, indeksi nisu korektnih vrednosti.");
            return;
        }
        else {
            for (i=indeksi[od+1].indeks;i<=*n;i++){
                if (indeksi[i].vred != -1){
                    gran = indeksi[i].vred;
                    break;
                }
            }
            for (i=indeksi[od].vred;i<gran;i++){
                if (ka == grane[i].vred){
                    fl = 1; poz = grane[i].indeks;
                    break;
                }
            }
            if (fl == 1){
                for (i=poz;i<*brGrana-1;i++) grane[i].vred = grane[i+1].vred;
                for (i=*n;i>od;i--) if (indeksi[i].vred != -1) indeksi[i].vred--;
                if (gran-indeksi[od].vred == 1) indeksi[od].vred = -1;
                if (--(*brGrana) != 0){
                    grane = realloc(grane,(*brGrana) * sizeof(cvor));
                    if (grane == NULL){
                        printf("Greska pri alokaciji memorije.");
                        exit(1);
                    }
                }
                else grane = NULL;
            }
            if (fl == 0){
                printf("Grana nije uspesno obrisana, indeksi nisu korektnih vrednosti.");
                return;
            }
        }
        graf->indeksi = indeksi; graf->grane = grane;
    }
    else printf("Grana nije uspesno obrisana, indeksi nisu korektnih vrednosti.");
}

void obrisi1(int *n, int *brGrana,int uklonjen,graf *graf){
    cvor *grane = graf->grane; indElem *indeksi = graf->indeksi;
    int i,gran,pom;
    for (i=indeksi[uklonjen+1].indeks;i<=*n;i++){
        if (indeksi[i].vred != -1){
            gran = indeksi[i].vred;
            break;
        }
    }
    for (i=indeksi[uklonjen].vred;i<gran;i++){
        if (i==indeksi[uklonjen].vred) pom = grane[i].vred;
        ukloniGranu(n,brGrana, uklonjen,pom,graf);
        pom = grane[i].vred;
    }
    graf->indeksi = indeksi; graf->grane = grane;
}

void obrisi2(int *n, int *brGrana,int uklonjen,graf *graf){
    cvor *grane = graf->grane; indElem *indeksi = graf->indeksi;
    int i,j,od;
    for (i=0;i<*brGrana;i++){
        if (grane[i].vred == uklonjen){
            for (j=0;j<=*n;j++){
                if (indeksi[j].vred != -1 && grane[i].indeks<indeksi[j].vred){
                    for (int k=j-1;k>=0;k--){
                        if (indeksi[k].vred != -1) {
                            od = indeksi[k].indeks;
                            break;
                        }
                    }
                    break;
                }
            }
            ukloniGranu(n,brGrana, od,uklonjen,graf); i--;
        }
    }
}

void ukloniCvor(int *n, int *brGrana,int uklonjen,graf *graf){
    cvor *grane = graf->grane; indElem *indeksi = graf->indeksi;
    int i;
    if (indeksi[*n-1].indeks >= uklonjen && uklonjen >= indeksi[0].indeks){
        if (indeksi[uklonjen].vred !=-1) {
            obrisi1(n,brGrana,uklonjen,graf);
        }
        obrisi2(n,brGrana,uklonjen,graf);
        for (i=uklonjen;i<*n;i++) {
            indeksi[i].vred = indeksi[i+1].vred;
        }
        --(*n);
        indeksi = realloc(indeksi,(*n+1)*sizeof(indElem));
        if (indeksi == NULL){
            printf("Greska pri alokaciji memorije.");
            exit(1);
        }
        for (i=0;i<*brGrana;i++) if (grane[i].vred >= uklonjen) grane[i].vred--;
        graf->indeksi = indeksi; graf->grane = grane;
    }
    else printf("Ne postoji cvor koji mozes ukloniti.");
}

void oslobodiMemoriju(graf *graf) {
    if (graf != NULL) {
        if (graf->grane != NULL) free(graf->grane);
        if (graf->indeksi != NULL) free(graf->indeksi);
        free(graf);
    }
}

graf *ucitajIzFajla(int *nn){
    char imeFajla[100]; int n;
    printf("Unesi ime fajla: \n"); scanf("%s", imeFajla);
    FILE *fajl = fopen(imeFajla, "r");
    if (fajl == NULL) {
        printf("Greska pri otvaranju fajla.\n");
        exit(1);
    }
    if (fscanf(fajl, "%d", &n) != 1) {
        printf("Greska pri citanju broja n.\n");
        fclose(fajl);
        exit(1);
    }
    if (n <= 0) {
        printf("Broj n mora biti pozitivan, jer on predstavlja broj cvorova.\n");
        fclose(fajl);
        exit(1);
    }
    graf *mojGraf = napraviGraf(&n);
    indElem *indeksi = mojGraf->indeksi;
    for (int i = 0; i < n; i++) {
        if (fscanf(fajl, "%lf %lf %lf", &indeksi[i].x, &indeksi[i].y, &indeksi[i].r) != 3) {
            printf("Greska pri citanju podataka za cvor.\n");
            fclose(fajl);
            exit(1);
        }
        if (indeksi[i].r <= 0) {
            printf("Poluprecnik za cvor %d mora biti pozitivan.\n", i + 1);
            fclose(fajl);
            exit(1);
        }
    }
    *nn = n;
    fclose(fajl);
    return mojGraf;
}

void poveziMinu(int *n, int *brGrana,int mina,graf *graf){
    indElem *indeksi = graf->indeksi;
    for (int i=0;i<*n;i++){
        if (i == mina) continue;
        if (sqrt(pow(indeksi[mina].x-indeksi[i].x,2)+pow(indeksi[mina].y-indeksi[i].y,2))<=indeksi[mina].r) dodajGranu(n,brGrana,mina,i,graf);
    }
}

int prazanStek(int top) {
    if (top == -1) return 0;
    else return 1;
}
void push(int* stek,int *top,int vred) {
    ++(*top);
    stek[*top] = vred;
}
int pop(int* stek,int *top) {
    return stek[(*top)--];
}

int *DFS(int *n, int *brGrana,int *duz,int poc,graf *graf){
    *duz = 0;
    cvor *grane = graf->grane; indElem *indeksi = graf->indeksi;
    int top = -1, stek[*brGrana], poseceni[*n],i,mina,brojac=0,gran;
    int *niz = malloc(*brGrana * sizeof(int));
    for (i=0;i<*n;i++) poseceni[i] = 0;
    push(stek,&top,poc);
    while (prazanStek(top) == 1){
        mina = pop(stek,&top);
        if (poseceni[mina] == 0){
            niz[brojac++] = mina; poseceni[mina] = 1;
            for (i=indeksi[mina+1].indeks;i<=*n;i++){
                if (indeksi[i].vred != -1){
                    gran = indeksi[i].vred;
                    break;
                }
            }
            if (indeksi[mina].vred != -1) for (i=indeksi[mina].vred;i<gran;i++) if (poseceni[grane[i].vred] == 0) push(stek,&top,grane[i].vred);
        }
    }
    for (i=0;i<*n;i++) if (poseceni[i] == 1) *duz = *duz + 1;
    return niz;
}

void raketa(int *n, int *brGrana,graf *original){
    int *niz = NULL,duz = 0;
    graf *kopija = kopirajGraf(n,brGrana,original);
    dodajCvor(n, kopija);
    cvor *grane = kopija->grane; indElem *indeksi = kopija->indeksi;
    printf("\nUnesi x vrednost rakete:"); scanf("%lf",&indeksi[*n-1].x);
    printf("Unesi y vrednost rakete:"); scanf("%lf",&indeksi[*n-1].y);
    printf("Unesi r vrednost rakete:"); scanf("%lf",&indeksi[*n-1].r);
    kopija->indeksi = indeksi;
    poveziMinu(n,brGrana,*n-1,kopija);
    niz = DFS(n,brGrana,&duz,*n-1,kopija);
    printf("\nRaketom detonirane mine su: \n\n");
    for (int i=0;i<duz;i++) {
        if (niz[i] != *n-1) printf("Mina: %2d\n",niz[i]);
    }
    putchar('\n');
}

double monteKarlo(int *n, int *brGrana, graf *graf) {
    indElem *indeksi = graf->indeksi;
    int brTacaka = 100000, duz, maks = 0, brUpalih = 0, *niz;
    double ukupanpp = 0, xt, yt, povrsina = 0;

    for (int i=0;i<*n;i++) {
        niz = DFS(n, brGrana, &duz, i, graf);
        if (duz >= maks) maks = duz;
    }
    for (int i=0;i<*n;i++) {
        niz = DFS(n, brGrana, &duz, i, graf);
        if (duz == maks) break;
    }
    for (int i=0;i<duz;i++) ukupanpp += indeksi[niz[i]].r;
    for (int i=0;i<brTacaka; i++) {
        xt = (2*((double)rand()/RAND_MAX)-1)*ukupanpp;
        yt = (2*((double)rand()/RAND_MAX)-1)*ukupanpp;
        for (int j = 0; j < duz; j++) {
            if (sqrt(pow(indeksi[niz[j]].x-xt, 2)+pow(indeksi[niz[j]].y-yt, 2))<=indeksi[niz[j]].r){
                brUpalih++;
                break;
            }
        }
    }
    povrsina = ((double)brUpalih/brTacaka)*(4*ukupanpp*ukupanpp);
    return povrsina;
}

void mineMain(int *n, int *brGrana){
    printf("Rad sa minama!\n");
    graf *mojGraf = ucitajIzFajla(n);
    for (int i=0;i<*n;i++){
        poveziMinu(n,brGrana,i,mojGraf);
    }
    printf("\nTvoj graf: \n\n");
    ispisiGraf(*n, *brGrana, mojGraf);
    int duz=0, poc, *niz = NULL, br, maks = 0;
    while(1){
        printf("1. Ispisi efikasnost zadate mine\n2. Pronadji minu/mine maksimalne/maksimalnih efikasnosti\n3. Lansiraj raketu\n4. Za prvu minu maksimalne efikasnosti ispisi povrsinu\n5. Izadji iz rada sa minama\n0. Prekini izvrsavanje programa\n\nIzaberi:");
        scanf("%d",&br);
        if(br<0 || br>5) {
            printf("Izabrao/la si opciju koja ne postoji!\nPokusaj ponovo!\n");
            continue;
        }
        if(br==0){
            oslobodiMemoriju(mojGraf);
            exit(0);
        }
        if(br==1) {
            printf("Unesi minu koji zelis da detoniras:"); scanf("%d",&poc);
            niz = DFS(n,brGrana,&duz,poc,mojGraf);
            printf("\nDetonirane mine su: \n");
            for (int i=0;i<duz;i++) printf("Mina: %2d\n",niz[i]);
            putchar('\n');
        }
        if(br==2) {
            for (int i=0;i<*n;i++){
                niz = DFS(n,brGrana,&duz,i,mojGraf);
                if (duz>=maks) maks = duz;
            }
            printf("\nMaksimalna mina/ maksimalne mine:\n\n");
            int broj = 1;
            for (int i=0;i<*n;i++){
                niz = DFS(n,brGrana,&duz,i,mojGraf);
                if (duz == maks){
                    printf("%d. maksimalna mina je %d. mina, koja detonira:\n",broj,i);
                    for (int j=0;j<duz;j++) printf("Minu: %2d\n",niz[j]);
                    broj++; putchar('\n');
                }
            }
        }
        if(br==3) {
            int a = *n, b = *brGrana;
            raketa(&a, &b,mojGraf);
            mojGraf->grane = realloc(mojGraf->grane,*brGrana * sizeof(cvor));
            mojGraf->indeksi = realloc(mojGraf->indeksi,(*n+1) * sizeof(indElem));
        }
        if(br==4){
            double povrsina = monteKarlo(n,brGrana,mojGraf);
            printf("\nPovrsina prve mine sa maksimalnom efikasnoscu je: %.2lf",povrsina);
            putchar('\n'); putchar('\n');
        }
        if(br==5) break;
    }
}

int main() {
    int br,n,brGrana = 0,uklonjen,od,ka;
    graf *mojGraf = NULL;
    while(1){
        printf("1. Napravi prazan graf\n2. Dodaj cvor u graf\n3. Ukloni cvor iz grafa\n4. Dodaj granu\n5. Ukloni granu\n6. Ispisi graf\n7. Oslobodi memoriju\n8. Pokreni drugi zadatak - rad sa minama\n0. Prekini izvrsavanje programa\n\nUzmi u obzir da je pre izvrsavanja operacija nad grafom bitno kreirati ga, kao i da indeksiranje krece od 0!\n\nIzaberi:");
        scanf("%d",&br);
        if(br<0 || br>8) {
            printf("Izabrao/la si opciju koja ne postoji!\nPokusaj ponovo!\n");
            continue;
        }
        if(br==0){
            oslobodiMemoriju(mojGraf);
            exit(0);
        }
        if(br==1) {
            if(mojGraf == NULL){
                printf("Unesi broj cvorova: "); scanf("%d",&n);
                putchar('\n');
                mojGraf = napraviGraf(&n);
                ispisiGraf(n, brGrana, mojGraf);
            }
            else printf("\nVec si napravio/la graf!\n\n");
        }
        if(br==2) {
            dodajCvor(&n, mojGraf);
            putchar('\n');
            printf("Tvoj graf: \n\n");
            ispisiGraf(n, brGrana, mojGraf);
        }
        if(br==3) {
            printf("Unesi broj cvora koji uklanjas iz grafa:"); scanf("%d",&uklonjen);
            ukloniCvor(&n,&brGrana,uklonjen,mojGraf);
            printf("\nTvoj graf: \n\n");
            ispisiGraf(n, brGrana, mojGraf);
        }
        if(br==4){
            printf("Dodajes granu iz cvora:"); scanf("%d",&od);
            printf("U cvor:"); scanf("%d",&ka);
            dodajGranu(&n, &brGrana, od, ka, mojGraf);
            printf("\nTvoj graf: \n\n");
            ispisiGraf(n, brGrana, mojGraf);
        }
        if(br==5) {
            printf("Uklanjas granu iz cvora:"); scanf("%d",&od);
            printf("U cvor:"); scanf("%d",&ka);
            ukloniGranu(&n, &brGrana, od, ka, mojGraf);
            printf("\nTvoj graf: \n\n");
            ispisiGraf(n, brGrana, mojGraf);
        }
        if(br==6) {
            putchar('\n');
            ispisiGraf(n, brGrana, mojGraf);
        }
        if(br==7) oslobodiMemoriju(mojGraf);
        if(br==8) mineMain(&n,&brGrana);
    }
}
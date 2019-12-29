#include <iostream>
#include<ctime>
#define DUZINA_NIZA 10
using namespace std;

class Artikal{
private:
    const int sifra;
    char* naziv;
    float cijena, kolicina;

public:
    Artikal(const int sifra_, char* naziv_, float kolicina_, float cijena_): sifra(sifra_)
    {
        naziv = naziv_;
        kolicina = kolicina_;
        cijena = cijena_;
    }
    ~Artikal();

    int vrati_sifra(){ return sifra; }
    char* vrati_naziv(){ return naziv; }
    float vrati_kolicina(){ return kolicina; }
    float vrati_cijena() { return cijena; }

    bool promijeni_kolicinu(const float& pk){
        float nova_kolicina = kolicina + pk;
        if(nova_kolicina >= 0){
            kolicina = nova_kolicina;
            return true;
        }
        else{
            return false;
        }
    }
};

Artikal* spisak_artikala[DUZINA_NIZA];
void funckija1()
{
    for(int i=0; i<DUZINA_NIZA; i++)
        spisak_artikala[i] = NULL;
}

Artikal::~Artikal(){
    for(int i=0; i<DUZINA_NIZA; i++)
    if(spisak_artikala[i] != NULL && spisak_artikala[i]->sifra == sifra){
        spisak_artikala[i] = NULL;
        break;
    }
}

bool ubaci_artikal_u_spisak(Artikal* artikal){
    for(int i=0; i<DUZINA_NIZA; i++)
        if(spisak_artikala[i] == NULL){
            spisak_artikala[i] = artikal;
            return true;
        }
    return false;
}

void ispis_vremena(time_t t)
{
    tm *ptm;
    ptm=gmtime(&t);

    int godina = ptm -> tm_year + 1900;
    int mjesec = ptm -> tm_mon + 1;
    int dan = ptm -> tm_mday;
    int sat = ptm -> tm_hour +1;
    int minuti = ptm -> tm_min;
    cout<<dan<<"/"<<mjesec<<"/"<<godina<<" "<<sat<<":"<<minuti;
}

class Racun{
private:
    float kolicina[10];
    const time_t vrijeme;
    float iznos;

public:
    Racun(): vrijeme(time(NULL))
    {
        iznos = 0;
        for(int i=0; i<DUZINA_NIZA; i++)
            kolicina[i] = 0;
    }
    ~Racun()
    {
        delete kolicina;
    }

    void dodaj_artikal(const int sifra_artikla, const float kllicina_)
    {
        bool uspj_prom_kol = false;
        for(int i=0; i<DUZINA_NIZA; i++)
            if(spisak_artikala[i]!=NULL && spisak_artikala[i]->vrati_sifra()==sifra_artikla)
            {
                uspj_prom_kol = spisak_artikala[i]->promijeni_kolicinu(-kllicina_);
                if(uspj_prom_kol){
                    kolicina[i] += kllicina_;
                    iznos += spisak_artikala[i]->vrati_cijena() * kllicina_;
                }
            }
        if(!uspj_prom_kol)
            cout<<"GRESKA! Zahtjev je odbijen."<<endl;
    }

    void obrisi_artikal(const int sifra_artikla)
    {
        bool uspjesna_radnja = false;
        for(int i=0; i<DUZINA_NIZA; i++)
            if(spisak_artikala[i]!=NULL && spisak_artikala[i]->vrati_sifra()==sifra_artikla)
            {
                spisak_artikala[i]->promijeni_kolicinu(kolicina[i]);
                iznos -= spisak_artikala[i]->vrati_cijena() * kolicina[i];
                kolicina[i] = 0;
                uspjesna_radnja = true;
                cout<<"Artikal "<<spisak_artikala[i]->vrati_naziv()<<" je uspjesno obrisan sa racuna."<<endl;
            }
        if(!uspjesna_radnja)
            cout<<"Greska: zahtjrv je odbijen!"<<endl;
    }

    float vrati_iznos(){ return iznos; }
    time_t vrati_vrijeme_izdavanja_racuna(){ return vrijeme; }

    Racun operator + (Racun& rac1)
    {
        Racun rez;
        for(int i=0; i<DUZINA_NIZA; i++){
            rez.dodaj_artikal(spisak_artikala[i]->vrati_sifra(), rac1.kolicina[i] + kolicina[i]);
            rac1.obrisi_artikal(spisak_artikala[i]->vrati_sifra());
            obrisi_artikal(spisak_artikala[i]->vrati_sifra());
        }
        return rez;
    }
    friend ostream& operator << (ostream& stream, const Racun& rac)
    {
        stream << "Vrijeme izdavanja racuna: ";
        ispis_vremena(rac.vrijeme);
        cout<< endl;
        for(int i = 0; i < DUZINA_NIZA; i++){
            if(rac.kolicina[i]!= 0){
                stream << spisak_artikala[i]->vrati_naziv() << '\t'
                       << rac.kolicina[i] << '\t'
                       << rac.kolicina[i]*spisak_artikala[i]->vrati_cijena()<<" KM"<<endl;
            }
        }
        stream << "Iznos racuna: " << rac.iznos << " KM";
        return stream;
    }
};

int main()
{
    funckija1();
    Artikal pilece_meso(1230, "Pilece meso", 30, 7.5);
    ubaci_artikal_u_spisak(&pilece_meso);
    Artikal junetina(1231, "Junece meso", 15.5, 12.5);
    ubaci_artikal_u_spisak(&junetina);
    Racun racun1;
    racun1.dodaj_artikal(1230, 2.200);
    racun1.dodaj_artikal(1231, 1.5);
    cout<<racun1<<endl;
}

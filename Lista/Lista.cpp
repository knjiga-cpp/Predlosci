#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

class Osoba
{
public:
    Osoba() : instanca(++brojac)
    {
        cout << "Osoba()" << instanca << endl;
    }
    Osoba(string ime) : ime(move(ime)), instanca(++brojac)
    {
        cout << "Osoba(string)" << instanca << endl;
    }
    Osoba(const Osoba& osoba) : ime(osoba.ime), instanca(++brojac)
    {
        cout << "Osoba(const Osoba&)" << instanca << endl;
    }
    Osoba(Osoba&& osoba) : ime(move(osoba.ime)), instanca(++brojac)
    {
        cout << "Osoba(Osoba&&) " << instanca << endl;
    }
    ~Osoba()
    {
        cout << "Destruktor " << instanca << endl;
    }

private:
    string ime;
    int instanca;

    static int brojac;
};

int Osoba::brojac = 0;

template <typename Tip>
class Lista
{
private:
	class ElementListe
	{
	private:
		Tip vrijednost;
		ElementListe* prethodni;
		unique_ptr<ElementListe> sljedeci;
	public:
		ElementListe* Prethodni() { return prethodni; }
		unique_ptr<ElementListe>& Sljedeci() { return sljedeci; }
		void StaviPrethodni(ElementListe* pret)
		{
			prethodni = pret;
		}
		void StaviSljedeci(unique_ptr<ElementListe>& sljed)
		{
			sljedeci = move(sljed);
		}
		ElementListe(const Tip& elem) : prethodni(nullptr),
			sljedeci(nullptr),
			vrijednost(elem) {}
		Tip& DajVrijednost() { return vrijednost; }
	};

	unique_ptr<ElementListe> glava;
	ElementListe* rep;
	int duljina;
public:
	Lista() : glava(nullptr), rep(nullptr), duljina(0) {}
	//ElementListe* AmoGlavu() { return glava; }
	//ElementListe* AmoRep() { return rep; }
    void UgurajNaPocetak(const Tip& elem);
    void NakeljiNaKraj(const Tip& elem);
    void UgurajClan(const Tip& elem, int redBr = 0);
	void GoniClan(int koga);
	Tip& operator [](int ind);
	int KolikoSiDuga() { return duljina; }
};

template <typename Tip>
void Lista<Tip>::UgurajNaPocetak(const Tip& elem)
{
    unique_ptr<ElementListe> element = make_unique<ElementListe>(elem);
    // ako već ima članova u listi...
    if (glava)
        // trenutni prvi element preusmjeri na novi
        glava->StaviPrethodni(element.get());
    // novi element preusmjeri na trenutni prvi
    element->StaviSljedeci(glava);
    // ako je prazna lista. rep usmjeri na novi element
    if (!rep)
        rep = element.get();
    // novi element postaje glava
    glava = move(element);
    ++duljina;
}

template <typename Tip>
void Lista<Tip>::NakeljiNaKraj(const Tip& elem)
{
    unique_ptr<ElementListe> element = make_unique<ElementListe>(elem);
    // trenutni zadnji preusmjeri na novi
    element->StaviPrethodni(rep);
    ElementListe* noviRep = element.get();
    if (rep)
        rep->StaviSljedeci(element);
    else
        glava = move(element);
    rep = noviRep;
    ++duljina;
}


template <typename Tip>
void Lista<Tip>::UgurajClan(const Tip& elem, int redBr)
{
	if (redBr < 0 || redBr > duljina)
		throw out_of_range("nedozvoljeni indeks");
    if (redBr == 0)
        return UgurajNaPocetak(elem);
    if (redBr == duljina)
        return NakeljiNaKraj(elem);
    // tražimo gdje treba umetnuti
    ElementListe* izaKojeg = glava.get();
	while (--redBr)
		izaKojeg = izaKojeg->Sljedeci().get();
    assert(izaKojeg);
    unique_ptr<ElementListe> element = make_unique<ElementListe>(elem);
	izaKojeg->Sljedeci()->StaviPrethodni(element.get());
	element->StaviSljedeci(izaKojeg->Sljedeci());
	element->StaviPrethodni(izaKojeg);
	izaKojeg->StaviSljedeci(element);
	++duljina;
}

template <typename Tip>
void Lista<Tip>::GoniClan(int koga)
{
	if (koga < 0 || koga >= duljina)
		throw out_of_range("nedozvoljeni indeks");
	ElementListe* pok = glava.get();
	while (koga)
	{
		pok = pok->Sljedeci().get();
		--koga;
	}
	if (pok->Sljedeci())
		pok->Sljedeci()->StaviPrethodni(pok->Prethodni());
	else
		rep = pok->Prethodni();
	if (pok->Prethodni())
		pok->Prethodni()->StaviSljedeci(pok->Sljedeci());
	else
		glava = move(pok->Sljedeci());
	//delete pok;
	--duljina;
}

template <typename Tip>
Tip& Lista<Tip>::operator [](int indeks)
{
	if (indeks < 0 || indeks >= duljina)
		throw out_of_range("nedozvoljeni indeks");
	ElementListe* pok = glava;
	while (indeks)
	{
		pok = pok->Sljedeci();
		--indeks;
	}
	return pok->DajVrijednost();
}

int main()
{
	Lista<Osoba> l;
    l.UgurajNaPocetak(Osoba("Dora"));
    l.GoniClan(0);
    l.NakeljiNaKraj(Osoba("Neven"));
	l.UgurajClan(Osoba("Dora"));
	l.UgurajClan(Osoba("Miha"), 2);
	l.UgurajClan(Osoba("Matej"), 1);
    l.UgurajClan(Osoba("Grigor"), 2);
    l.GoniClan(1);
    l.GoniClan(0);
    l.GoniClan(0);
    l.GoniClan(0);
    l.GoniClan(0);
}

#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

template <typename Tip>
class Lista
{
private:
	class ElementListe
	{
	private:
		Tip vrijednost;
		ElementListe* prethodni;
		ElementListe* sljedeci;
	public:
		ElementListe* Prethodni() { return prethodni; }
		ElementListe* Sljedeci() { return sljedeci; }
		void StaviPrethodni(ElementListe* pret)
		{
			prethodni = pret;
		}
		void StaviSljedeci(ElementListe* sljed)
		{
			sljedeci = sljed;
		}
		ElementListe(const Tip& elem) : prethodni(nullptr),
			sljedeci(nullptr),
			vrijednost(elem) {}
		Tip& DajVrijednost() { return vrijednost; }
	};

	ElementListe* glava;
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
    ElementListe* element = new ElementListe{ elem };
    // novi element preusmjeri na trenutni prvi
    element->StaviSljedeci(glava);
    // ako već ima članova u listi...
    if (glava)
        // trenutni prvi element preusmjeri na novi
        glava->StaviPrethodni(element);
    // novi element postaje glava
    glava = element;
    // ako je prazna lista. rep usmjeri na novi element
    if (!rep)
        rep = element;
    ++duljina;
}

template <typename Tip>
void Lista<Tip>::NakeljiNaKraj(const Tip& elem)
{
    ElementListe* element = new ElementListe{ elem };
    // novi element preusmjeri na trenutni zadnji
    element->StaviPrethodni(rep);
    if (rep)
        // trenutni zadnji preusmjeri na novi
        rep->StaviSljedeci(element);
    // novi element postaje rep
    rep = element;
    if (!glava)
        glava = element;
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
    ElementListe* izaKojeg = glava;
	while (--redBr)
		izaKojeg = izaKojeg->Sljedeci();
    assert(izaKojeg);
    ElementListe* element = new ElementListe{ elem };
	izaKojeg->Sljedeci()->StaviPrethodni(element);
	element->StaviSljedeci(izaKojeg->Sljedeci());
	izaKojeg->StaviSljedeci(element);
	element->StaviPrethodni(izaKojeg);
	++duljina;
}

template <typename Tip>
void Lista<Tip>::GoniClan(int koga)
{
	if (koga < 0 || koga >= duljina)
		throw out_of_range("nedozvoljeni indeks");
	ElementListe* pok = glava;
	while (koga)
	{
		pok = pok->Sljedeci();
		--koga;
	}
	if (pok->Sljedeci())
		pok->Sljedeci()->StaviPrethodni(pok->Prethodni());
	else
		rep = pok->Prethodni();
	if (pok->Prethodni())
		pok->Prethodni()->StaviSljedeci(pok->Sljedeci());
	else
		glava = pok->Sljedeci();
	delete pok;
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
	Lista<string> l;
	l.UgurajClan("Dora");
	l.UgurajClan("Miha", 1);
	l.UgurajClan("Matej", 1);
    l.UgurajClan("Grigor", 2);
    l.GoniClan(1);
    l.GoniClan(0);
    l.GoniClan(0);
    l.GoniClan(0);
}

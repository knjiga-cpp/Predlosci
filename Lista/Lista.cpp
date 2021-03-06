#include <iostream>
#include <memory>
#include <cassert>
#include <string>
using namespace std;

class Osoba
{
public:
    Osoba() : instanca(++brojac)
    {
        cout << "Osoba(): " << instanca << endl;
    }
    Osoba(string ime) : ime(move(ime)), instanca(++brojac)
    {
        cout << "Osoba(string): " << instanca << endl;
    }
    Osoba(const Osoba& osoba) : ime(osoba.ime), instanca(++brojac)
    {
        cout << "Osoba(const Osoba&): " << instanca << endl;
    }
    Osoba(Osoba&& osoba) : ime(move(osoba.ime)), instanca(++brojac)
    {
        cout << "Osoba(Osoba&&): " << instanca << endl;
    }

	Osoba& operator=(const Osoba&) = default;

    ~Osoba()
    {
        cout << "Destruktor: " << instanca << endl;
    }

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
		ElementListe* prethodni{ nullptr };
        unique_ptr<ElementListe> sljedeci{};
	public:
		ElementListe* Prethodni() const noexcept { return prethodni; }
		unique_ptr<ElementListe>& Sljedeci() noexcept { return sljedeci; }
		void PostaviPrethodni(ElementListe* pret)
		{
			prethodni = pret;
		}
		void PostaviSljedeci(unique_ptr<ElementListe>& sljed)
		{
			sljedeci = move(sljed);
		}
		ElementListe(const Tip& elem) : vrijednost(elem) {}
		Tip& DajVrijednost() { return vrijednost; }
	};

    unique_ptr<ElementListe> glava{};
    ElementListe* rep{ nullptr };
    int duljina{ 0 };

	ElementListe* DajElement(int indeks) const noexcept;
public:
    void UgurajNaPocetak(const Tip& elem);
    void NakeljiNaKraj(const Tip& elem);
    void UgurajClan(const Tip& elem, int redBr = 0);
	void GoniClan(int indeks);
	Tip& operator [](int ind);
	int KolikoSiDuga() const noexcept { return duljina; }
};

template <typename Tip>
void Lista<Tip>::UgurajNaPocetak(const Tip& elem)
{
    unique_ptr<ElementListe> element = make_unique<ElementListe>(elem);
    // ako već ima članova u listi...
    if (glava)
        // trenutni prvi element preusmjeri na novi
        glava->PostaviPrethodni(element.get());
    // novi element preusmjeri na trenutni prvi
    element->PostaviSljedeci(glava);
    if (!rep)
		// ako je prazna lista. rep usmjeri na novi član
		rep = element.get();
    // novi član postaje glava
    glava = move(element);
    ++duljina;
}

template <typename Tip>
void Lista<Tip>::NakeljiNaKraj(const Tip& elem)
{
    unique_ptr<ElementListe> element = make_unique<ElementListe>(elem);
    // trenutni zadnji preusmjeri na novi
    element->PostaviPrethodni(rep);
    ElementListe* noviRep = element.get();
    if (rep)
		// ako već ima članova u listi, novi dodaj umjesto dosadašnjeg repa
		rep->PostaviSljedeci(element);
    else
		// ako nema članova u listi, novi postaje glava
        glava = move(element);
	// novi član postaje rep
    rep = noviRep;
    ++duljina;
}

template <typename Tip>
void Lista<Tip>::UgurajClan(const Tip& elem, int indeks)
{
	if (indeks < 0 || indeks > duljina)
		throw out_of_range("nedozvoljeni indeks");
    if (indeks == 0)
        return UgurajNaPocetak(elem);
    //if (indeks == duljina)
    //    return NakeljiNaKraj(elem);
    // tražimo iza kojeg treba umetnuti
    ElementListe* izaKojeg = glava.get();
	while (--indeks)
		izaKojeg = izaKojeg->Sljedeci().get();
    assert(izaKojeg);
    unique_ptr<ElementListe> element = make_unique<ElementListe>(elem);
	// preusmjerava pokazivače
	if (izaKojeg->Sljedeci())
		izaKojeg->Sljedeci()->PostaviPrethodni(element.get());
	else
		rep = element.get();
	element->PostaviSljedeci(izaKojeg->Sljedeci());
	element->PostaviPrethodni(izaKojeg);
	izaKojeg->PostaviSljedeci(element);
	++duljina;
}

template <typename Tip>
void Lista<Tip>::GoniClan(int indeks)
{
	if (indeks < 0 || indeks >= duljina)
		throw out_of_range("nedozvoljeni indeks");
	ElementListe* pok = glava.get();
	while (indeks)
	{
		pok = pok->Sljedeci().get();
		--indeks;
	}
	if (pok->Sljedeci())
		pok->Sljedeci()->PostaviPrethodni(pok->Prethodni());
	else
		rep = pok->Prethodni();
	if (pok->Prethodni())
		pok->Prethodni()->PostaviSljedeci(pok->Sljedeci());
	else
		glava = move(pok->Sljedeci());
	--duljina;
}

template <typename Tip>
Tip& Lista<Tip>::operator [](int indeks)
{
	if (indeks < 0 || indeks >= duljina)
		throw out_of_range("nedozvoljeni indeks");
	return DajElement(indeks)->DajVrijednost();
}

template <typename Tip>
typename Lista<Tip>::ElementListe* Lista<Tip>::DajElement(int indeks) const noexcept
{
	ElementListe* pok = glava.get();
	while (--indeks >= 0)
		pok = pok->Sljedeci().get();
	return pok;
}

int main()
{
	{
		Lista<Osoba> l;
		l.UgurajNaPocetak(Osoba("Dora"));
		l.GoniClan(0);
		l.NakeljiNaKraj(Osoba("Neven"));
		l.UgurajClan(Osoba("Dora"));
		l.UgurajClan(Osoba("Miha"), 2);
		l.UgurajClan(Osoba("Matej"), 1);
		l.UgurajClan(Osoba("Grigor"), 2);
		cout << l[3].ime << endl;
		l[3] = Osoba("Pero");
		l.GoniClan(1);
		l.GoniClan(3);
		l.GoniClan(0);
		l.GoniClan(0);
		l.GoniClan(0);
	}

	{
		Lista<Osoba> l;
		l.UgurajNaPocetak(Osoba("Dora"));
		l.GoniClan(0);
		l.NakeljiNaKraj(Osoba("Neven"));
		l.UgurajClan(Osoba("Dora"));
		l.UgurajClan(Osoba("Miha"), 2);
	}
}

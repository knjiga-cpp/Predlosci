#include <iostream>
#include <memory>

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
	ElementListe* AmoGlavu() { return glava; }
	ElementListe* AmoRep() { return rep; }
	void UgurajClan(const Tip& elem, int redBr = 0);
	void GoniClan(int koga);
	Tip& operator [](int ind);
	int KolikoSiDuga() { return duljina; }
};

template <typename Tip>
void Lista<Tip>::UgurajClan(const Tip& elem, int redBr)
{
	if (redBr < 0 || redBr >= duljina)
		throw out_of_range("nedozvoljeni indeks");
	ElementListe* element = new ElementListe{ elem };
	ElementListe* izaKojeg = nullptr;
	if (redBr)
	{
		izaKojeg = glava;
		--redBr;
		while (redBr)
		{
			izaKojeg = izaKojeg->Sljedeci();
			--redBr;
		}
	}
	// da li se dodaje na početak?
	if (izaKojeg)
	{
		// ne dodaje se na početak.
		// da li se dodaje na kraj?
		if (izaKojeg->Sljedeci())
			// ne dodaje se na kraj
			izaKojeg->Sljedeci()->StaviPrethodni(element);
		else
			// dodaje se na kraj
			rep = element;
		element->StaviSljedeci(izaKojeg->Sljedeci());
		izaKojeg->StaviSljedeci(element);
	}
	else
	{
		// dodaje se na početak
		element->StaviSljedeci(glava);
		if (glava)
			// da li već ima članova u listi?
			glava->StaviPrethodni(element);
		glava = element;
	}
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
    std::cout << "Hello World!\n"; 
}

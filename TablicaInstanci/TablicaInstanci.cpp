// TablicaInstanci.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

using TablicaStringInt = std::unordered_map<std::string, int>;

// klasu definiramo kao singleton:
class TablicaInstanci
{
	// samo prijateljska klasa može zvati članove Uvecaj() i Umanji():
	template<typename T>
	friend class BrojacInstanci;
private:
	TablicaInstanci() = default;        // privatni konstruktor (singleton)
	~TablicaInstanci();
	// onemogućavamo stvaranje kopije (singleton):
	TablicaInstanci(const TablicaInstanci&) = delete;
	TablicaInstanci& operator= (const TablicaInstanci&) = delete;
	// članovi kojima pristupaju samo prijateljske klase:
	void Uvecaj(const string& name);
	void Umanji(const string& name);
public:
	// jedini pristup tablici:
	static TablicaInstanci& Tablica();
	// javno dostupne funkcije:
	void Ispisi();
	void Obrisi();
private:
	TablicaStringInt tablica;
};

// definicije članova klase
TablicaInstanci::~TablicaInstanci()
{
	Obrisi();
}

void TablicaInstanci::Uvecaj(const string& name)
{
	if (tablica.find(name) == tablica.end())
		tablica[name] = 1;
	else
		++tablica[name];
}

void TablicaInstanci::Umanji(const string& name)
{
	--tablica[name];
}

void TablicaInstanci::Ispisi()
{
	cout << "ISPIS STANJA" << endl;
	for (const auto& iter : tablica)
		cout << iter.first << ": " << iter.second << endl;
}

void TablicaInstanci::Obrisi()
{
	Ispisi();
	tablica.clear();
}

TablicaInstanci& TablicaInstanci::Tablica()
{
	static TablicaInstanci instanca;
	return instanca;
}

#include <typeinfo>

template<typename Tip>
class BrojacInstanci
{
public:
	BrojacInstanci<Tip>()
	{
		string ime(typeid(Tip).name());
		TablicaInstanci::Tablica().Uvecaj(ime);
	}

	virtual ~BrojacInstanci<Tip>()
	{
		string ime(typeid(Tip).name());
		TablicaInstanci::Tablica().Umanji(ime);
	}
};


class MojaKlasa : public BrojacInstanci<MojaKlasa>
{
};

int main()
{
	MojaKlasa mk1;
	MojaKlasa mk2;
	TablicaInstanci::Tablica().Ispisi();
	{
		MojaKlasa mk3;
		TablicaInstanci::Tablica().Ispisi();
	}
	TablicaInstanci::Tablica().Ispisi();
	return 0;
}

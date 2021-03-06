#include <iostream>
#include <string>
using namespace std;

template<typename Tip>
Tip Zbroji(const Tip& prvi)
{
	return prvi;
}
 

template<typename TipPrvi, typename... TipOstali>
auto Zbroji(const TipPrvi& prvi, const TipOstali&... ostali)
{
	return prvi + Zbroji(ostali...);
}


// specijalizacija za nula argumenata:


//template <typename Tip1, typename Tip2>
//auto Zbroji(const Tip1& a, const Tip2& b)
//{
//	return a + b;
//}

template<typename... T>
size_t BrojArgumenata(const T&... argumenti)
{
	return sizeof...(argumenti);
}

template <typename Tip>
const Tip& Manji(const Tip& a, const Tip& b)
{
	return a < b ? a : b;
}

// specijalizacija za const char* slučaj
const char* Manji(const char* a, const char* b)
{
	return strcmp(a, b) < 0 ? a : b;
}

int main()
{
	auto deset = Zbroji(2, 3, 5);
	auto str = Zbroji(string("jedan"), string("dva"), string("tri"));

	const char* rez1 = Manji("manji", "veći");

	auto rez2 = Manji<string>("manji", "veći");

	BrojArgumenata(1, 2, 4);            // vraća 3
	BrojArgumenata("pi", 3.14);         // vraća 2
	BrojArgumenata();                    // vraća 0

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

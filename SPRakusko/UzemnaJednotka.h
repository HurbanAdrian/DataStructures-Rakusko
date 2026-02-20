#pragma once
#include <string>
class UzemnaJednotka
{
private:
	std::string nazov;
	int id;
	int pocetMuzov[5];
	int pocetZien[5];
	int celkovyPocet[5];

public:
	UzemnaJednotka();
	UzemnaJednotka(std::string paNazov, int paId, int paPocetMuzov[5], int paPocetZien[5]);
	~UzemnaJednotka();
	

	UzemnaJednotka(const UzemnaJednotka& zdroj);
	UzemnaJednotka& operator=(const UzemnaJednotka& zdroj);

	bool operator==(const UzemnaJednotka& other) const;


	std::string getNazov();
	int getId() const;

	int getPocetMuzov(int rokIndex);
	int getPocetZien(int rokIndex);
	int getCelkovyPocet(int rokIndex);

	void pridajZaznamMuzov(int pocet, int rokIndex);
	void pridajZaznamZien(int pocet, int rokIndex);


	int getTyp();
};



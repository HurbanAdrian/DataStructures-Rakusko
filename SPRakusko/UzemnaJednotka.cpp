#include "UzemnaJednotka.h"

UzemnaJednotka::UzemnaJednotka()
{
	this->nazov = "";
	this->id = -1;

	// moze byt std::fill_n?
	for (int i = 0; i < 5; ++i) {
		pocetMuzov[i] = 0;
		pocetZien[i] = 0;
		celkovyPocet[i] = 0;
	}
}

UzemnaJednotka::UzemnaJednotka(std::string paNazov, int paId, int paPocetMuzov[5], int paPocetZien[5])
{
	this->nazov = paNazov;
	this->id = paId;

	for (int i = 0; i < 5; ++i) {
		pocetMuzov[i] = paPocetMuzov[i];
		pocetZien[i] = paPocetZien[i];
		celkovyPocet[i] = pocetMuzov[i] + pocetZien[i];
	}
}

UzemnaJednotka::~UzemnaJednotka()
{
	// polia alokovane uz v hlavicke -> mali by sa zrusit s objektom
}

UzemnaJednotka::UzemnaJednotka(const UzemnaJednotka& zdroj)
{
	this->nazov = zdroj.nazov;
	this->id = zdroj.id;

	for (int i = 0; i < 5; ++i) {
		pocetMuzov[i] = zdroj.pocetMuzov[i];
		pocetZien[i] = zdroj.pocetZien[i];
		celkovyPocet[i] = zdroj.celkovyPocet[i];
	}
}

UzemnaJednotka& UzemnaJednotka::operator=(const UzemnaJednotka& zdroj)
{
	if (this != &zdroj)
	{
		this->nazov = zdroj.nazov;
		this->id = zdroj.id;

		for (int i = 0; i < 5; ++i) {
			pocetMuzov[i] = zdroj.pocetMuzov[i];
			pocetZien[i] = zdroj.pocetZien[i];
			celkovyPocet[i] = zdroj.celkovyPocet[i];
		}
	}
	return *this;
}

bool UzemnaJednotka::operator==(const UzemnaJednotka& other) const
{
	return id == other.id;
}

int UzemnaJednotka::getCelkovyPocet(int rokIndex)
{
	return celkovyPocet[rokIndex];
}

std::string UzemnaJednotka::getNazov()
{
	return this->nazov;
}

int UzemnaJednotka::getId() const
{
	return this->id;
}

int UzemnaJednotka::getPocetMuzov(int rokIndex)
{
	return this->pocetMuzov[rokIndex];
}

int UzemnaJednotka::getPocetZien(int rokIndex)
{
	return this->pocetZien[rokIndex];
}

void UzemnaJednotka::pridajZaznamMuzov(int pocet, int rokIndex)
{
	this->pocetMuzov[rokIndex] += pocet;
	this->celkovyPocet[rokIndex] = this->pocetMuzov[rokIndex] + this->pocetZien[rokIndex];
}

void UzemnaJednotka::pridajZaznamZien(int pocet, int rokIndex)
{
	this->pocetZien[rokIndex] += pocet;
	this->celkovyPocet[rokIndex] = this->pocetMuzov[rokIndex] + this->pocetZien[rokIndex];
}

int UzemnaJednotka::getTyp()
{
	if (this->id == 0)
	{
		return 0;
	}
	else if (this->id >= 1 && this->id <= 9)
	{
		return 10;
	}
	else if (this->id >= 10 && this->id <= 99)
	{
		return 100;
	}
	else if (this->id >= 100 && this->id <= 999)
	{
		return 1000;
	}
	return 10000;
}



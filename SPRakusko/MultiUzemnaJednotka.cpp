#include "MultiUzemnaJednotka.h"
/*
MultiUzemnaJednotka::MultiUzemnaJednotka(UzemnaJednotka& uzj) : uzj_(uzj) , dupSeq_(nullptr)
{

}

MultiUzemnaJednotka::~MultiUzemnaJednotka()
{
	delete dupSeq_;
}

void MultiUzemnaJednotka::pripravSekvenciu()
{
	if (!dupSeq_)
	{
		dupSeq_ = new ds::amt::IS<UzemnaJednotka>();
	}
}

void MultiUzemnaJednotka::pridajDuplikat(UzemnaJednotka& uzj)
{
	this->pripravSekvenciu();
	auto& block = dupSeq_->insertLast();
	block.data_ = uzj;
}

UzemnaJednotka& MultiUzemnaJednotka::getPovodna()
{
	return uzj_;
}

ds::amt::IS<UzemnaJednotka>* MultiUzemnaJednotka::getDuplicitne()
{
	return dupSeq_;
}*/

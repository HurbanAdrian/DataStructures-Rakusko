#pragma once
#include <libds/amt/explicit_hierarchy.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "UzemnaJednotka.h"
#include <libds/adt/table.h>
#include "MultiUzemnaJednotka.h"
#include <libds/amt/implicit_sequence.h>
#include <libds/adt/sorts.h>

#include <locale>
#include <functional>
#include <iterator>

using TableUsing = ds::adt::Treap<std::string, MultiUzemnaJednotka>;		// u mojej to neislo lebo treap nema insert? cez pretazenie metody to teraz ide v treape 

using IteratorMWEH = ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator;

static inline const std::locale locAt("German_Austria.1250");

class Algoritmus
{
private:
	std::vector<UzemnaJednotka> obce;
	std::ifstream citac;
	std::ofstream pisac;

	ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*> hierarchia;

	TableUsing* tableObce = nullptr;					// musime nastavit na nullptr
	TableUsing* tableRegiony = nullptr;
	TableUsing* tableRepubliky = nullptr;
	TableUsing* tableGeo = nullptr;

	ds::adt::ShellSort<UzemnaJednotka*> sorter;
public:
	Algoritmus();
	~Algoritmus();

	Algoritmus(const Algoritmus& zdroj);
	Algoritmus& operator=(const Algoritmus& zdroj);

	void nacitaj(std::string nazvySuborov[5]);
	bool containsStr(UzemnaJednotka paObec, std::string paRetazec);
	bool hasMaxResidents(UzemnaJednotka paObec, int maxPocet, int rokIndex);
	bool hasMinResidents(UzemnaJednotka paObec, int minPocet, int rokIndex);

	bool hasType(UzemnaJednotka uzJed, int paTyp);
	void nacitajHierarchiu(std::string nazovSuboruObce, std::string nazovSuboruUzemie);
	UzemnaJednotka* najdiObecPodlaId(int id);
	bool jePointerNaObec(UzemnaJednotka* ptr);
	void vypisObceHierarchia(int paRok);

	void spustiAgregaciu();
	std::pair<int, int> agregujObyvatelov(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* uzol, int indexRoku);
	void resetniHodnotyVrcholov(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* uzol);

	void naplnTabulky();
	void prejdiNodeAZaplnTabulku(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* node);
	void najdiVTabulkach(std::string paNazov, int paTyp, int paRok);

	void konzola();
	ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator getIterator(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* uzol) {
		ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator it(&this->hierarchia, uzol);
		return it;
	};
	ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator getNullptrIterator() {
		return ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>::PreOrderHierarchyIterator(&this->hierarchia, nullptr);
	}

	template <typename Predicate>
	ds::amt::IS<UzemnaJednotka*> filter(IteratorMWEH begin, IteratorMWEH end, Predicate paPredikat) {
		ds::amt::IS<UzemnaJednotka*> prefiltrovaneObce;
		for (auto it = begin; it != end; ++it) {
			if (paPredikat(*it)) {
				//prefiltrovaneObce.push_back(*it);
				//std::cout << "Adding block.data_ = " << *it << std::endl;
				auto& block = prefiltrovaneObce.insertLast();
				block.data_ = *it;
			}
		}
		return prefiltrovaneObce;
	};

	template<typename Predikat>
	std::vector<UzemnaJednotka> filter(std::vector<UzemnaJednotka>& paObce, Predikat paPredikat) {
		std::vector<UzemnaJednotka> prefiltrovaneObce;
		for (const auto& obec : paObce)
		{
			if (paPredikat(obec))
			{
				prefiltrovaneObce.push_back(obec);
			}
		}
		return prefiltrovaneObce;
	};
};
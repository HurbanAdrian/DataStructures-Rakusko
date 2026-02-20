#include "Algoritmus.h"

Algoritmus::Algoritmus()
{
    this->tableObce = new ds::adt::Treap<std::string, MultiUzemnaJednotka>();
    this->tableRepubliky = new TableUsing();
    this->tableRegiony = new TableUsing();
    this->tableGeo = new TableUsing();
}

Algoritmus::~Algoritmus()
{
	if (this->citac.is_open())
	{
		this->citac.close();
	}
    for (auto it = hierarchia.begin(); it != hierarchia.end(); ++it)
    {
        UzemnaJednotka* ptr = *it;

        // maz len tie, ktoré NIE SÚ vo vektore obce
        if (!jePointerNaObec(ptr)) {
            delete ptr;
        }
    }

    delete this->tableGeo;
    delete this->tableObce;
    delete this->tableRegiony;
    delete this->tableRepubliky;

    this->hierarchia.clear();
}

Algoritmus::Algoritmus(const Algoritmus& zdroj)
{
	this->obce = zdroj.obce;
    this->hierarchia.assign(zdroj.hierarchia);

	/*this->obce.clear();
	for (auto& obec : zdroj.obce)
	{
		obce.push_back(obec);
	}*/
}

Algoritmus& Algoritmus::operator=(const Algoritmus& zdroj)
{
	if (this != &zdroj)
	{
		obce = zdroj.obce;
        this->hierarchia.assign(zdroj.hierarchia);
	}
	return *this;
}

void Algoritmus::nacitaj(std::string nazvySuborov[5])
{
    for (int i = 0; i < 5; i++)
    {
        this->citac.open(nazvySuborov[i]);

        if (!citac.is_open())
        {
            std::cout << "Nazov suboru je chybny!" << std::endl;
            continue;
        }

        std::string riadok;
        std::getline(citac, riadok);

        while (std::getline(citac, riadok)) {
            //std::cout << riadok << std::endl;

            std::stringstream ss(riadok);
            std::string nazovStr, kodStr, muziPocStr, medziStlpec, zenyPocStr;

            std::getline(ss, nazovStr, ';');
            std::getline(ss, kodStr, ';');

            kodStr.erase(remove(kodStr.begin(), kodStr.end(), '<'), kodStr.end());
            kodStr.erase(remove(kodStr.begin(), kodStr.end(), '>'), kodStr.end());

            std::getline(ss, muziPocStr, ';');
            std::getline(ss, medziStlpec, ';');
            std::getline(ss, zenyPocStr, ';');          //s

            //std::cout << nazovStr << " " << kodStr << " " << muziPocStr << " " << zenyPocStr << " " << std::endl;

            if (kodStr.empty() || muziPocStr.empty() || zenyPocStr.empty() || muziPocStr == "-" || zenyPocStr == "-")
            {
                //std::cout << "Problemovy riadok" << std::endl;
                continue;
            }

            int kod = std::stoi(kodStr);
            int muzi = std::stoi(muziPocStr);
            int zeny = std::stoi(zenyPocStr);

            auto it = std::find_if(obce.begin(), obce.end(), [kod](UzemnaJednotka& obec) {            //predikat zase?    []co chcem ()typ {}kod
                return obec.getId() == kod;                 // ak sa najde zhoda vrati iterator na danu obec inac vrati obec.end()
            });

            if (it != obce.end())
            {
                it->pridajZaznamMuzov(muzi, i);
                it->pridajZaznamZien(zeny, i);
            }
            else
            {
                int arrM[5] = {};
                int arrZ[5] = {};
                arrM[i] = muzi;
                arrZ[i] = zeny;
                UzemnaJednotka novaObec(nazovStr, kod, arrM, arrZ);
                obce.push_back(novaObec);
            }
        }

        this->citac.close();
    }
}

bool Algoritmus::containsStr(UzemnaJednotka paObec, std::string paRetazec)
{
    return paObec.getNazov().find(paRetazec) != -1;              // alebo aj std::string::npos
}

bool Algoritmus::hasMaxResidents(UzemnaJednotka paObec, int maxPocet, int rokIndex)
{
    return paObec.getCelkovyPocet(rokIndex) <= maxPocet;
}

bool Algoritmus::hasMinResidents(UzemnaJednotka paObec, int minPocet, int rokIndex)
{
    return paObec.getCelkovyPocet(rokIndex) >= minPocet;
}

bool Algoritmus::hasType(UzemnaJednotka uzJed, int paTyp)           // vzdy zadam vrchnu hranicu cize 0 10 100 1000 a 10000 pre obce
{
    int id = uzJed.getId();

    switch (paTyp) {
    case 0:                                                                 // Rakúsko
        return id == 0;

    case 10:                                                                // Geografické členenie (napr. 1–9)
        return id >= 1 && id <= 9;

    case 100:                                                               // Spolkové republiky (napr. 11–99)
        return id >= 10 && id <= 99;

    case 1000:                                                              // Regióny (napr. 100–999)
        return id >= 100 && id <= 999;

    case 10000:                                                             // Obce (1000+)
        return id >= 1000;

    default:
        return false;                                                       // neznámy typ
    }
}

void Algoritmus::nacitajHierarchiu(std::string nazovSuboruObce, std::string nazovSuboruUzemie)
{
	this->citac.open(nazovSuboruUzemie);            //UZEMIE
	if (!this->citac.is_open())
	{
		std::cout << "Nazov suboru je chybny!" << std::endl;
		return;
	}

	std::string riadok;

    //vytvorenie korena
	ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>& koren = hierarchia.emplaceRoot();
    int arrM[5] = {};
    int arrZ[5] = {};
    hierarchia.accessRoot()->data_ = new UzemnaJednotka("Rakusko", 0, arrM, arrZ);

	while (std::getline(citac, riadok)) {
        std::stringstream ss(riadok);
        std::string nazovStr, kodStr;
        int cisloAT;

		std::getline(ss, nazovStr, ';');
		std::getline(ss, kodStr, ';');

        kodStr.erase(remove(kodStr.begin(), kodStr.end(), '<'), kodStr.end());
        kodStr.erase(remove(kodStr.begin(), kodStr.end(), '>'), kodStr.end());
        kodStr = kodStr.substr(2);

		cisloAT = std::stoi(kodStr);
		
        if (cisloAT < 10)
        {
			std::string idStr = std::to_string(cisloAT);
            size_t index1 = (kodStr[0] - '1');       //upravit na 0
            auto& syn = hierarchia.emplaceSon(koren, index1);
			syn.data_ = new UzemnaJednotka(nazovStr, cisloAT, arrM, arrZ);
			//std::cout << "Geograficke delenie: " << nazovStr << " INDEX -> " << index1 << std::endl;
        }
        else if (cisloAT < 100) {
            std::string idStr = std::to_string(cisloAT);
            size_t index1 = (kodStr[0] - '1');
            size_t index2 = (kodStr[1] - '1');

            auto* otec = hierarchia.accessSon(koren, index1);
            if (otec == nullptr)
            {
                std::cout << "Chyba pre: " << nazovStr << " neexistuje pri indexe " << index1 << std::endl;
            }
            else
            {
                auto& syn = hierarchia.emplaceSon(*otec, index2);
                syn.data_ = new UzemnaJednotka(nazovStr, cisloAT, arrM, arrZ);
                //std::cout << "Spolkova republika: " << nazovStr << " Otec: " << otec->data_->getNazov() << " INDEX -> " << index2 << std::endl;

                /* ak je to Viedeň(13)  
                if (cisloAT == 13) {
                    auto& regionVienna = hierarchia.emplaceSon(syn, 0); // region na indexe 0
                    regionVienna.data_ = new UzemnaJednotka("Región Viedeň", 130, {}, {});
                    std::cout << "Vytvorený fiktívny región pre Viedeň" << std::endl;
                }*/
            }
        }
        else if (cisloAT < 1000)
        {
            std::string idStr = std::to_string(cisloAT);
            size_t index1 = (kodStr[0] - '1');
            size_t index2 = (kodStr[1] - '1');
            size_t index3 = kodStr[2] <= '0' ? 0 : (kodStr[2] - '1');

            auto* geo = hierarchia.accessSon(koren, index1);
            auto* rep = hierarchia.accessSon(*geo, index2);

            auto& syn = hierarchia.emplaceSon(*rep, index3);
            syn.data_ = new UzemnaJednotka(nazovStr, cisloAT, arrM, arrZ);
            //std::cout << "Región: " << nazovStr << " Pod: " << rep->data_->getNazov() << " INDEX -> " << index3 << std::endl;
        }
	}
	this->citac.close();
    /*
    *     OBEC
    */
    std::cout << "Uzemia nacitolo ---------------------------------------------------------------------------------------" << std::endl;
    
    this->citac.open(nazovSuboruObce);
    if (!this->citac.is_open())
    {
        std::cout << "Nazov suboru obce je chybny!" << std::endl;
        return;
    }

    while (std::getline(citac, riadok)) {
        std::stringstream ss(riadok);
        std::string idStr, nazovStr, regionKodStr;
        std::getline(ss, nazovStr, ';');
        std::getline(ss, idStr, ';');
        std::getline(ss, regionKodStr, ';');

        idStr.erase(std::remove(idStr.begin(), idStr.end(), '<'), idStr.end());
        idStr.erase(std::remove(idStr.begin(), idStr.end(), '>'), idStr.end());
        regionKodStr.erase(std::remove(regionKodStr.begin(), regionKodStr.end(), '<'), regionKodStr.end());
        regionKodStr.erase(std::remove(regionKodStr.begin(), regionKodStr.end(), '>'), regionKodStr.end());

        regionKodStr = regionKodStr.substr(2);

        int id = std::stoi(idStr);
        int regionKod = std::stoi(regionKodStr);

        // indexovanie z region kódu
        std::string kodStr = std::to_string(regionKod);
        size_t index1 = (kodStr[0] - '1');
        size_t index2 = (kodStr[1] - '1');
        size_t index3 = kodStr[2] <= '0' ? 0 : (kodStr[2] - '1');
        

        auto* geo = hierarchia.accessSon(koren, index1);
        if (!geo) {
            std::cout << "Chýba GEO na indexe " << index1 << std::endl;
            continue;
        }
        auto* rep = hierarchia.accessSon(*geo, index2);
        if (!rep) {
            std::cout << "Chýba republika na indexe " << index2 << " pod " << geo->data_->getNazov() << std::endl;
            continue;
        }
        auto* region = hierarchia.accessSon(*rep, index3);
        if (!region) {
            std::cout << "Chýba region na indexe " << index3 << " pod " << rep->data_->getNazov() << std::endl;
            continue;
        }

        UzemnaJednotka* obecPtr = najdiObecPodlaId(id);

        if (obecPtr != nullptr) {
            auto& obec = hierarchia.emplaceSon(*region, region->sons_->size());             // najskor 0 postupne zvysuje
            obec.data_ = obecPtr;
        }

        //auto& obec = hierarchia.emplaceSon(*region, region->sons_->size());             // najskor 0 postupne zvysuje
        //obec.data_ = UzemnaJednotka(nazovStr, id, std::vector<int>{}, std::vector<int>{});
        // 
        //std::cout << "Obec: " << nazovStr << " Priradená k regiónu: " << region->data_->getNazov() << std::endl;
    }

    this->citac.close();
    
    std::cout << "Obce nacitolo ---------------------------------------------------------------------------------------" << std::endl;
}

UzemnaJednotka* Algoritmus::najdiObecPodlaId(int id)
{
    auto it = std::find_if(obce.begin(), obce.end(), [id](const UzemnaJednotka& obec) {
        return obec.getId() == id;
        });

    return (it != obce.end()) ? &(*it) : nullptr;
}

bool Algoritmus::jePointerNaObec(UzemnaJednotka* ptr)
{
    return std::any_of(obce.begin(), obce.end(), [ptr](UzemnaJednotka& o) {         // any of ci splna podmienku predikat
        return &o == ptr;
        });
}

void Algoritmus::vypisObceHierarchia(int paRok)
{
    for (auto it = hierarchia.begin(); it != hierarchia.end(); ++it)
    {
        UzemnaJednotka* uzJed = *it;
        std::cout << uzJed->getNazov()
            << " - Muži: " << uzJed->getPocetMuzov(paRok)
            << ", Ženy: " << uzJed->getPocetZien(paRok) << std::endl;
    }
}

void Algoritmus::spustiAgregaciu()
{
    auto* koren = hierarchia.accessRoot();
    if (koren != nullptr) {
        for (int rok = 0; rok < 5; ++rok) {                                 // agregujeme kazdy rok
            resetniHodnotyVrcholov(koren);
            auto [muzi, zeny] = agregujObyvatelov(koren, rok);
            std::cout << "Agregácia hotová. Rakúsko(" << (2020 + rok) << "): " << muzi << " muži, " << zeny << " ženy." << std::endl;
        }
    }
}

std::pair<int, int> Algoritmus::agregujObyvatelov(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* uzol, int indexRoku)
{
    int celkovyPocetMuzi = 0;
    int celkovyPocetZeny = 0;

    bool jeList = (uzol->sons_ == nullptr || uzol->sons_->size() == 0);

    if (jeList && uzol->data_ != nullptr)                   // beriem LEN pre prvy rok agregujem
    {
        celkovyPocetMuzi += uzol->data_->getPocetMuzov(indexRoku);
        celkovyPocetZeny += uzol->data_->getPocetZien(indexRoku);
    }
    else if (uzol->sons_ != nullptr)                        // ak nie sme na liste tak zavolame agregaciu pre kazdeho syna a spocitame jeho pocty
    {
        for (auto* syn : *uzol->sons_)
        {
            auto [muzi, zeny] = agregujObyvatelov(syn, indexRoku);
            celkovyPocetMuzi += muzi;
            celkovyPocetZeny += zeny;
        }
    }

    // nastav výsledky do uzla
    if (uzol->data_ != nullptr)
    {
        uzol->data_->pridajZaznamMuzov(celkovyPocetMuzi, indexRoku);
        uzol->data_->pridajZaznamZien(celkovyPocetZeny, indexRoku);
    }
    return { celkovyPocetMuzi, celkovyPocetZeny };
}

void Algoritmus::resetniHodnotyVrcholov(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* uzol)
{
    if (uzol->data_ != nullptr)
    {
        bool jeList = (uzol->sons_ == nullptr || uzol->sons_->size() == 0);

        if (!jeList)  // ak to NIE JE obec
        {
            for (int i = 0; i < 5; ++i) {
                uzol->data_->pridajZaznamMuzov(0, i);
                uzol->data_->pridajZaznamZien(0, i);
            }
        }
    }

    if (uzol->sons_ != nullptr)
    {
        for (auto* syn : *uzol->sons_) {
            resetniHodnotyVrcholov(syn);
        }
    }
    
}

void Algoritmus::naplnTabulky()
{
    // spustime prechadzanie od korena
    prejdiNodeAZaplnTabulku(hierarchia.accessRoot());
    std::cout << "Tabulky naplnene" << std::endl;
    std::cout << std::endl;
}

void Algoritmus::prejdiNodeAZaplnTabulku(ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* node)
{
    if (!node)              // ak neexistuje
    {
        return;
    }

    UzemnaJednotka* uj = node->data_;
    int typ = uj->getTyp();  // hodnoty 0/10/100/1000/10000     od rakuska po obce
    switch (typ) {
    case 0:                      // koren by nemal nic byt
        break;
    case 10:
        //std::cout << "Vkladam do tabulky GEO uzem. jednotku " << uj->getId() << " s klucom " << uj->getNazov() << std::endl;
        tableGeo->insert(uj->getNazov(), MultiUzemnaJednotka(*uj), true);
        break;
    case 100:
        //std::cout << "Vkladam do tabulky REPUBLIKY uzem. jednotku " << uj->getId() << " s klucom " << uj->getNazov() << std::endl;
        tableRepubliky->insert(uj->getNazov(), MultiUzemnaJednotka(*uj), true);
        break;
    case 1000:
        //std::cout << "Vkladam do tabulky REGIONY uzem. jednotku " << uj->getId() << " s klucom " << uj->getNazov() << std::endl;
        tableRegiony->insert(uj->getNazov(), MultiUzemnaJednotka(*uj), true);
        break;
    case 10000:
        //std::cout << "Vkladam do tabulky OBCE uzem. jednotku " << uj->getId() << " s klucom " << uj->getNazov() << std::endl;
        tableObce->insert(uj->getNazov(), MultiUzemnaJednotka(*uj), true);
        break;
    default:
        throw std::invalid_argument("Neznamy typ");
    }

    // cheme zopakovat pre cely strom
    size_t degree = hierarchia.degree(*node);           // piseme pred lebo Block* node alebo Block *node ale NIE Block node*
    for (size_t i = 0; i < degree; i++)
    {
        prejdiNodeAZaplnTabulku(hierarchia.accessSon(*node, i));
    }
}

void Algoritmus::najdiVTabulkach(std::string paNazov, int paTyp, int paRok)
{
    bool najdeny = false;

    switch (paTyp)
    {
    case 10:
    {           // zaobalujem este raz lebo muj je viac krat vytvarany ako insatncia
        MultiUzemnaJednotka* muj = nullptr;
        if (tableGeo->tryFind(paNazov, muj))
        {
            auto& orig = muj->getPovodna();
            std::cout << "|10| GEO : " << orig.getNazov() << " ID = " << orig.getId() << std::endl;
            std::cout << orig.getCelkovyPocet(paRok) << " M -> " << orig.getPocetMuzov(paRok) << " Ž -> " << orig.getPocetZien(paRok) << std::endl;

            auto* seq = muj->getDuplicitne();  // nullptr ak zianedne duplicity
            if (seq)
            {
                //std::cout << "Duplicitné záznamy pre \"" << paNazov << "\":" << std::endl;
                for (auto it = seq->begin(); it != seq->end(); ++it)
                {
                    auto& dup = *it;
                    //std::cout << "  - " << dup.getNazov() << " (ID=" << dup.getId() << ")" << std::endl;
                    std::cout << "|10| GEO : " << dup.getNazov() << " ID = " << dup.getId() << std::endl;
                    std::cout << dup.getCelkovyPocet(paRok) << " M -> " << dup.getPocetMuzov(paRok) << " Ž -> " << dup.getPocetZien(paRok) << std::endl;
                }
            }
            najdeny = true;
        }
    }
    break;
    case 100:
    {
        MultiUzemnaJednotka* muj = nullptr;
        if (tableRepubliky->tryFind(paNazov, muj))
        {
            auto& orig = muj->getPovodna();
            std::cout << "|100| REPUBLIKA: " << orig.getNazov() << " ID = " << orig.getId() << std::endl;
            std::cout << orig.getCelkovyPocet(paRok) << " M -> " << orig.getPocetMuzov(paRok) << " Ž -> " << orig.getPocetZien(paRok) << std::endl;

            auto* seq = muj->getDuplicitne();
            if (seq)
            {
                for (auto it = seq->begin(); it != seq->end(); ++it)
                {
                    auto& dup = *it;
                    //std::cout << "  - " << dup.getNazov() << " (ID=" << dup.getId() << ")" << std::endl;
                    std::cout << "|100| REPUBLIKA : " << dup.getNazov() << " ID = " << dup.getId() << std::endl;
                    std::cout << dup.getCelkovyPocet(paRok) << " M -> " << dup.getPocetMuzov(paRok) << " Ž -> " << dup.getPocetZien(paRok) << std::endl;
                }
            }
            najdeny = true;
        }
    }
    break;
    case 1000:
    {
        MultiUzemnaJednotka* muj = nullptr;
        if (tableRegiony->tryFind(paNazov, muj))
        {
            auto& orig = muj->getPovodna();
            std::cout << "|1000| REGION   : " << orig.getNazov() << " ID = " << orig.getId() << std::endl;
            std::cout << orig.getCelkovyPocet(paRok) << " M -> " << orig.getPocetMuzov(paRok) << " Ž -> " << orig.getPocetZien(paRok) << std::endl;

            auto* seq = muj->getDuplicitne();
            if (seq)
            {
                for (auto it = seq->begin(); it != seq->end(); ++it)
                {
                    auto& dup = *it;
                    //std::cout << "  - " << dup.getNazov() << " (ID=" << dup.getId() << ")" << std::endl;
                    std::cout << "|1000| REGION : " << dup.getNazov() << " ID = " << dup.getId() << std::endl;
                    std::cout << dup.getCelkovyPocet(paRok) << " M -> " << dup.getPocetMuzov(paRok) << " Ž -> " << dup.getPocetZien(paRok) << std::endl;
                }
            }
            najdeny = true;
        }
    }
    break;
    case 10000:
    {
        MultiUzemnaJednotka* muj = nullptr;
        if (tableObce->tryFind(paNazov, muj))
        {
            auto& orig = muj->getPovodna();
            std::cout << "|10000| OBEC : " << orig.getNazov() << " ID = " << orig.getId() << std::endl;
            std::cout << orig.getCelkovyPocet(paRok) << " M -> " << orig.getPocetMuzov(paRok) << " Ž -> " << orig.getPocetZien(paRok) << std::endl;

            auto* seq = muj->getDuplicitne();
            if (seq)
            {
                for (auto it = seq->begin(); it != seq->end(); ++it)
                {
                    auto& dup = *it;
                    //std::cout << "  - " << dup.getNazov() << " (ID=" << dup.getId() << ")" << std::endl;
                    std::cout << "|10000| OBEC : " << dup.getNazov() << " ID = " << dup.getId() << std::endl;
                    std::cout << dup.getCelkovyPocet(paRok) << " M -> " << dup.getPocetMuzov(paRok) << " Ž -> " << dup.getPocetZien(paRok) << std::endl;
                }
            }
            najdeny = true;
        }
    }
    break;
    default:
        std::cout << "Neznamy typ" << std::endl;
        break;
    }
    if (!najdeny)
    {
        std::cout << "Nenasiel som ziaden zaznam pre " << paNazov << " v type " << paTyp << std::endl;
    }
}


void Algoritmus::konzola()
{
    ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* aktualny = hierarchia.accessRoot();
    std::string prikaz;

    while (true)
    {
        std::cout << "Aktuálny uzol: " << aktualny->data_->getNazov() << std::endl;

        std::cout << "Príkazy: up | down <i> | print | list | filter | tabulka | exit> " << std::endl;
        std::cin >> prikaz;
        std::cout << "-----------------------------------------------------------------------------------------------" << std::endl;
        if (prikaz == "up")
        {
            if (aktualny->parent_ != nullptr) {
                aktualny = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>*>(aktualny->parent_);
                std::cout << "Ideme hore" << std::endl;
            }
            else
            {
                std::cout << "Sme v koreni" << std::endl;
            }

        }
        else if (prikaz == "down")
        {
            size_t index;
            std::cin >> index;
            std::cout << "Ideme dole na " << index << std::endl;
            if (aktualny->sons_ != nullptr && index < aktualny->sons_->size())
            {
                //aktualny = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>*>(aktualny->sons_->access(index));
                auto* block = aktualny->sons_->access(index);
                if (block && block->data_) {
                    aktualny = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>*>(block->data_);
                    std::cout << "Zisli sme dole na vrchol: " << aktualny->data_->getNazov() << std::endl;
                }
                if (index >= aktualny->sons_->size())
                {
                    std::cout << "Zadany index je mimo rozsah!" << std::endl;
                }
            }
            else
            {
                std::cout << "Neplatny index alebo uzol nema potomkov!" << std::endl;
            }

        }
        else if (prikaz == "print") {
            std::cout << "Názov: " << aktualny->data_->getNazov() << std::endl;
            int rok;
            std::cout << "Zadaj rok (2020-2024): ";
            std::cin >> rok;
            int idx = rok - 2020;
            if (idx < 0 || idx >= 5) {
                std::cout << "Neplatny rok." << std::endl;
            }
            else {
                std::cout << "Muži: " << aktualny->data_->getPocetMuzov(idx)
                    << ", Ženy: " << aktualny->data_->getPocetZien(idx) << std::endl;
            }
        }
        else if (prikaz == "list") {
            if (aktualny->sons_ && aktualny->sons_->size() > 0) {
                std::cout << "Deti: " << std::endl;
                for (size_t i = 0; i < aktualny->sons_->size(); ++i) {
                    auto* block = aktualny->sons_->access(i);
                    if (block && block->data_) {
                        auto* child = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>*>(block->data_);
                        std::cout << i << ": " << child->data_->getNazov() << std::endl;
                    }
                }
            }
            else {
                std::cout << "Tento uzol nema ziadne deti." << std::endl;
            }
        }
        else if (prikaz == "filterv") {
            std::vector<UzemnaJednotka> prefiltrovaneObce = filter(obce, [&](const UzemnaJednotka obec) { return hasMinResidents(obec, 3000, 0); });

            std::cout << "Obce splnajuce podmienky:" << std::endl;
            for (auto& obec : prefiltrovaneObce)
            {
                std::cout << obec.getNazov() << " <" << obec.getId() << "> " << obec.getPocetMuzov(0) << " " << obec.getPocetZien(0) << std::endl;      // tu sme na 0 indexe lebo mame iba tento rok
            }
        }
        else if (prikaz == "filter")
        {
            //using Iterator = ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator;
            //ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator begin = hierarchia.beginPre();
            //auto end = hierarchia.endPre();
            std::cout << "Filter" << std::endl;

            int rok;
            std::cout << "Zadaj rok pre filter (2020-2024): ";
            std::cin >> rok;
            int idx = rok - 2020;
            if (idx < 0 || idx >= 5) {
                std::cout << "Neplatny rok." << std::endl;
                continue;
            }

            std::cout << "Vyber filter:" << std::endl;
            std::cout << "1 - Viac ako X obyvateľov" << std::endl;
            std::cout << "2 - Názov obsahuje reťazec" << std::endl;
            std::cout << "3 - Menej ako X obyvateľov" << std::endl;
            std::cout << "4 - Typ do ID hranice" << std::endl;
            int vyber;
            std::cin >> vyber;

            auto it = getIterator(aktualny);
            auto end = getNullptrIterator();

            std::cout << "Chcete výsledky zoradiť?" << std::endl;
            std::cout << "  1) Abecedne podľa názvu" << std::endl;
            std::cout << "  2) Podľa počtu obyvateľov" << std::endl;
            std::cout << "  0) Bez triedenia" << std::endl;
            std::cout << "Voľba: ";
            int volba;
            std::cin >> volba;

            std::function<bool(UzemnaJednotka*, UzemnaJednotka*)> cmp;
            int rokPop;
            std::string pohlavie;
            std::function<bool(UzemnaJednotka*, UzemnaJednotka*, int, std::string)> cmpPop;

            if (volba == 1)
            {
                auto compareAlphabetical = [](UzemnaJednotka* a, UzemnaJednotka* b) {
                    const auto& coll = std::use_facet<std::collate<char>>(locAt);

                    const std::string& sa = a->getNazov();
                    const std::string& sb = b->getNazov();

                    return coll.compare(sa.data(), sa.data() + sa.size(),
                        sb.data(), sb.data() + sb.size()) < 0;
                    };

                cmp = compareAlphabetical;
            }
            else if (volba == 2)
            {
                std::cout << "Zadaj rok (2020-2024): ";
                std::cin >> rokPop;
                rokPop = rokPop - 2020;

                std::cout << "Zadaj pohlavie (muzi, zeny, spolu): ";
                std::cin >> pohlavie;

                auto comparePopulation = [](UzemnaJednotka* a, UzemnaJednotka* b, int rokPop, std::string pohlavie) {
                    int pa, pb;
                    if (pohlavie == "muzi") {
                        pa = a->getPocetMuzov(rokPop);
                        pb = b->getPocetMuzov(rokPop);
                    }
                    else if (pohlavie == "zeny") {
                        pa = a->getPocetZien(rokPop);
                        pb = b->getPocetZien(rokPop);
                    }
                    else {
                        pa = a->getCelkovyPocet(rokPop);
                        pb = b->getCelkovyPocet(rokPop);
                    }
                    return pa < pb;                 // bool lebo rozdelene budu aj tak rovnako
                    };
                cmpPop = comparePopulation;
            }

            //ds::amt::IS<UzemnaJednotka*> vysledky;

            if (vyber == 1) {
                int minPocet;
                std::cout << "Zadaj minimálny počet obyvateľov: ";
                std::cin >> minPocet;
                auto vysledky = this->filter(it, end, [this, minPocet, idx](UzemnaJednotka* u) {
                    return u && hasMinResidents(*u, minPocet, idx);                               // ak existuje skontrolujeme predikat
                    });

                if (volba != 0) {
                    if (volba == 1) {
                        sorter.sort(vysledky, cmp);
                    }
                    else if (volba == 2) {
                        sorter.sort(vysledky, cmpPop, rokPop, pohlavie);
                    }
                }

                for (UzemnaJednotka* u : vysledky) {
                    int muzi = u->getPocetMuzov(idx);
                    int zeny = u->getPocetZien(idx);
                    int spolu = muzi + zeny;
                    std::cout << "- " << u->getNazov() << " | Muži: " << muzi << ", Ženy: " << zeny << ", Spolu: " << spolu << std::endl;
                }
            }
            else if (vyber == 2) {
                std::string retazec;
                std::cout << "Zadaj reťazec pre vyhľadanie v názve: ";                                  // öf
                std::cin >> retazec;
                auto vysledky = this->filter(it, end, [this, retazec](UzemnaJednotka* u) {
                    return u && containsStr(*u, retazec);
                    });

                if (volba != 0) {
                    if (volba == 1) {
                        sorter.sort(vysledky, cmp);
                    }
                    else if (volba == 2) {
                        sorter.sort(vysledky, cmpPop, rokPop, pohlavie);
                    }
                }

                for (UzemnaJednotka* u : vysledky) {
                    int muzi = u->getPocetMuzov(idx);
                    int zeny = u->getPocetZien(idx);
                    int spolu = muzi + zeny;
                    std::cout << "- " << u->getNazov() << " | Muži: " << muzi << ", Ženy: " << zeny << ", Spolu: " << spolu << std::endl;
                }
            }
            else if (vyber == 3) {
                int maxPocet;
                std::cout << "Zadaj maximálny počet obyvateľov: ";
                std::cin >> maxPocet;
                auto vysledky = this->filter(it, end, [this, maxPocet, idx](UzemnaJednotka* u) {
                    /*if (u && hasMaxResidents(*u, maxPocet, idx))
                    {
                        std::cout << u->getNazov() << std::endl;
                    }*/
                    return u && hasMaxResidents(*u, maxPocet, idx);
                    });

                if (volba != 0) {
                    if (volba == 1) {
                        sorter.sort(vysledky, cmp);
                    }
                    else if (volba == 2) {
                        sorter.sort(vysledky, cmpPop, rokPop, pohlavie);
                    }
                }

                // rovno tu vypiseme lebo operator= neni pri IS
                for (UzemnaJednotka* u : vysledky) {
                    int muzi = u->getPocetMuzov(idx);
                    int zeny = u->getPocetZien(idx);
                    int spolu = muzi + zeny;
                    std::cout << "- " << u->getNazov() << " | Muži: " << muzi << ", Ženy: " << zeny << ", Spolu: " << spolu << std::endl;
                }
            }
            else if (vyber == 4) {
                int typ;
                std::cout << "Zadaj hranicu typu (0 Rakusko 10 GEO 100 Spolk. republiky 1000 Regiony 10000 Obce): ";
                std::cin >> typ;
                auto vysledky = this->filter(it, end, [this, typ](UzemnaJednotka* u) {
                    return u && hasType(*u, typ);
                    });

                if (volba != 0) {
                    if (volba == 1) {
                        sorter.sort(vysledky, cmp);
                    }
                    else if (volba == 2) {
                        sorter.sort(vysledky, cmpPop, rokPop, pohlavie);
                    }
                }

                for (UzemnaJednotka* u : vysledky) {
                    int muzi = u->getPocetMuzov(idx);
                    int zeny = u->getPocetZien(idx);
                    int spolu = muzi + zeny;
                    std::cout << "- " << u->getNazov() << " | Muži: " << muzi << ", Ženy: " << zeny << ", Spolu: " << spolu << std::endl;
                }
            }
            else {
                std::cout << "Neplatná voľba filtra." << std::endl;
                continue;
            }


        }
        else if (prikaz == "exit")
        {
            break;
        }
        else if (prikaz == "tabulka")
        {
            int rok;
            int typ;

            std::cout << "Zadaj rok (2020-2024): "; std::cin >> rok;
            int idx = rok - 2020;
            if (idx < 0 || idx >= 5) {
                std::cout << "Neplatny rok." << std::endl;
            }
            else
            {
                std::cout << "Zadaj typ & nazov: " << std::endl;
                std::cin >> typ;
                std::string nazovUzemia;
                std::cin.ignore();
                std::getline(std::cin, nazovUzemia);

                this->najdiVTabulkach(nazovUzemia, typ, idx);
            }
        }
        else
        {
            std::cout << "Neznámy príkaz." << std::endl;
        }
    }
}
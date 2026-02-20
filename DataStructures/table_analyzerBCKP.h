#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <string>
#include <vector>

#include "C:\Users\AMD Ryzen\OneDrive - Žilinská univerzita v Žiline\AuS\Semestralka\cast3\AUS-VS2022\SPRakusko/MultiUzemnaJednotka.h"



namespace ds::utils
{
    using MultiTable = ds::adt::MultiValueTable<std::string, MultiUzemnaJednotka>;

    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<MultiTable>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(MultiTable& structure, size_t size) override;

        size_t getRandomKey() const;
        int getRandomData() const;

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
        size_t key_;
        int data_;
    };

    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<MultiTable>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(MultiTable& structure) override;
    };

    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<MultiTable>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);
    protected:
        void executeOperation(MultiTable& structure) override;
    };

    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    // ---------------------------------------------------------------------------------------------------------------------

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name)
        : ComplexityAnalyzer<MultiTable>(name),
        rngKey_(144),
        rngData_(144),
        key_(),
        data_(0)
    {
        ComplexityAnalyzer<MultiTable>::registerBeforeOperation([this](MultiTable& tbl) {
            std::uniform_int_distribution<size_t> keyDist(0, tbl.size() ? tbl.size() - 1 : 0);
            //std::uniform_int_distribution<size_t> keyDist(0, tbl.size() - 1);
            //size_t idx = keyDist(rngKey_);
            key_ = keyDist(rngKey_);
            data_ = static_cast<int>(rngData_());
            }
        );
    }

    template<class Table>
    void TableAnalyzer<Table>::growToSize(MultiTable& structure, size_t size)
    {
        for (size_t k = structure.size(); k < size; ++k)
        {
            std::string key = std::to_string(k);
            UzemnaJednotka uj(key, static_cast<int>(k), {}, {});
            //structure.insert(key, MultiUzemnaJednotka(uj));
        }
    }

    template<class Table>
    size_t TableAnalyzer<Table>::getRandomKey() const
    {
        return key_;
    }

    template<class MultiTable>
    int TableAnalyzer<MultiTable>::getRandomData() const
    {
        return data_;
    }


    template<class Table>
    TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name)
        : TableAnalyzer<Table>(name)
    {

    }

    template<class Table>
    void TableInsertAnalyzer<Table>::executeOperation(MultiTable& structure)
    {
        std::string key = std::to_string(this->getRandomKey());
        MultiUzemnaJednotka* muj = nullptr;
        //structure.insert(key, muj);
    }

    template<class Table>
    inline TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name)
    {

    }

    template<class Table>
    inline void TableFindAnalyzer<Table>::executeOperation(MultiTable& structure)
    {
        std::string key = std::to_string(this->getRandomKey());
        MultiUzemnaJednotka* muj = nullptr;
        structure.tryFind(key, muj);
    }

    inline TablesAnalyzer::TablesAnalyzer()
        : CompositeAnalyzer("Tables")
    {
        //this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::MultiValueTable<size_t, MultiUzemnaJednotka>>>("multi-insert"));
        //this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::MultiValueTable<size_t, MultiUzemnaJednotka>>>("multi-find"));
    }

}
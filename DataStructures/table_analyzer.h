#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <string>
#include <vector>



namespace ds::utils
{
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;

        size_t getRandomKey() const;
        int getRandomData() const;

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
        size_t key_;
        int data_;
    };

    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    // ---------------------------------------------------------------------------------------------------------------------

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name)
        : ComplexityAnalyzer<Table>(name),
        rngKey_(144),
        rngData_(144),
        key_(),
        data_(0)
    {
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& tbl) {
            std::uniform_int_distribution<size_t> keyDist(0, tbl.size() ? tbl.size() - 1 : 0);
            //size_t idx = keyDist(rngKey_);
            key_ = keyDist(rngKey_);
            data_ = static_cast<int>(rngData_());
            }
        );
    }

    template<class Table>
    void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
    {
        for (size_t k = structure.size(); k < size; ++k)
        {
            structure.insert(k, rngData_());
        }
    }

    template<class Table>
    size_t TableAnalyzer<Table>::getRandomKey() const
    {
        return key_;
    }

    template<class Table>
    int TableAnalyzer<Table>::getRandomData() const
    {
        return data_;
    }


    template<class Table>
    TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name)
        : TableAnalyzer<Table>(name)
    {

    }

    template<class Table>
    void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
    {
        auto data = this->getRandomData();
        auto key = structure.size();
        structure.insert(key, data);
    }

    template<class Table>
    TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name)
        : TableAnalyzer<Table>(name)
    {

    }

    template<class Table>
    void TableFindAnalyzer<Table>::executeOperation(Table& structure)
    {
        size_t key = this->getRandomKey();
        int* out = nullptr;
        structure.tryFind(key, out);
    }

    inline TablesAnalyzer::TablesAnalyzer()
        : CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<size_t, int>>>("table-insert"));
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::SortedSequenceTable<size_t, int>>>("table-find"));
    }
}
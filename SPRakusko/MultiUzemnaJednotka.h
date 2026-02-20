#pragma once
#include "UzemnaJednotka.h"
#include <libds/amt/implicit_sequence.h>

class MultiUzemnaJednotka
{
private:
	UzemnaJednotka uzj_;
	ds::amt::IS<UzemnaJednotka>* dupSeq_ = nullptr;				// bez duplicity pradzna
public:
    // defalutny pre MemManagera?
    MultiUzemnaJednotka() = default;

    explicit MultiUzemnaJednotka(const UzemnaJednotka& uzj)
        : uzj_(uzj)
    {}

    ~MultiUzemnaJednotka() {
        delete dupSeq_;
    }

    void pripravSekvenciu() {
        if (!dupSeq_)
            dupSeq_ = new ds::amt::ImplicitSequence<UzemnaJednotka>();
    }

    void pridajDuplikat(const UzemnaJednotka& uzj) {
        pripravSekvenciu();
        auto& blk = dupSeq_->insertLast();
        blk.data_ = uzj;
    }

    UzemnaJednotka& getPovodna() { return uzj_; }
    const UzemnaJednotka& getPovodna() const { return uzj_; }

    ds::amt::ImplicitSequence<UzemnaJednotka>*
        getDuplicitne() { return dupSeq_; }
    const ds::amt::ImplicitSequence<UzemnaJednotka>*
        getDuplicitne() const { return dupSeq_; }


    bool operator==(const MultiUzemnaJednotka& o) const {
        return uzj_ == o.uzj_;
    }
    bool operator!=(const MultiUzemnaJednotka& o) const {
        return !(*this == o);
    }
};


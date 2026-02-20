
#include <iostream>
#include <libds/mm/compact_memory_manager.h>
#include <string>
#include <vector>

struct Person       //origo bol student
{
    int vek = 10;
    double id = 10;
    std::string meno = "PlaceHolder";

    Person() = default;
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;
};

struct Student : public Person                      // virtual public Person najlepsia moznost? 
{
    double average;
    std::vector<char> grades;

    Student() = default;
    Student(const Student&) = default;
    Student& operator=(const Student&) = default;
};

int main()
{
    ds::mm::CompactMemoryManager<int> cmm_int;            //short ma 2 bajty moze lahsie pretiect, double vyzera inac ako cely oprtoi primitivnym

    // CTRL R dva krat

    //cmm_int.allocateMemory();

    //int* pointerNaInt = cmm_int.allocateMemory();
    //*pointerNaInt = 10;

    *cmm_int.allocateMemory() = 10;         // vidno endianitu ze 10 je na zaciatku ako najemnej vyznamny aj keby realne mala byt na konci to je to ze podla velkosti 
    *cmm_int.allocateMemory() = 20;         // aby to potom lepsie sedelo s paddingom
    *cmm_int.allocateMemory() = 30;
    *cmm_int.allocateMemory() = 1000;

    cmm_int.releaseMemory();
    cmm_int.releaseMemory();

    *cmm_int.allocateMemory() = 40;         // prepiseme potom 30 lebo posunieme ten last

    cmm_int.print(std::cout);                   // malloc a calloc rozdiel ktore poutijeme, calloc bezpecny, malloc musime prepisat

    //ULOHA 2
    //vytvor triedu 2 atributy int double a potom cmm studentov a vyalokovat jedneho studenta, nechajte vygenerovat implicitny konstruktor a assign operator

    ds::mm::CompactMemoryManager<Person> cmm_person;
    *cmm_person.allocateMemory() = { 10, 1.1, "Adrian"};        // pozor pointer
    cmm_person.print(std::cout);


    //uloha 3 cmm studentov
    ds::mm::CompactMemoryManager<Student> cmm_student;
    *cmm_student.allocateMemory() = { 10, 1.1, "Michal", 2.3, {'A', 'B', 'C'} };
    cmm_student.print(std::cout);
}

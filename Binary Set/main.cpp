#include <iostream>

class BinarySet {
public:
    BinarySet(unsigned int max);
    BinarySet(const BinarySet &b);
    ~BinarySet() {makeNull();};
    void makeNull();
    BinarySet unite(const BinarySet &b);
    BinarySet intersect(const BinarySet &b);
    BinarySet difference(const BinarySet &b);
    BinarySet merge(const BinarySet &b);
    BinarySet &insertNumber(unsigned int number);
    BinarySet &deleteNumber(unsigned int number);
    int minimalElement() const;
    int maximalElement() const;
    BinarySet &assign(const BinarySet &b);
    int equal(const BinarySet &b) const;
    BinarySet &find(unsigned int number, BinarySet &b);
    int member(unsigned int number) const;
    void print() const;
private:
    int size;
    int maximum;
    unsigned int *array;
    int empty(unsigned int start) const;
};

BinarySet fictive(0);

BinarySet::BinarySet(unsigned int max) : maximum(max){
    size = max/(sizeof(unsigned int)*8) + 1;
    array = new unsigned int[size];
    for (int i = 0; i < size; i++)
        array[i] = 0;
}

BinarySet::BinarySet(const BinarySet &b) {
    size = b.size;
    maximum = b.maximum;
    array = new unsigned int [size];
    
    for (int i = 0; i < size; i++)
        array[i] = b.array[i];
}

int BinarySet::empty(unsigned int start) const {
    for (int i = start; i < size; i++)
        if (array[i])
            return 0;
    return 1;
}

BinarySet BinarySet::unite(const BinarySet &b) {
    if (empty(0) && b.empty(0))
        return BinarySet(0);
    
    if (empty(0))
        return b;
    
    if (b.empty(0))
        return *this;
    
    if (size > b.size) {
        BinarySet newSet(*this);
        for (int i = 0; i < b.size; i++)
            newSet.array[i] = array[i] | b.array[i];
        return newSet;
    }
    
    BinarySet newSet(b);
    for (int i = 0; i < size; i++)
        newSet.array[i] = array[i] | b.array[i];
    return newSet;
}

BinarySet BinarySet::intersect(const BinarySet &b) {
    if (empty(0) || b.empty(0))
        return BinarySet(0);
    
    if (size > b.size) {
        BinarySet newSet(b);
        for (int i = 0; i < b.size; i++)
            newSet.array[i] = array[i] & b.array[i];
        return newSet;
    }
    
    BinarySet newSet(*this);
    for (int i = 0; i < size; i++)
        newSet.array[i] = array[i] & b.array[i];
    return newSet;
}

BinarySet BinarySet::difference(const BinarySet &b) {
    if (empty(0) && b.empty(0))
        return BinarySet(0);
    
    if (empty(0))
        return BinarySet(b);
    
    if (b.empty(0))
        return BinarySet(*this);
    
    BinarySet newSet(*this);
    if (size > b.size) {
        for (int i = 0; i < b.size; i++)
            newSet.array[i] = (array[i] ^ b.array[i]) & array[i];
        return newSet;
    }
    
    for (int i = 0; i < size; i++)
        newSet.array[i] = (array[i] ^ b.array[i]) & array[i];
    return newSet;
}

BinarySet BinarySet::merge(const BinarySet &b) {
    if (empty(0) && b.empty(0))
        return BinarySet(0);
    
    if (empty(0))
        return b;
    
    if (b.empty(0))
        return *this;
    
    if (size > b.size) {
        BinarySet newSet(*this);
        for (int i = 0; i < b.size; i++)
            newSet.array[i] = array[i] | b.array[i];
        return newSet;
    }
    
    BinarySet newSet(b);
    for (int i = 0; i < size; i++)
        newSet.array[i] = array[i] | b.array[i];
    return newSet;
}

BinarySet &BinarySet::insertNumber(unsigned int number) {
    int arrayPos = (number - 1) / (sizeof(unsigned int) * 8);
    int elementPos = number % (sizeof(unsigned int) * 8);
    int a = elementPos > 0 ? 1<<(sizeof(unsigned int)*8 - elementPos) : 1;
    
    array[arrayPos] = array[arrayPos] | a;
    
    return *this;
}

BinarySet &BinarySet::deleteNumber(unsigned int number) {
    int arrayPos = (number - 1) / (sizeof(unsigned int) * 8);
    int elementPos = number % (sizeof(unsigned int) * 8);
    int a = elementPos > 0 ? ~(1<<(sizeof(unsigned int)*8 - elementPos)) : ~1;
    
    array[arrayPos] = array[arrayPos] & a;
    
    return *this;
}

int BinarySet::minimalElement() const {
    int mask = 1<<(sizeof(unsigned int)*8 - 1);
    
    for (int i = 0; i < size; i++) {
        if (array[i] > 0)
            for (int j = 0; j < sizeof(unsigned int)*8; j++) {
                if (array[i] & mask)
                    return i*(sizeof(unsigned int)*8)+(j+1);
                mask = mask>>1;
            }
    }
    
    return -1;
}

int BinarySet::maximalElement() const { // ИСПРАВИТЬ
    int mask = 1;
    
    for (int i = size; i > 0; i--) {
        if (array[i] > 0)
            for (int j = sizeof(unsigned int)*8; j > 0; j--) {
                if (array[i] & mask)
                    return i*(sizeof(unsigned int)*8)+(j+1);
                mask = mask<<1;
            }
    }
    
    return -1;
}

BinarySet &BinarySet::assign(const BinarySet &b) {
    if (this == &b)
        return *this;
    
    delete [] array;
    
    maximum = b.maximum;
    size = b.size;
    array = new unsigned int [size];
    for (int i = 0; i < size; i++)
        array[i] = b.array[i];
    
    return *this;
}

int BinarySet::equal(const BinarySet &b) const {
    if (size > b.size) {
        for (int i = 0; i < b.size; i++)
            if (array[i] != b.array[i])
                return 0;
        return empty(b.size);
    }
    
    for (int i = 0; i < size; i++)
        if (array[i] != b.array[i])
            return 0;
    return b.empty(size);
}

BinarySet &BinarySet::find(unsigned int number, BinarySet &b) {
    int arrayPos = (number - 1) / (sizeof(unsigned int) * 8);
    int elementPos = number % (sizeof(unsigned int) * 8);
    int mask = elementPos > 0 ? 1<<(sizeof(unsigned int)*8-elementPos) : 1;
    
    if ((array[arrayPos] & mask) > 0)
        return *this;
    
    if ((b.array[arrayPos] & mask) > 0)
        return b;
    
    return fictive;
}

int BinarySet::member(unsigned int number) const {
    int arrayPos = (number - 1) / (sizeof(unsigned int) * 8);
    int elementPos = number % (sizeof(unsigned int) * 8);
    int mask = elementPos > 0 ? 1<<(sizeof(unsigned int)*8-elementPos) : 1;
    
    if ((array[arrayPos] & mask) > 0)
        return 1;
    
    return 0;
}

void BinarySet::makeNull() {
    size = 0;
    maximum = 0;
    delete [] array;
}

void BinarySet::print() const
{
    unsigned int mask, p;
    for(int i=0; i<size; i++)
    {
        mask = 1<<((sizeof(unsigned int)*8)-1);
        for(int j=0; j<(sizeof(unsigned int)*8); j++)
        {
            p = array[i] & mask;
            if(p == mask)
                std::cout<<(i*(sizeof(unsigned int)*8)+(j+1))<<" ";
            mask = mask>>1;
        }
    }
    std::cout<<std::endl;
}

int main()
{
    BinarySet m(512), m1(100), a(100);
    m.insertNumber(64);
    m.insertNumber(35);
    m.print();
    
    m.insertNumber(6);
    m.insertNumber(23);
    m.insertNumber(4);
    m.insertNumber(112);
    std::cout<<"m: ";
    m.print();
    m1.insertNumber(89);
    m1.insertNumber(4);
    m1.insertNumber(13);
    m1.insertNumber(59);
    m1.insertNumber(23);
    std::cout<<"m1: ";
    m1.print();
    std::cout<<"m + m1: ";
    m.unite(m1).print();
    std::cout<<"m * m1: ";
    m.intersect(m1).print();
    std::cout<<"m - m1: ";
    m.difference(m1).print();
//        m.findElement(59, m1).printMultiplicity();
    std::cout<<"m1 MIN: "<<m1.minimalElement()<<std::endl;
    std::cout<<"m1 MAX: "<<m1.maximalElement()<<std::endl;
    std::cout<<"m = m1: ";
    m.assign(m1).print();
    std::cout<<"m == m1: "<<m.equal(m1)<<std::endl;
}


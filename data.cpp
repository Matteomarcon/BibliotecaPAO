#include "data.h"


Data::Data(int g, int m, int a) : giorno(g), mese(m), anno(a) {}

int Data::getGiorno() const {
    return giorno;
}
int Data::getMese() const {
    return mese;
}
int Data::getAnno() const {
    return anno;
}

void Data::setGiorno(int g) {
    giorno = g;
}
void Data::setMese(int m) {
    mese = m;
}
void Data::setAnno(int a) {
    anno = a;
}

string Data::toString() const {
    return to_string(giorno) + "/" + to_string(mese) + "/" + to_string(anno);
}

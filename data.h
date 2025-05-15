#ifndef DATA_H
#define DATA_H

#include <string>
using std::string;
using std::to_string;

class Data {
private:
    int giorno;
    int mese;
    int anno;

public:
    Data(int g = 1, int m = 1, int a = 2000);
    int getGiorno() const;
    int getMese() const;
    int getAnno() const;
    void setGiorno(int g);
    void setMese(int m);
    void setAnno(int a);

    string toString() const;
};

#endif // DATA_H

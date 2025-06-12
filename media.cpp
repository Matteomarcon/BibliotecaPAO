#include "media.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QBuffer>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>

using std::string;

Media::Media(QString immagine, QString titolo, float prezzo, Data dataPubblicazione, QString genere, bool disponibilita, int copie):
    immagine(immagine), titolo(titolo), prezzo(prezzo), dataPubblicazione(dataPubblicazione), genere(genere), disponibilita(disponibilita), copie(copie){

}

Media::Media() {}

Media::~Media() {}

QString Media::getImmagine() const {
    return immagine;
}
QString Media::getTitolo() const {
    return titolo;
}
float Media::getPrezzo() const {
    return prezzo;
}
QString Media::getData() const {
    return QString::fromStdString(dataPubblicazione.toString());
}
QString Media::getGenere() const {
    return genere;
}
bool Media::getDisponibilita() const {
    return disponibilita;
}
int Media::getCopie() const {
    return copie;
}
void Media::setCopie(int copie) {
    this->copie=copie;
}
void Media::setDisponibilita(bool disponibilita) {
    this->disponibilita=disponibilita;
}

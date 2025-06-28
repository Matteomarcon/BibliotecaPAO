#ifndef GESTOREMEDIA_H
#define GESTOREMEDIA_H

#include "gestorejson.h"

#include <QListWidget>
#include <QFormLayout>
#include <QLabel>

class GestoreMedia {
public:
    GestoreMedia(QListWidget* listaMedia, QListWidget* listaPrestiti, QFormLayout* formLayout, QLabel* imagePreview, QString percorsoJson = "BibliotecaDefault.json");

    static QStringList getTipiDisponibili();
    void caricaFormDaMedia(int indice) const;
    void caricaBiblioteca(QLabel* risultatiLabel);
    void salvaMediaDaForm(const QString& tipo, int indice = -1);
    void modificaMedia(int indice, int copie, bool disponibilita);
    void eliminaMedia(int indice);
    void caricaPrestiti(QLabel* labelRisultatiPrestiti);
    void salvaPrestito(Prestito* prestito);
    void eliminaPrestito(int indice);
    void creaForm(const QString& tipo) const;

private:    
    QListWidget* listaMedia;
    QListWidget* listaPrestiti;
    QFormLayout* formLayout;
    QLabel* imagePreview;
    GestoreJson gestoreJson;

    void creaFormAudiovisivo() const;
    void creaFormCartaceo() const;
    void creaFormFilm() const;
    void creaFormGiornale() const;
    void creaFormLibro() const;
    void creaFormRivista() const;
    void creaFormVinile() const;
    Media* creaFilm(int indice) const;
    Media* creaGiornale(int indice) const;
    Media* creaLibro(int indice) const;
    Media* creaRivista(int indice) const;
    Media* creaVinile(int indice) const;

};

#endif // GESTOREMEDIA_H

#ifndef GESTOREJSON_H
#define GESTOREJSON_H

#include <QJsonObject>
#include <QString>
#include <QObject>

class Media;
class Audiovisivo;
class Cartaceo;
class Giornale;
class Libro;
class Rivista;
class Vinile;
class Film;

class GestoreJson: public QObject
{
Q_OBJECT
private:
    QList<Media*> media;
    QString percorsoFile;
public:


    GestoreJson(const QString& nomeFile);
    QList<Media*> caricaBiblioteca();
    void eliminaMedia(int indice);
    void salvaNuovoMedia(Media* media);
    void salvaMedia(Media* media, QJsonObject& oggetto);
    void salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto);
    void salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto);
    QJsonObject salvaMedia(Libro* libro);
    QJsonObject salvaMedia(Rivista* rivista);
    QJsonObject salvaMedia(Vinile* vinile);
    QJsonObject salvaMedia(Film* film);
    QJsonObject salvaMedia(Giornale* giornale);

    Rivista* caricaRivista(const QJsonObject& jsonObject);
    Film* caricaFilm(const QJsonObject& jsonObject);
    Vinile* caricaVinile(const QJsonObject& jsonObject);
    Giornale* caricaGiornale(const QJsonObject& jsonObject);
    Libro* caricaLibro(const QJsonObject& jsonObject);
};

#endif // GESTOREJSON_H

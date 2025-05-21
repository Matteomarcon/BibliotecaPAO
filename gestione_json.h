#ifndef GESTIONE_JSON_H
#define GESTIONE_JSON_H

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

class gestione_json: public QObject
{
Q_OBJECT
private:
    QList<Media*> media;
    QString percorsoFile;
public:
    gestione_json(const QString& nomeFile);
    void salvaNuovoMedia(Media* media);
    void salvaMedia(Media* media, QJsonObject& obj);
    void salvaMedia(Audiovisivo* audiovisivo, QJsonObject& obj);
    void salvaMedia(Cartaceo* cartaceo, QJsonObject& obj);
    QJsonObject salvaMedia(Libro* libro);
    QJsonObject salvaMedia(Rivista* rivista);
    QJsonObject salvaMedia(Vinile* vinile);
    QJsonObject salvaMedia(Film* film);
    QJsonObject salvaMedia(Giornale* giornale);
};

#endif // GESTIONE_JSON_H

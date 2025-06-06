#include "gestorejson.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QByteArray>
#include "media.h"
#include <QFile>
#include <QBuffer>

#include "film.h"
#include "libro.h"
#include "giornale.h"
#include "rivista.h"
#include "vinile.h"

GestoreJson::GestoreJson(const QString& nomeFile) : percorsoFile(nomeFile) {}

void GestoreJson::eliminaMedia(int indice) {
    QFile file(percorsoFile);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isArray()) return;

    QJsonArray array = doc.array();
    if (indice >= 0 && indice < array.size()) {
        array.removeAt(indice);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;

    file.write(QJsonDocument(array).toJson());
    file.close();
}


void GestoreJson::salvaMedia(Media* media, int indice) {
    if (indice == -1) {
        listaMedia.append(media);
    }
    else {
        qDebug() << indice << " " << listaMedia.size();
        delete listaMedia[indice];
        listaMedia.replace(indice, media);
        qDebug() << "aoaoaoao";
    }

    QJsonObject nuovoOggetto;
    if (typeid(*media)==typeid(Libro)) nuovoOggetto = salvaMedia(static_cast<Libro*>(media));
    else if (typeid(*media)==typeid(Giornale)) nuovoOggetto = salvaMedia(static_cast<Giornale*>(media));
    else if (typeid(*media)==typeid(Vinile)) nuovoOggetto = salvaMedia(static_cast<Vinile*>(media));
    else if (typeid(*media)==typeid(Film)) nuovoOggetto = salvaMedia(static_cast<Film*>(media));
    else if (typeid(*media)==typeid(Rivista)) nuovoOggetto = salvaMedia(static_cast<Rivista*>(media));

    QFile file(percorsoFile);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossibile aprire 1 dati.json");
    }
    QJsonDocument documento = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!documento.isArray()) {
        throw std::runtime_error("Non c'è nessun array in dati.json");
        return;
    }
    QJsonArray jsonArray = documento.array();

    if (indice == -1) {
        jsonArray.append(nuovoOggetto);
    }
    else {
        jsonArray.replace(indice, nuovoOggetto);
    }

    QFile outputFile(percorsoFile);
    QJsonDocument aggiornaDocumento(jsonArray);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Impossibile aprire 2 dati.json");
        return;
    }
    outputFile.write(aggiornaDocumento.toJson());
    outputFile.close();
}

void GestoreJson::salvaMedia(Media* media, QJsonObject& oggetto){
    oggetto["Immagine"] = media->getImmagine();
    oggetto["Titolo"] = media->getTitolo();
    oggetto["Prezzo"] = media->getPrezzo();
    oggetto["Data di pubblicazione"] = media->getData();
    oggetto["Genere"] = media->getGenere();
    oggetto["Disponibilita"] = media->getDisponibilita();
    oggetto["Copie"] = media->getCopie();
}

void GestoreJson::salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto){
    oggetto["Durata"] = audiovisivo->getDurata();
    oggetto["Produzione"] = audiovisivo->getProduzione();
}

void GestoreJson::salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto){
    oggetto["Autore"] = cartaceo->getAutore();
    oggetto["Editore"] = cartaceo->getEditore();
}

QJsonObject GestoreJson::salvaMedia(Libro* libro){
    QJsonObject oggetto;
    oggetto["Classe"]="Libro";
    salvaMedia(static_cast<Media*>(libro), oggetto);
    salvaMedia(static_cast<Cartaceo*>(libro), oggetto);
    QString formato;
    oggetto["Formato"] = libro->getFormato();
    oggetto["Lingua"] = libro->getLingua();
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Rivista* rivista){
    QJsonObject oggetto;
    oggetto["Classe"]="Rivista";
    salvaMedia(static_cast<Media*>(rivista), oggetto);
    salvaMedia(static_cast<Cartaceo*>(rivista), oggetto);
    oggetto["Numero"] = rivista->getNumero();
    oggetto["Periodicita"] = rivista->getPeriodicita();
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Vinile* vinile){
    QJsonObject oggetto;
    oggetto["Classe"]="Vinile";
    salvaMedia(static_cast<Media*>(vinile), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(vinile), oggetto);
    oggetto["Artista"] = vinile->getArtista();
    oggetto["Numero tracce"] = vinile->getNumeroTracce();
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Film* film){
    QJsonObject oggetto;
    oggetto["Classe"]="Film";
    salvaMedia(static_cast<Media*>(film), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(film), oggetto);
    oggetto["Regista"] = film->getRegista();
    oggetto["Lingua originale"] = film->getLinguaOriginale();
    oggetto["Paese produzione"] = film->getPaeseProduzione();
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Giornale* giornale){
    QJsonObject oggetto;
    oggetto["Classe"]="Giornale";
    salvaMedia(static_cast<Media*>(giornale), oggetto);
    salvaMedia(static_cast<Cartaceo*>(giornale), oggetto);
    oggetto["Testata"] = giornale->getTestata();
    return oggetto;
}

QList<Media*> GestoreJson::caricaBiblioteca() {
    QFile file(percorsoFile);

    listaMedia.clear();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossibile aprire il file per la lettura:" << file.errorString();
        return {};
    }

    // Leggi il contenuto del file
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            QJsonObject jsonObject = value.toObject();
            QString classe = jsonObject["Classe"].toString();
            Media* media = nullptr;
            if(classe == "Rivista") media = caricaRivista(jsonObject);
            else if(classe == "Film") media = caricaFilm(jsonObject);
            else if(classe =="Vinile") media = caricaVinile(jsonObject);
            else if(classe =="Libro") media = caricaLibro(jsonObject);
            else if (classe =="Giornale") media = caricaGiornale(jsonObject);

            listaMedia.append(media);
        }
    }
    return listaMedia;
}

Rivista* GestoreJson::caricaRivista(const QJsonObject& jsonObject) {
    Data data;

    return new Rivista(jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       data, jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                       jsonObject["Copie"].toInt(), jsonObject["Autore"].toString(), jsonObject["Editore"].toString(),
                       jsonObject["Numero"].toInt(), jsonObject["Periodicita"].toString());
}
Film* GestoreJson::caricaFilm(const QJsonObject& jsonObject) {
    Data data;

    return new Film(jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       data, jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                    jsonObject["Copie"].toInt(), jsonObject["Durata"].toInt(), jsonObject["Produzione"].toString(), jsonObject["Regista"].toString(),
                       jsonObject["Lingua originale"].toString(), jsonObject["Paese produzione"].toString());
}
Vinile* GestoreJson::caricaVinile(const QJsonObject& jsonObject) {
    Data data;

    return new Vinile(jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       data, jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                       jsonObject["Copie"].toInt(), jsonObject["Durata"].toInt(), jsonObject["Produzione"].toString(),
                       jsonObject["Artista"].toString(), jsonObject["Numero tracce"].toInt());
}
Giornale* GestoreJson::caricaGiornale(const QJsonObject& jsonObject) {
    Data data;

    return new Giornale(jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                    data, jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                    jsonObject["Copie"].toInt(), jsonObject["Autore"].toString(), jsonObject["Editore"].toString(),
                    jsonObject["Testata"].toString());
}
Libro* GestoreJson::caricaLibro(const QJsonObject& jsonObject) {
    Data data;

    return new Libro(jsonObject["Immagine"].toString(), jsonObject["Titolo"].toString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                      data, jsonObject["Genere"].toString(), jsonObject["Disponibilita"].toBool(),
                      jsonObject["Copie"].toInt(), jsonObject["Autore"].toString(), jsonObject["Editore"].toString(),
                      jsonObject["Lingua"].toString(), jsonObject["Formato"].toString());
}
























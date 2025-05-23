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

void GestoreJson::salvaNuovoMedia(Media* nuovoMedia) {
    media.append(nuovoMedia);
    QJsonObject nuovoOggetto;
    if (typeid(*nuovoMedia)==typeid(Libro)) {
        nuovoOggetto = salvaMedia(static_cast<Libro*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Giornale)) {
        nuovoOggetto = salvaMedia(static_cast<Giornale*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Vinile)) {
        nuovoOggetto = salvaMedia(static_cast<Vinile*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Film)) {
        nuovoOggetto = salvaMedia(static_cast<Film*>(nuovoMedia));
    }
    else if (typeid(*nuovoMedia)==typeid(Rivista)) {
        nuovoOggetto = salvaMedia(static_cast<Rivista*>(nuovoMedia));
    }
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
    jsonArray.append(nuovoOggetto);

    QFile outputFile(percorsoFile);
    QJsonDocument aggiornaDocumento(jsonArray);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Impossibile aprire 2 dati.json");
        return;
    }
    outputFile.write(aggiornaDocumento.toJson());
    outputFile.close();

    if (typeid(*nuovoMedia)!=typeid(Vinile)) {
        qDebug() << "Vinile rilevato";
    }
}

void GestoreJson::salvaMedia(Media* media, QJsonObject& oggetto){
    QImage image = media->getImmagine();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QString imageInBase64 = QString::fromLatin1(byteArray.toBase64());
    oggetto["Immagine"] = imageInBase64;

    oggetto["Titolo"] = QString::fromStdString(media->getTitolo());
    oggetto["Prezzo"] = media->getPrezzo();
    oggetto["Data di pubblicazione"] = QString::fromStdString(media->getData().toString());
    oggetto["Genere"] = QString::fromStdString(media->getGenere());
    oggetto["Disponibilita'"] = media->getDisponibilita();
    oggetto["Copie"] = media->getCopie();
}

void GestoreJson::salvaMedia(Audiovisivo* audiovisivo, QJsonObject& oggetto){
    oggetto["Durata"] = audiovisivo->getDurata();
    oggetto["Produzione"] = QString::fromStdString(audiovisivo->getProduzione());
}

void GestoreJson::salvaMedia(Cartaceo* cartaceo, QJsonObject& oggetto){
    oggetto["Autore"] = QString::fromStdString(cartaceo->getAutore());
    oggetto["Editore"] = QString::fromStdString(cartaceo->getEditore());
}

QJsonObject GestoreJson::salvaMedia(Libro* libro){
    QJsonObject oggetto;
    oggetto["Classe"]="Libro";
    salvaMedia(static_cast<Media*>(libro), oggetto);
    salvaMedia(static_cast<Cartaceo*>(libro), oggetto);
    QString formato;
    oggetto["Formato"] = QString::fromStdString(libro->getFormato());
    oggetto["Lingua"] = QString::fromStdString(libro->getLingua());
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Rivista* rivista){
    QJsonObject oggetto;
    oggetto["Classe"]="Rivista";
    salvaMedia(static_cast<Media*>(rivista), oggetto);
    salvaMedia(static_cast<Cartaceo*>(rivista), oggetto);
    oggetto["Numero"] = rivista->getNumero();
    oggetto["Periodicita"] = QString::fromStdString(rivista->getPeriodicita());
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Vinile* vinile){
    QJsonObject oggetto;
    oggetto["Classe"]="Vinile";
    salvaMedia(static_cast<Media*>(vinile), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(vinile), oggetto);
    oggetto["Artista"] = QString::fromStdString(vinile->getArtista());
    oggetto["Numero tracce"] = vinile->getNumeroTracce();
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Film* film){
    QJsonObject oggetto;
    oggetto["Classe"]="Film";
    salvaMedia(static_cast<Media*>(film), oggetto);
    salvaMedia(static_cast<Audiovisivo*>(film), oggetto);
    oggetto["Regista"] = QString::fromStdString(film->getRegista());
    oggetto["Lingua originale"] = QString::fromStdString(film->getLinguaOriginale());
    oggetto["Paese di produzione"] = QString::fromStdString(film->getPaeseProduzione());
    return oggetto;
}

QJsonObject GestoreJson::salvaMedia(Giornale* giornale){
    QJsonObject oggetto;
    oggetto["Classe"]="Giornale";
    salvaMedia(static_cast<Media*>(giornale), oggetto);
    salvaMedia(static_cast<Cartaceo*>(giornale), oggetto);
    oggetto["Testata"] = QString::fromStdString(giornale->getTestata());
    return oggetto;
}

QList<Media*> GestoreJson::caricaBiblioteca() {
    QList<Media*> listaMedia;

    QFile file(percorsoFile);

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
    QImage immagine;
    Data data;

    return new Rivista(immagine, jsonObject["Titolo"].toString().toStdString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       data, jsonObject["Genere"].toString().toStdString(), jsonObject["Disponibilita"].toBool(),
                       jsonObject["Copie"].toInt(), jsonObject["Autore"].toString().toStdString(), jsonObject["Editore"].toString().toStdString(),
                       jsonObject["Numero"].toInt(), jsonObject["Periodicita"].toString().toStdString());
}
Film* GestoreJson::caricaFilm(const QJsonObject& jsonObject) {
    QImage immagine;
    Data data;

    return new Film(immagine, jsonObject["Titolo"].toString().toStdString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       data, jsonObject["Genere"].toString().toStdString(), jsonObject["Disponibilita"].toBool(),
                    jsonObject["Copie"].toInt(), jsonObject["Duarata"].toInt(), jsonObject["Produzione"].toString().toStdString(), jsonObject["Regista"].toString().toStdString(),
                       jsonObject["Lingua originale"].toString().toStdString(), jsonObject["Paese produzione"].toString().toStdString());
}
Vinile* GestoreJson::caricaVinile(const QJsonObject& jsonObject) {
    QImage immagine;
    Data data;

    return new Vinile(immagine, jsonObject["Titolo"].toString().toStdString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                       data, jsonObject["Genere"].toString().toStdString(), jsonObject["Disponibilita"].toBool(),
                       jsonObject["Copie"].toInt(), jsonObject["Duarata"].toInt(), jsonObject["Produzione"].toString().toStdString(),
                       jsonObject["Artista"].toString().toStdString(), jsonObject["Numero tracce"].toInt());
}
Giornale* GestoreJson::caricaGiornale(const QJsonObject& jsonObject) {
    QImage immagine;
    Data data;

    return new Giornale(immagine, jsonObject["Titolo"].toString().toStdString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                    data, jsonObject["Genere"].toString().toStdString(), jsonObject["Disponibilita"].toBool(),
                    jsonObject["Copie"].toInt(), jsonObject["Autore"].toString().toStdString(), jsonObject["Editore"].toString().toStdString(),
                    jsonObject["Testata"].toString().toStdString());
}
Libro* GestoreJson::caricaLibro(const QJsonObject& jsonObject) {
    QImage immagine;
    Data data;

    return new Libro(immagine, jsonObject["Titolo"].toString().toStdString(), static_cast<float>(jsonObject["Prezzo"].toDouble()),
                      data, jsonObject["Genere"].toString().toStdString(), jsonObject["Disponibilita"].toBool(),
                      jsonObject["Copie"].toInt(), jsonObject["Autore"].toString().toStdString(), jsonObject["Editore"].toString().toStdString(),
                      jsonObject["Lingua"].toString().toStdString(), jsonObject["Formato"].toString().toStdString());
}
























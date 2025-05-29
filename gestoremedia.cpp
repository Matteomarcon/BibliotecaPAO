#include "gestoremedia.h"
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QLabel>
#include <typeinfo>
#include <media.h>
#include <film.h>
#include <vinile.h>
#include <rivista.h>
#include <giornale.h>
#include <libro.h>

GestoreMedia::GestoreMedia(QListWidget* lista, QString percorso)
    : gestoreJson(percorso), listaMedia(lista) {}

QStringList GestoreMedia::getTipiDisponibili() const {
    return {"Film", "Giornale", "Libro", "Rivista", "Vinile"};
}

void GestoreMedia::salvaMediaDaForm(QString tipo, QFormLayout *layout) {
    Media* media = nullptr;

    if (tipo == "Film") media = creaFilm(layout);
    else if (tipo == "Giornale"); //media = creaGiornale(layout);
    else if (tipo == "Libro"); //media = creaLibro(layout);
    else if (tipo == "Rivista"); //media = creaRivista(layout);
    else if (tipo == "Vinile"); //media = creaVinile(layout);

    gestoreJson.salvaNuovoMedia(media);
}

Media* GestoreMedia::creaFilm(QFormLayout* layout) {
    QString immagineStr, titoloStr, genereStr, produzioneStr, registaStr, linguaOriginaleStr, paeseProduzioneStr;
    double prezzoVal = 0;
    QDate dataVal;
    bool disponibilitaVal = false;
    int copieVal = 0, durataVal = 0;

    for (int i = 1; i < layout->rowCount(); ++i) {
        QLabel* label = qobject_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
        QWidget* field = layout->itemAt(i, QFormLayout::FieldRole)->widget();

        QString labelText = label->text();

        if (labelText == "Immagine") {
            if (auto lineEdit = qobject_cast<QTextEdit*>(field)) {
                immagineStr = lineEdit->toPlainText();
            }
        } else if (labelText == "Titolo") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                titoloStr = lineEdit->text();
            }
        } else if (labelText == "Prezzo") {
            if (auto doubleSpin = qobject_cast<QDoubleSpinBox*>(field)) {
                prezzoVal = doubleSpin->value();
            }
        } else if (labelText == "Data") {
            if (auto dateEdit = qobject_cast<QDateEdit*>(field)) {
                dataVal = dateEdit->date();
            }
        } else if (labelText == "Genere") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                genereStr = lineEdit->text();
            }
        } else if (labelText == "Disponibilita") {
            if (auto checkBox = qobject_cast<QCheckBox*>(field)) {
                disponibilitaVal = checkBox->isChecked();
            }
        } else if (labelText == "Copie") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                copieVal = spinBox->value();
            }
        } else if (labelText == "Durata") {
            if (auto spinBox = qobject_cast<QSpinBox*>(field)) {
                durataVal = spinBox->value();
            }
        } else if (labelText == "Produzione") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                produzioneStr = lineEdit->text();
            }
        } else if (labelText == "Regista") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                registaStr = lineEdit->text();
            }
        } else if (labelText == "Lingua originale") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                linguaOriginaleStr = lineEdit->text();
            }
        } else if (labelText == "Paese produzione") {
            if (auto lineEdit = qobject_cast<QLineEdit*>(field)) {
                paeseProduzioneStr = lineEdit->text();
            }
        }
    }

    Data dataPubblicazione(dataVal.day(), dataVal.month(), dataVal.year());

    return new Film(immagineStr, titoloStr, static_cast<float>(prezzoVal), dataPubblicazione, genereStr, disponibilitaVal, copieVal,
    durataVal, produzioneStr, registaStr, linguaOriginaleStr, paeseProduzioneStr);
}

void GestoreMedia::caricaBiblioteca() {
    listaMedia->clear();
    QList<Media*> lista = gestoreJson.caricaBiblioteca();

    for (Media* media : lista) {
        QString tipo;

        if (typeid(*media) == typeid(Vinile)) {
            tipo = "Vinile";
        } else if (typeid(*media) == typeid(Film)) {
            tipo = "Film";
        } else if (typeid(*media) == typeid(Rivista)) {
            tipo = "Rivista";
        } else if (typeid(*media) == typeid(Giornale)) {
            tipo = "Giornale";
        } else if (typeid(*media) == typeid(Libro)) {
            tipo = "Libro";
        }

        QString nomeItem = tipo + ":\n" + media->getTitolo();
        QListWidgetItem* item = new QListWidgetItem(media->getIcon(), nomeItem, listaMedia);
        QString base64Immagine = media->getImmagine();
        item->setData(Qt::UserRole, base64Immagine);

        QFont font;
        font.setBold(true);
        item->setFont(font);

        listaMedia->setIconSize(QSize(48, 48));
        listaMedia->addItem(item);
    }

}

void GestoreMedia::creaForm(const QString& tipo, QFormLayout* layout) const {
    if (tipo == "Film") Film::creaForm(layout);
    else if (tipo == "Giornale") Giornale::creaForm(layout);
    else if (tipo == "Libro") Libro::creaForm(layout);
    else if (tipo == "Rivista") Rivista::creaForm(layout);
    else if (tipo == "Vinile") Vinile::creaForm(layout);
}

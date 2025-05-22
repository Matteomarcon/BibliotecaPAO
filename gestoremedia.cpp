#include "gestoremedia.h"
#include <QHBoxLayout>
#include <QLabel>
#include <typeinfo>
#include <media.h>
#include <film.h>
#include <vinile.h>
#include <rivista.h>
#include <giornale.h>
#include <libro.h>

GestoreMedia::GestoreMedia(QListWidget* lista, const QString& percorso)
    : gestoreJson(percorso), listaMedia(lista) {}

void GestoreMedia::inserisciNuovoMedia(Media* media) {
    // Salva nel JSON usando l'istanza gestoreJson
    gestoreJson.salvaNuovoMedia(media);

    // Mostra sulla lista (semplificato)
    QListWidgetItem* item = new QListWidgetItem(listaMedia);
    item->setSizeHint(QSize(200, 60));

    QString titolo = QString::fromStdString("prova");
    QString descrizione = QString::fromStdString("prova2");

    QWidget* placeholder = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(placeholder);
    layout->addWidget(new QLabel(titolo + " - " + descrizione));
    placeholder->setLayout(layout);
    listaMedia->setItemWidget(item, placeholder);
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

        QString nomeItem = tipo + ": " + QString::fromStdString(media->getTitolo());
        QListWidgetItem* item = new QListWidgetItem(media->getIcon(), nomeItem, listaMedia);
        listaMedia->setIconSize(QSize(48, 48));
        listaMedia->addItem(item);
    }

}

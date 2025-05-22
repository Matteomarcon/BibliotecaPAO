#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

private slots:
    void showPaginaPrincipale();
    void showPaginaNuovoMedia();
    void onItemClicked(QListWidgetItem *item);

private:
    QStackedWidget *stackedWidget;
    QWidget *paginaPrincipale;
    QWidget *paginaNuovoMedia;

    QWidget *rightPanel;
    QLabel *labelTitolo;
    QLabel *labelDescrizione;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H

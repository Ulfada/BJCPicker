#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jsonparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void resetcombo();
    void resetstats();
    void similar(QList<QVariant>);
    void stat(QString, QVariant, QString);

private:
    void loadFile();
    void resetValues();
    Ui::MainWindow *ui;
    JSONParser* parser;
};
#endif // MAINWINDOW_H

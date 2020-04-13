#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

const int MAX_OG = 1100;
const int MIN_OG = 1000;
const int MAX_IBU = 100;
const int MAX_SRM = 50;
const int MAX_ABV = 150;
const int ZERO = 0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    this->resetValues();
    this->loadFile();
}

void MainWindow::loadFile()
{
    QFile file("://bjcp-2015.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,
                             tr("Error loading"),
                             tr(file.errorString().toStdString().c_str()));
        return;
    }

    ui->statusbar->showMessage("File loaded");

    parser = new JSONParser(file);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(resetcombo()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(resetstats()));

    connect(ui->pushButton, SIGNAL(clicked()), parser, SLOT(Pick()));

    connect(parser, SIGNAL(category(QString)), ui->lineEdit, SLOT(setText(QString)));
    connect(parser, SIGNAL(id(QString)), ui->lineEdit_2, SLOT(setText(QString)));
    connect(parser, SIGNAL(notes(QString)), ui->textEdit, SLOT(setText(QString)));
    connect(parser, SIGNAL(name(QString)), ui->lineEdit_5, SLOT(setText(QString)));
    connect(parser, SIGNAL(beerid(QString)), ui->lineEdit_4, SLOT(setText(QString)));

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), parser, SLOT(info(int)));
    connect(parser, SIGNAL(info(QString)), ui->textEdit_2, SLOT(setText(QString)));

    connect(parser, SIGNAL(example(QList<QVariant>)), this, SLOT(similar(QList<QVariant>)));
    connect(parser, SIGNAL(stat(QString, QVariant, QString)), this, SLOT(stat(QString, QVariant, QString)));

    connect(parser, SIGNAL(repaint()), this, SLOT(repaint()));
}

void MainWindow::resetValues()
{
    ui->setupUi(this);

    ui->og->setMaximum(MAX_OG);
    ui->og->setMinimum(MIN_OG);

    ui->fg->setMinimum(ZERO);
    ui->fg->setMaximum(MAX_OG);

    ui->ibu->setMinimum(ZERO);
    ui->ibu->setMaximum(MAX_IBU);

    ui->srm->setMinimum(ZERO);
    ui->srm->setMaximum(MAX_SRM);

    ui->abv->setMinimum(ZERO);
    ui->abv->setMaximum(MAX_ABV);
}

void MainWindow::resetcombo()
{
    ui->comboBox->setCurrentIndex(0);
}

void MainWindow::resetstats()
{
    ui->og->setUpperValue(MAX_OG);
    ui->og_high->setValue(ZERO);

    ui->og->setLowerValue(MIN_OG);
    ui->og_low->setValue(ZERO);

    ui->fg->setUpperValue(MAX_OG);
    ui->fg_high->setValue(ZERO);

    ui->fg->setLowerValue(ZERO);
    ui->fg_low->setValue(ZERO);

    ui->ibu->setUpperValue(MAX_IBU);
    ui->ibu_high->setValue(ZERO);

    ui->ibu->setLowerValue(ZERO);
    ui->ibu_low->setValue(ZERO);

    ui->srm->setUpperValue(MAX_SRM);
    ui->srm_high->setValue(ZERO);

    ui->srm->setLowerValue(ZERO);
    ui->srm_low->setValue(ZERO);

    ui->abv->setUpperValue(MAX_ABV);
    ui->abv_high->setValue(ZERO);

    ui->abv->setLowerValue(ZERO);
    ui->abv_low->setValue(ZERO);
}

void MainWindow::similar(QList<QVariant> str)
{
    ui->listWidget->clear();

    for(const auto& beer : str){
        ui->listWidget->addItem(beer.toString());
    }

    ui->listWidget->repaint();
}

void MainWindow::stat(QString type, QVariant val, QString lim)
{
    if(type == "og"){
        if(lim == "high"){
            ui->og->setUpperValue(val.toFloat() * 1000);
            ui->og_high->setValue(val.toFloat());
        } else if(lim == "low"){
            ui->og->setLowerValue(val.toFloat() * 1000);
            ui->og_low->setValue(val.toFloat());
        } else {}
    } else if(type == "fg"){
        if(lim == "high"){
            ui->fg->setUpperValue(val.toFloat() * 1000);
            ui->fg_high->setValue(val.toFloat());
        } else if(lim == "low"){
            ui->fg->setLowerValue(val.toFloat() * 1000);
            ui->fg_low->setValue(val.toFloat());
        } else {}
    } else if(type == "ibu"){
        if(lim == "high"){
            ui->ibu->setUpperValue(val.toInt());
            ui->ibu_high->setValue(val.toInt());
        } else if(lim == "low"){
            ui->ibu->setLowerValue(val.toInt());
            ui->ibu_low->setValue(val.toInt());
        } else {}
    } else if(type == "srm"){
        if(lim == "high"){
            ui->srm->setUpperValue(val.toInt());
            ui->srm_high->setValue(val.toInt());
        } else if(lim == "low"){
            ui->srm->setLowerValue(val.toInt());
            ui->srm_low->setValue(val.toInt());
        } else {}
    } else if(type == "abv"){
        if(lim == "high"){
            ui->abv->setUpperValue(val.toFloat() * 10);
            ui->abv_high->setValue(val.toFloat());
        } else if(lim == "low"){
            ui->abv->setLowerValue(val.toFloat() * 10);
            ui->abv_low->setValue(val.toFloat());
        } else {}
    } else {
        // exceptions
        QCoreApplication::processEvents();
        QMessageBox::warning(
            this,
            tr("Attention"),
            val.toString());
    }
}

MainWindow::~MainWindow()
{
    delete parser;
    delete ui;
}

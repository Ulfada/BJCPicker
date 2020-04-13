#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QFile>
#include <QTime>

#include <QDebug>

class JSONParser : public QObject
{
    Q_OBJECT

signals:
    void category(QString);
    void id(QString);
    void notes(QString);
    void name(QString);
    void beerid(QString);
    void info(QString);
    void example(QList<QVariant>);

    void stat(QString, QVariant, QString);

    void repaint();

public:
    JSONParser(QFile& file);
public slots:
    void Pick();
    void info(int idx);

private:
    QVariantMap _data;
    QMap<QString, QVariant> _beer;
};

#endif // JSONPARSER_H

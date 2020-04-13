#include "jsonparser.h"
#include <QCoreApplication>

int random(int high){
    int low = 0;
    qsrand((uint)QTime::currentTime().msec());
    return qrand() % ((high + 1) - low) + low;
}

JSONParser::JSONParser(QFile &file)
{
    QTextStream file_text(&file);
    auto json_str = file_text.readAll();
    file.close();

    auto json_bytes = json_str.toLocal8Bit();
    auto json_doc = QJsonDocument::fromJson(json_bytes);

    if(json_doc.isNull())
        return;

    if(!json_doc.isObject())
        return;

    QJsonObject json_obj = json_doc.object();

    if(json_obj.isEmpty())
        return;

    _data = json_obj.toVariantMap();
}

void JSONParser::Pick()
{
    auto m = _data["styleguide"].toMap().value("class").toList();
    auto brew_type = m.at(0).toMap().value("type");
    auto beer_list = m.at(0).toMap().value("category").toList();

    auto beer_category = beer_list.at(random(beer_list.size()-1));

    emit category(beer_category.toMap().value("name").toString());
    emit id(beer_category.toMap().value("id").toString());
    emit notes(beer_category.toMap().value("notes").toString());

    auto beer_subcategory = beer_category.toMap().value("subcategory").toList();
    _beer = beer_subcategory.at(random(beer_subcategory.size()-1)).toMap();

    emit name(_beer.value("name").toString());
    emit beerid(_beer.value("id").toString());

    emit info(_beer.value("impression").toString());

    auto examples = _beer.value("examples").toList();
    emit example(examples);

    // stats
    auto stats = _beer.value("stats").toMap();

    QStringList values = {"og", "fg", "ibu", "srm", "abv"};
    QStringList limits = {"low", "high"};

    for(const auto& val : values){
        for(const auto& lim : limits){
            if(stats.value(val).toMap().contains(lim)){
                auto data = stats.value(val).toMap().value(lim);
                emit stat(val, data, lim);
            }
        }
    }

    if(stats.contains("exceptions")){
        auto data = stats.value("exceptions").toString();
        emit stat("exceptions", data, "");
    }

    // MacOS QT BUG
    emit repaint();
}

void JSONParser::info(int idx)
{
    QStringList values = {"impression", "aroma", "appearance", "flavor", "mouthfeel", "comments", "history", "ingredients", "comparison"};
    emit info(_beer.value(values.at(idx)).toString());
}

#ifndef MODELDATA_H
#define MODELDATA_H

#include <iostream>
#include <map>
#include <iterator>
#include <QList>
#include <QString>

/** @note
数据载体
*/
class ModelData
{
public:
    explicit ModelData();

    virtual ~ModelData();

    ModelData(const ModelData & other);

    ModelData & operator = (const ModelData & other);

    std::string getValue(std::string key);

    std::string getKey(std::string value);

    void setValue(std::string key,std::string value);

    std::map<std::string,std::string> getDatas() const;
    void setDatas(const std::map<std::string, std::string> &datas);

protected:
    std::map<std::string,std::string> mDatas;
};

typedef QList<ModelData> ModelDatas;
typedef QList<ModelDatas> ModelDataList;
typedef QMap<QString,ModelData> ModelDataMap;

#endif // MODELDATA_H

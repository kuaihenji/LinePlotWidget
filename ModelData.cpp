#include "ModelData.h"

ModelData::ModelData()
{

}

ModelData::~ModelData()
{
    mDatas.clear();
}

ModelData::ModelData(const ModelData &other)
{
    mDatas = other.mDatas;
}

ModelData &ModelData::operator =(const ModelData &other)
{
    mDatas = other.mDatas;
    return *this;
}

std::string ModelData::getValue(std::string key)
{
    std::map<std::string,std::string>::iterator it = mDatas.find(key);
    if(it != mDatas.end())
    {
        return it->second;
    }
    return std::string();
}

std::string ModelData::getKey(std::string value)
{
    for(std::map<std::string,std::string>::iterator it = mDatas.begin();it!=mDatas.end();it++)
    {
        if(it->second == value)
            {
            return it->first;
        }
    }
    return std::string();
}

void ModelData::setValue(std::string key, std::string value)
{
    std::pair<std::map<std::string,std::string>::iterator,bool> ret;
    ret = mDatas.insert(std::pair<std::string,std::string>(key,value));
    if (ret.second == false) {
        ret.first->second = value;
    }
}

std::map<std::string, std::string> ModelData::getDatas() const
{
    return this->mDatas;
}

void ModelData::setDatas(const std::map<std::string, std::string> &datas)
{
    this->mDatas = datas;
}

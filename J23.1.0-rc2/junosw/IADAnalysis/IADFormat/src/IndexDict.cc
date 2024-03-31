#include "IADFormat/IndexDict.h"
#include <iostream>

using namespace IAD;

IndexDict* IndexDict::s_obj = nullptr;

IndexDict* IndexDict::instance(){
    static std::once_flag initFlag;
    std::call_once(initFlag, [](){
        std::cout<<"Creating a IndexDict"<<std::endl;
        s_obj = new IndexDict();});
    return s_obj;
}

const std::string& IndexDict::getPath(uint32_t pathId, int fileId){
    std::unique_lock<std::mutex> lock(m_mapMutex);
    return m_indexMap[fileId][pathId];
}

int IndexDict::insertMap(const std::map<uint32_t, std::string>& indexMap){
    std::unique_lock<std::mutex> lock(m_mapMutex);
    m_indexMap.push_back(indexMap);
    return m_indexMap.size() - 1;
}

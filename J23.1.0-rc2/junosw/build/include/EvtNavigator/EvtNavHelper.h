#ifndef EvtNavHelper_h
#define EvtNavHelper_h
/*
 * Description: helpers to retrieve the related data from EvtNavigator
 *
 *      Author: Tao Lin <lintao AT ihep.ac.cn>
 *        Date:  2022.03.09
 */

#include "EvtNavigator/EvtNavigator.h"
#include "EDMUtil/EDMManager.h"

namespace JM {

    /*
     * Get header object with the default path
     */
    template<typename T>
    T* getHeaderObject(EvtNavigator* evtnav) {
        std::string defaultPath = EDM_DEFAULT_PATH(T::Class_Name());
        return dynamic_cast<T*>(evtnav->getHeader(defaultPath));
    }

    /*
     * Get header object with the path
     */
    template<typename T>
    T* getHeaderObject(EvtNavigator* evtnav, const std::string& path) {
        return dynamic_cast<T*>(evtnav->getHeader(path));
    }


    /*
     * Set header object with the default path
     */
    template<typename T>
    void addHeaderObject(EvtNavigator* evtnav, T* header) {
        std::string defaultPath = EDM_DEFAULT_PATH(T::Class_Name());
        evtnav->addHeader(defaultPath, header);
    }

    /*
     * Set header object with the path
     */
    template<typename T>
    void addHeaderObject(EvtNavigator* evtnav, T* header, const std::string& path) {
        evtnav->addHeader(path, header);
    }

}

#endif

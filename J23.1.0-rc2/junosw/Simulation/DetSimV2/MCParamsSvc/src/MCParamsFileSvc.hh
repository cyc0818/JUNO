#ifndef MCParamsFileSvc_hh
#define MCParamsFileSvc_hh

/*  
 * This is one implemenation of IMCParamsSvc. It allows user:
 *   + organise properties in dir/file based.
 *   + data format in file is simple, just key and val mapping
 *
 * Updates:
 *   * the metadata could be queried from DB. 
 *     The priority (from high to low) will be:
 *       - specify at command line / property
 *       - query from database
 *       - the default data path under MCPARAMSROOT or WORKTOP or JUNOTOP
 *     Tao Lin <lintao AT ihep.ac.cn>, 8th Dec 2022
 */

#include <MCParamsSvc/IMCParamsSvc.hh>
#include <ParaDB/IParaDBSvc.hh>
#include <SniperKernel/SvcBase.h>

class MCParamsFileSvc: public IMCParamsSvc, public SvcBase
{
   
  
    public:
        MCParamsFileSvc(const std::string& name);
        ~MCParamsFileSvc();

        bool initialize();
        bool finalize();

        virtual bool Get(const std::string&, vec_d2d& props);
        virtual bool Get(const std::string&, vec_s2d& props);
        virtual bool Get(const std::string&, map_s2d& props); 
    private:
        std::map<std::string,std::string> m_Param2value;//this list contains the optical parameters which will be replaced;
    public:
        const std::string GetPath(const std::string & param);
        double g4string2value(std::string value);

    private:
        // common base
        std::string m_base;
        std::string m_worktop_base;
        // ParaDB related
        bool m_paradb_enabled;
        ParaDB::IParaDBSvc* m_paradb_svc;

        // remove _vX from the param
        std::string get_paradb_property_name(const std::string& param);

        // param white list for paradb
        // - Material
        bool is_available_in_paradb(const std::string& param);
};
      
#endif

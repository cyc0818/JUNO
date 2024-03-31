#ifndef MCParamsDBSvc_hh
#define MCParamsDBSvc_hh

/*  
 * This is one implemenation of IMCParamsSvc. It allows user:
 *   + organise properties in dir/file based.
 *   + data format in file is simple, just key and val mapping
 */

#include <MCParamsSvc/IMCParamsSvc.hh>
#include <SniperKernel/SvcBase.h>

class MCParamsDBSvc: public IMCParamsSvc, public SvcBase
{
    public:
        MCParamsDBSvc(const std::string& name);
        ~MCParamsDBSvc();

        bool initialize();
        bool finalize();

        virtual bool Get(const std::string&, vec_d2d& props);
        virtual bool Get(const std::string&, vec_s2d& props);
        virtual bool Get(const std::string&, map_s2d& props);

    private:
        std::string m_uri;

    private:
        std::map<std::string, IMCParamsSvc::vec_d2d> m_vec_d2d;
        std::map<std::string, IMCParamsSvc::vec_s2d> m_vec_s2d;
        std::map<std::string, IMCParamsSvc::map_s2d> m_map_s2d;

};

#endif

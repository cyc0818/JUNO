#ifndef ROOT_FILE_READER_H
#define ROOT_FILE_READER_H 0

#include <string>
#include <map>
#include <vector>

class NavTreeList;

class RootFileReader {

    typedef std::vector<std::string> StringVector;

    public:
        RootFileReader(const StringVector& filelist, bool withNav);
        ~RootFileReader();

        // Analyze input files, construct NavTreeList and InputElementKeeper
        bool checkAndExecute();
        NavTreeList* getNavTree() { return m_navTree; }
        // For SniperLog
        const std::string& objName() { return m_objName; }

    private:
        StringVector m_fileList;
        bool m_withNav;
        NavTreeList* m_navTree;
        std::string  m_objName;
};

#endif

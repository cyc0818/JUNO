
#define BOOST_TEST_MODULE CondDBTest

#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <CondDB/IRepo.h>
#include "LocalFSRepo.h"

namespace fs = boost::filesystem;


BOOST_AUTO_TEST_CASE(testLocalFSRepo)
{

    const char* conddb_root = getenv("CONDDBROOT");
    BOOST_CHECK(conddb_root != NULL);

    const std::string base = conddb_root;
    const std::string repo_uri = (fs::path(base)/"share/dummy-repo").string();
    const std::string repo_name = "localrepo";
    boost::shared_ptr<CondDB::IRepo> repo(new CondDB::LocalFSRepo(repo_name, repo_uri));

    const std::vector<CondDB::GlobalTag>& vec_gt_not_exist = repo->findGlobalTags("gt_not_exists");

    BOOST_REQUIRE(vec_gt_not_exist.size()==0);

    const std::vector<CondDB::GlobalTag>& vec_gt = repo->findGlobalTags("GT_J18_v1");
    BOOST_REQUIRE(vec_gt.size()==1);
    const CondDB::GlobalTag& gt = vec_gt[0];
    BOOST_CHECK(gt.name() == "GT_J18_v1");

    const std::vector<CondDB::Tag>& vec_tag = repo->findTags(gt);
    BOOST_REQUIRE(vec_tag.size() == 1);

    const CondDB::Tag& tag = vec_tag[0];
    BOOST_CHECK(tag.name() == "CD.LS.MatProp.2018.V1");

    const std::vector<CondDB::IOV>& vec_iov = repo->findIOVs(tag);
    BOOST_REQUIRE(vec_iov.size() == 2);

    // check each one
    const CondDB::IOV& iov_0 = vec_iov[0];
    BOOST_CHECK(iov_0.since() == 1234);
    BOOST_CHECK(iov_0.payload_hash() == "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d");

    const CondDB::IOV& iov_1 = vec_iov[1];
    BOOST_CHECK(iov_1.since() == 5678);
    BOOST_CHECK(iov_1.payload_hash() == "7c211433f02071597741e6ff5a8ea34789abbf43");

    

}

BOOST_AUTO_TEST_CASE(testParseJson)
{
    std::stringstream ss;
    ss << "{" 
       << "\"iovs\": ["
       << "{\"since\": 1234, \"payload_hash\": \"aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d\"}, "
       << "{\"since\": 5678, \"payload_hash\": \"7c211433f02071597741e6ff5a8ea34789abbf43\"}"
       << "],"
       << "\"entries\": 2"
       << "}"
       << std::endl;

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);


}

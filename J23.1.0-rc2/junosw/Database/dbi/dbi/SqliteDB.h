#ifndef dbi_SqliteDB_h
#define dbi_SqliteDB_h

#include <dbi/dbi.h>
#include <sqlite3.h>
#include <cstdio>
#include <memory>

namespace dbi {

    struct SqliteDB {

        SqliteDB(const std::string& _fn)
        : fn(_fn) {

        }

        SqliteDB(const std::string& _fn, const std::string& _schema_name)
            : fn(_fn), schema_name(_schema_name) {

        }


        bool doConnect() {

            sqlite3* conn{nullptr};

            int rc = sqlite3_open_v2(fn.c_str(), 
                                     &conn, 
                                     SQLITE_OPEN_READONLY | SQLITE_OPEN_URI, 
                                     NULL);

            if (rc) {
                std::cerr << "Failed to open sqlite3 connection "
                          << fn
                          << " error message: " 
                          << sqlite3_errmsg(conn)
                          << std::endl;
                return false;
            }

            m_connection = std::shared_ptr<sqlite3>(conn, sqlite3_close);

            // if there is schema_name
            if (schema_name.size()) {
                // Need to use 'ATTACH DATABASE ... AS schema_name' 
                std::string stmt = "ATTACH DATABASE '";
                stmt += fn + "' AS " + schema_name;
                std::cout << "EXECUTE SQL: " << stmt << std::endl;
                doQuery(stmt);
            }

            return true;
        }

        std::vector<ResultSet> doQuery(const std::string& str) {
            std::vector<ResultSet> results;


            sqlite3_stmt *prepared_stmt = nullptr;
            int rc = sqlite3_prepare_v2(m_connection.get(),
                                        str.c_str(),
                                        -1, //str.size(),
                                        &prepared_stmt,
                                        NULL);

            if (rc != SQLITE_OK) {
                std::cerr << "Failed to query in sqlite3: ["
                          << str 
                          << "]"
                          << " error message: " 
                          << sqlite3_errmsg(m_connection.get())
                          << std::endl;
                return results;
            }

            while (sqlite3_step(prepared_stmt) == SQLITE_ROW) {
                ResultSet rs;

                int num_fields = sqlite3_column_count(prepared_stmt);

                for (int i = 0; i < num_fields; ++i) {
                    const char* s = reinterpret_cast<const char*>(sqlite3_column_text(prepared_stmt, i));
                    std::string r;
                    if (s) {
                        r = s;
                    }
                    rs.m_internals.push_back(r);
                }

                results.push_back(rs);
            }

            sqlite3_finalize(prepared_stmt);

            return results;
        }

    private:
        std::string fn; // filename
        std::string schema_name; // database alias name

        std::shared_ptr<sqlite3> m_connection{nullptr}; // sqlite3 is opaque struct

    };

}


#endif

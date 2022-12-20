//
// Created by yjrj on 2022/12/20.
//

#include "DataBase.h"
#include "base/memory/Memory.h"
#include "FileUtils.h"

#if (CC_PLATFORM == CC_PLATFORM_WINDOWS)
    #include <sqlite3/sqlite3.h>
#else
    #include <sqlite3.h>
#endif

namespace cc::exp {

struct SqliteImpl : public DataBase::Impl {
public:
    SqliteImpl() = default;
    ~SqliteImpl() = default;

private:
    void init(const std::string &path) override;
    void close() override;

    bool setData(const std::string &key, const std::string &data) override;
    std::string getData(const std::string &key) override;

    int createTable(const std::string &path);
    void createStatement();

    sqlite3 *db = nullptr;
    sqlite3_stmt *selectSTAT = nullptr;
    sqlite3_stmt *removeSTAT = nullptr;
    sqlite3_stmt *updateSTAT = nullptr;
    sqlite3_stmt *clearSTAT  = nullptr;
    sqlite3_stmt *keySTAT    = nullptr;
    sqlite3_stmt *countSTAT  = nullptr;
};

int SqliteImpl::createTable(const std::string &path) {
    int ret = sqlite3_open(path.c_str(), &db);
    if (ret != SQLITE_OK && ret != SQLITE_DONE) {
        return ret;
    }

    const char *createTableSTAT = "CREATE TABLE IF NOT EXISTS data(key TEXT PRIMARY KEY,value TEXT);";
    sqlite3_stmt *stmt;
    ret = sqlite3_prepare_v2(db, createTableSTAT, -1, &stmt, nullptr);
    ret |= sqlite3_step(stmt);
    ret |= sqlite3_finalize(stmt);

    if (ret != SQLITE_OK && ret != SQLITE_DONE) {
        printf("Error in CREATE TABLE\n");
    }
    return ret;
}

void SqliteImpl::createStatement() {
    // SELECT
    const char *sql_select = "SELECT value FROM data WHERE key=?;";
    sqlite3_prepare_v2(db, sql_select, -1, &selectSTAT, nullptr);

    // REPLACE
    const char *sql_update = "REPLACE INTO data (key, value) VALUES (?,?);";
    sqlite3_prepare_v2(db, sql_update, -1, &updateSTAT, nullptr);

    // DELETE
    const char *sql_remove = "DELETE FROM data WHERE key=?;";
    sqlite3_prepare_v2(db, sql_remove, -1, &removeSTAT, nullptr);

    // Clear
    const char *sql_clear = "DELETE FROM data;";
    sqlite3_prepare_v2(db, sql_clear, -1, &clearSTAT, nullptr);

    // key
    const char *sql_key = "SELECT key FROM data ORDER BY ROWID ASC;";
    sqlite3_prepare_v2(db, sql_key, -1, &keySTAT, nullptr);

    //count
    const char *sql_count = "SELECT COUNT(*) FROM data;";
    sqlite3_prepare_v2(db, sql_count, -1, &countSTAT, nullptr);
}

bool SqliteImpl::setData(const std::string &key, const std::string &data) {
    int ret = sqlite3_bind_text(updateSTAT, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    ret |= sqlite3_bind_text(updateSTAT, 2, data.c_str(), -1, SQLITE_TRANSIENT);
    ret |= sqlite3_step(updateSTAT);
    ret |= sqlite3_reset(updateSTAT);

    return ret == SQLITE_OK || ret == SQLITE_DONE;
}

std::string SqliteImpl::getData(const std::string &key) {
    int ret = sqlite3_reset(selectSTAT);

    ret |= sqlite3_bind_text(selectSTAT, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    ret |= sqlite3_step(selectSTAT);
    const unsigned char *text = sqlite3_column_text(selectSTAT, 0);
    std::string result;
    if (text != nullptr) {
        result.assign(reinterpret_cast<const char*>(text));
    }
    return result;
}

void SqliteImpl::init(const std::string &path) {
    if (!createTable(path)) {
        return;
    }
    createStatement();
}

void SqliteImpl::close() {
    if (db != nullptr) {
        sqlite3_finalize(selectSTAT);
        sqlite3_finalize(removeSTAT);
        sqlite3_finalize(updateSTAT);

        sqlite3_close(db);
    }
}

void DataBase::initDataBase(const std::string &path) {
    if (impl == nullptr) {
        impl = ccnew SqliteImpl();
        impl->init(path);
    }
}

DataBase::~DataBase() noexcept {
    if (impl != nullptr) {
        impl->close();
        delete impl;
    }
}

void DataBase::setData(const std::string &key, const std::string &data) {
    if (impl != nullptr) {
        impl->setData(key, data);
    }
}

std::string DataBase::getData(const std::string &key) {
    static std::string EMPTY = "";
    return impl != nullptr ? impl->getData(key) : EMPTY;
}

}

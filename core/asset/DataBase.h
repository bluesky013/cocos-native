//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <core/singleton/Singleton.h>

namespace cc::exp {

class DataBase : public Singleton<DataBase> {
public:
    DataBase() = default;
    ~DataBase() noexcept;

    void initDataBase(const std::string &path);

    void setData(const std::string &key, const std::string &data);
    std::string getData(const std::string &key);

    struct Impl {
        Impl() = default;
        virtual ~Impl() = default;

        virtual bool setData(const std::string &key, const std::string &data) = 0;
        virtual std::string getData(const std::string &key) = 0;

        virtual void init(const std::string &path) = 0;
        virtual void close() = 0;
    };

private:
    Impl *impl = nullptr;
};

}

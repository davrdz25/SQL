#pragma once
#include <optional>
#include <vector>

template <typename T>
class IBaseRepository {
    public:
        ~IBaseRepository() = default;
        virtual std::vector<T> ReadAll() = 0;
        virtual T ReadByEntry(const int&) = 0;
        virtual bool Create(const T&) = 0;
        virtual bool Update(const T&) = 0;
        virtual bool Delete(const T&) = 0;
};
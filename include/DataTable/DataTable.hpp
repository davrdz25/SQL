#include <iostream>
#include <map>


class DataTable {
    private:
        std::vector<std::map<std::string, std::string>> data;
    
    public:
        void setData(const std::vector<std::map<std::string, std::string>>& newData);
        std::map<std::string, std::string>& operator[](int index);
        int size() const ;
    };
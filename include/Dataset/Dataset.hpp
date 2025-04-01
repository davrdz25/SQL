#include <iostream>
#include <map>


class Dataset {
    private:
        std::vector<std::map<std::string, std::string>> data;
    
    public:
        void setData(const std::vector<std::map<std::string, std::string>>& newData) {
            data = newData;
        }
    
        std::map<std::string, std::string>& operator[](int index) {
            if (index < 0 || index >= data.size()) {
                throw std::out_of_range("Índice fuera de rango");
            }
            return data[index];
        }
    
        int size() const {
            return data.size();
        }
    };
#include "include/SQL/SQL.hpp"
#include "include/DataTable/DataTable.hpp"

int main() {
    try {
        SQL controller;
        controller.connect("192.168.1.253", "POS", "sa", "Development..");
        controller.prepareStatement("SELECT ItemName, ItemCode FROM Items");
        controller.execute();
        auto results = controller.fetchResults();

        DataTable DataTable;
        DataTable.setData(results);

        // Accediendo a los datos como DataTable[i]["ColumnName"]
        for (int i = 0; i < DataTable.size(); ++i) {
            std::cout << "Row " << i << ":" << std::endl;
            std::cout << "ItemName: " << DataTable[i]["ItemName"] << std::endl;
            std::cout << "ItemCode: " << DataTable[i]["ItemCode"] << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

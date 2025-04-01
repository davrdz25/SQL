#include "include/SQL/SQL.hpp"
#include "include/Dataset/Dataset.hpp"

int main() {
    try {
        SQL controller;
        controller.connect("192.168.1.253", "POS", "sa", "Development..");
        controller.prepareStatement("SELECT ItemName, ItemCode FROM Items");
        controller.execute();
        auto results = controller.fetchResults();

        Dataset dataset;
        dataset.setData(results);

        // Accediendo a los datos como dataset[i]["ColumnName"]
        for (int i = 0; i < dataset.size(); ++i) {
            std::cout << "Row " << i << ":" << std::endl;
            std::cout << "ItemName: " << dataset[i]["ItemName"] << std::endl;
            std::cout << "ItemCode: " << dataset[i]["ItemCode"] << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

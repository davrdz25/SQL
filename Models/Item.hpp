#pragma one
#include <iostream>

class Item
{
private:
    float _OnHand;

    int _Entry;

    std::string _ItemCode;
    std::string _ItemName;
    std::string _Codebars;

public:
    float OnHand();    

    int Entry();

    std::string ItemCode();
    std::string ItemName();
    std::string Codebars();

    void OnHand(float);

    void Entry(int const);
    
    void ItemCode(std::string& const);
    void ItemName(std::string& const);
    void Codebars(std::string& const);

    Item();
    ~Item();
};

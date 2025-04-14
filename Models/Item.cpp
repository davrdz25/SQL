#include "Item.hpp"

Item::Item()
{
    this->_Entry = 0;
    this->_ItemName = nullptr;
    this->_ItemCode = nullptr;
    this->_Codebars= nullptr;
    this->_OnHand = 0.0;
}

Item::~Item()
{
}

int Item::Entry()
{
    return this->_Entry;
}
std::string Item::ItemCode()
{
    return this->_ItemCode;
}
std::string Item::ItemName()
{
    return this->_ItemName;
}
std::string Item::Codebars()
{
    return this->_Codebars;
}
float Item::OnHand()
{
    return this->_OnHand;
}

void Item::Entry(int const _Entry)
{
    this->_Entry = _Entry;
}
void Item::ItemCode(std::string& const _ItemCode)
{
    this->_ItemCode = _ItemCode;
}
void Item::ItemName(std::string& const _ItemName)
{
    this->_ItemName = _ItemName;
}
void Item::Codebars(std::string& const _Codebars)
{
    this->_Codebars = _Codebars;
}
void Item::OnHand(float _OnHand)
{
    this->_OnHand = _OnHand;
}
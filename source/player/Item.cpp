#include "player/Item.hpp"

Item::Item() : id(0), name(""), price(0), amount(0), category(ItemType::Other), is_deleted(false)
{
}

Item::Item(int _id, std::string _name, int _price, int _amount, ItemType _category, bool _is_deleted) 
                    : id(_id), name(_name), price(_price), amount(_amount), category(_category), is_deleted(_is_deleted)
{
}

void Item::set_id(int _id)
{
    id = _id;
}

void Item::set_name(std::string _name)
{
    name = _name;
}

void Item::set_price(int _price)
{
    price =_price;
}

void Item::set_amount(int _amount)
{
    amount = _amount;
}

void Item::set_category(ItemType _category)
{
    category = _category;
}

void Item::set_is_deleted(bool _is_deleted)
{
    is_deleted = _is_deleted;
}

int Item::get_id()
{
    return id;
}

std::string Item::get_name()
{
    return name;
}

int Item::get_price()
{
    return price;
}

int Item::get_amount()
{
    return amount;
}

ItemType Item::get_category()
{
    return category;
}

bool Item::get_is_deleted()
{
    return is_deleted;
}

void Item::increase_amount(int amount_to_increase)
{
    amount += amount_to_increase;
}

void Item::decrease_amount(int amount_to_decrease)
{
    amount -= amount_to_decrease;
}

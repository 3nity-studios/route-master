#include "player/Item.hpp"

Item::Item() : id(0), name(""), price(0), amount(0), category(ItemType::Other), is_deleted(false)
{
    // empty
}

Item::Item(int _id, std::string _name, int _price, int _amount, ItemType _category, bool _is_deleted) 
                    : id(_id), name(_name), price(_price), amount(_amount), category(_category), is_deleted(_is_deleted)
{
    // empty
}

Item::Item(nlohmann::json j)
{
    id = j["id"];
    name = j["name"];
    price = j["price"];
    amount = j["amount"];
    category = static_cast<ItemType>(j["category"]);
    is_deleted = j["is_deleted"];
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

int Item::get_id() const noexcept
{
    return id;
}

std::string Item::get_name() const noexcept
{
    return name;
}

int Item::get_price() const noexcept
{
    return price;
}

int Item::get_amount() const noexcept
{
    return amount;
}

ItemType Item::get_category() const noexcept
{
    return category;
}

bool Item::get_is_deleted() const noexcept
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

nlohmann::json Item::to_json()
{
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["price"] = price;
    j["amount"] = amount;
    j["category"] = static_cast<int>(category);
    j["is_deleted"] = is_deleted;

    return j;
}
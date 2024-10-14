#pragma once

#include <string>

enum class ItemType
{
    Bus,
    Upgrade,
    Employee,
    Other
};

class Item
{
    private: 

    int id; 
    std::string name; 
    int price; 
    int amount;
    ItemType category;
    bool is_deleted; 

    public: 

    Item();
    Item(int _id, std::string _name, int _price, int _amount, ItemType _category, bool _is_deleted);

    void set_id(int _id);
    void set_name(std::string _name);
    void set_price(int _price);
    void set_amount(int _amount);
    void set_category(ItemType _category);
    void set_is_deleted(bool _is_deleted);

    int get_id();
    std::string get_name();
    int get_price();
    int get_amount();
    ItemType get_category();
    bool get_is_deleted();

    void increase_amount(int amount_to_increase);
    void decrease_amount(int amount_to_decrease);
};
#include <string>

class Employee
{
    private:

    std::string name; 
    std::string last_name;
    ushort age;
    ushort shift_len;
    ushort fatigue; 

    public:

    Employee();
    Employee(std::string _name, std::string _last_name, ushort _age, ushort _shift_len, ushort _fatigue); 

    std::string get_name();
    std::string get_last_name();
    ushort get_age();
    ushort get_shift_len();
    ushort get_fatigue();

    void set_name(std::string _name);
    void set_last_name(std::string _last_name);
    void set_age(ushort _age);
    void set_shift_len(ushort _shift_len);
    void set_fatigue(ushort _fatigue);

    void calc_fatigue(int km);
};
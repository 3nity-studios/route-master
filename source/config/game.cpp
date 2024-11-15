#include "config/game.hpp"
#include "engine/state_machine.hpp"
#include "states/main_menu_state.hpp"
#include "states/map_viewer_state.hpp"
#include "states/store_state.hpp"

#include "simulation/Bus.hpp"
#include "simulation/Employee.hpp"
#include "player/AchievementManager.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include "utils/street_factory.hpp"

float calc_distance(VisualElement element1, VisualElement element2)
{
    return sqrt(pow(element1.get_x() - element2.get_x(), 2) + pow(element1.get_y() - element2.get_y(), 2));
}

// private
void Game::init_variables()
{
    this->_data->window = nullptr;
    this->_data->states.add_state(Engine::StateRef(new MainMenuState(this->_data)), false);

    std::ifstream player_file("data/player.json");
    if (player_file.is_open())
    {
        nlohmann::json player_json;
        player_file >> player_json;
        this->_data->player = Player(player_json);
        player_file.close();
    }
    else
    {
        this->_data->player = Player(0, "Player1", 80000);
        this->_data->player.save();
    }

    // Load store data from JSON if it exists
    std::ifstream store_file("data/store.json");
    if (store_file.is_open())
    {
        nlohmann::json store_json;
        store_file >> store_json;
        this->_data->store = Store(store_json);
        store_file.close();
    }
    else
    {
        this->_data->store = Store();
        this->_data->store.add_bus_to_inventory(Bus(0, "Encava", 50, {}, 5, 100, 500, 80, 150, 60, 150, 80, 150), 35000, 10);
        this->_data->store.add_bus_to_inventory(Bus(1, "Gillig Low-Floor", 35, {}, 10, 150, 650, 120, 200, 90, 200, 100, 200), 40000, 5);
        this->_data->store.add_bus_to_inventory(Bus(2, "Gillig ADA", 55, {}, 7, 120, 800, 100, 300, 80, 300, 150, 300), 58800, 3);

        this->_data->store.add_employee_to_inventory(Employee(0, "Jane", "Smith", 30, 20, 7, 1), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(1, "Alice", "Johnson", 28, 20, 6, 2), 450, 1);
        this->_data->store.add_employee_to_inventory(Employee(2, "Bob", "Brown", 35, 70, 20, 3), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(3, "Charlie", "Davis", 40, 20, 15, 4), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(4, "Abdul", "Qureshi", 32, 20, 8, 5), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(5, "Erick", "Moukoko", 29, 20, 7, 6), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(6, "Hans", "Schlafzimmer", 45, 20, 12, 7), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(7, "Katarina", "Marinkovic", 27, 20, 6, 8), 450, 1);
        this->_data->store.add_employee_to_inventory(Employee(8, "Nanako", "Miyamoto", 30, 20, 7, 9), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(9, "Juliana", "Contreras", 33, 20, 9, 10), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(10, "Sofía", "Pérez", 26, 50, 5, 20), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(11, "Pedro", "López", 28, 55, 6, 20), 500, 1);
        this->_data->store.add_employee_to_inventory(Employee(12, "Alejandro", "Muñoz", 31, 47, 8, 13), 500, 1);

        this->_data->store.save();
    }

    this->_data->achievement_manager = AchievementManager();
    this->_data->achievement_manager.add_achievement(Achievement(
        0, 
        "Buy Buses", 
        "Add buses to your fleet.",
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_buses().size() >= 1; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_buses().size() >= 5; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_buses().size() >= 10; }
        },
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_buses().size() / 1) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_buses().size() / 5) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_buses().size() / 10) * 100; }
        },
        {10000, 20000, 100000}
    ));
    this->_data->achievement_manager.add_achievement(Achievement(
        1, 
        "Hire Employees", 
        "Hire employees for your company.",
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_employees().size() >= 1; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_employees().size() >= 5; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_employees().size() >= 10; }
        },
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_employees().size() / 1) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_employees().size() / 5) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_employees().size() / 10) * 100; }
        },
        {5000, 10000, 50000}
    ));

    this->_data->achievement_manager.add_achievement(Achievement(
        2, 
        "Earn Money", 
        "Accumulate money in your account.",
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_balance() >= 10000; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_balance() >= 50000; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return player.get_balance() >= 100000; }
        },
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_balance() / 10000) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_balance() / 50000) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (player.get_balance() / 100000) * 100; }
        },
        {1000, 5000, 10000}
    ));

    this->_data->achievement_manager.add_achievement(Achievement(
        3, 
        "Complete Routes", 
        "Successfully complete bus routes.",
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return simulation_info.size() >= 1; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return simulation_info.size() >= 5; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return simulation_info.size() >= 10; }
        },
        {
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (simulation_info.size() / 1) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (simulation_info.size() / 5) * 100; },
            [](Player &player, Store &store, std::vector<SimulationInfo> &simulation_info) { return (simulation_info.size() / 10) * 100; }
        },
        {2000, 10000, 50000}
    ));
    this->_data->achievement_manager.update_from_json();

    std::ifstream city_file("data/city.json");
    if (city_file.is_open())
    {
        nlohmann::json city_json;
        city_file >> city_json;
        this->_data->city = City(city_json);
        city_file.close();
    }
    else
    {
        BusStop stop1(1, "Stop1", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 5.0, 2.0, 3200.f, 128.f);
        BusStop stop2(2, "Stop2", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 3.0, 2.0, 1824.f, 128.f);
        BusStop stop3(3, "Stop3", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 704.f, 416.f);
        BusStop stop4(4, "Stop4", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 5.0, 2.0, 704.f, 1792.f);
        BusStop stop5(5, "Stop5", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 1056.f, 1440.f);
        BusStop stop6(6, "Stop6", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 1248.f, 736.f);
        BusStop stop7(7, "Stop7", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 1440.f, 416.f);
        BusStop stop8(8, "Stop8", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 1440.f, 1760.f);
        BusStop stop9(9, "Stop9", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 1984.f, 2048.f);
        BusStop stop10(10, "Stop10", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 2784.f, 1440.f);
        BusStop stop11(11, "Stop11", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 2784.f, 736.f);
        BusStop stop12(12, "Stop12", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 2976.f, 448.f);
        BusStop stop13(13, "Stop13", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 3744.f, 352.f);
        BusStop stop14(14, "Stop14", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10}, 30.0, 15.0, 3.0, 6.0, 2.0, 3424.f, 1440.f);
        BusStop stop15(36, "Stop15", {}, 0, 0, 0, 0, 0, 352.f, 736.f);
        BusStop stop16(37, "Stop16", {}, 0, 0, 0, 0, 0, 3360.f, 736.f);

        VisualElement curve15(15, 3744.f, 128.f);
        VisualElement curve16(16, 2976.f, 128.f);
        VisualElement curve17(17, 2208.f, 128.f);
        VisualElement curve18(18, 1440.f, 128.f);
        VisualElement curve19(19, 704.f, 128.f);
        VisualElement curve20(20, 704.f, 736.f);
        VisualElement curve21(21, 704.f, 1440.f);
        VisualElement curve22(22, 704.f, 2048.f);
        VisualElement curve23(23, 32.f, 736.f);
        VisualElement curve24(24, 32.f, 1440.f);
        VisualElement curve25(25, 1440.f, 736.f);
        VisualElement curve26(26, 2208.f, 736.f);
        VisualElement curve27(27, 2976.f, 736.f);
        VisualElement curve28(28, 3744.f, 736.f);
        VisualElement curve29(29, 1440.f, 1440.f);
        VisualElement curve30(30, 2208.f, 1440.f);
        VisualElement curve31(31, 2976.f, 1440.f);
        VisualElement curve32(32, 3744.f, 1440.f);
        VisualElement curve33(33, 1440.f, 2048.f);
        VisualElement curve34(34, 2208.f, 2048.f);
        VisualElement curve35(35, 2976.f, 2048.f);

        this->_data->city.add_bus_stop(stop1);
        this->_data->city.add_bus_stop(stop2);
        this->_data->city.add_bus_stop(stop3);
        this->_data->city.add_bus_stop(stop4);
        this->_data->city.add_bus_stop(stop5);
        this->_data->city.add_bus_stop(stop6);
        this->_data->city.add_bus_stop(stop7);
        this->_data->city.add_bus_stop(stop8);
        this->_data->city.add_bus_stop(stop9);
        this->_data->city.add_bus_stop(stop10);
        this->_data->city.add_bus_stop(stop11);
        this->_data->city.add_bus_stop(stop12);
        this->_data->city.add_bus_stop(stop13);
        this->_data->city.add_bus_stop(stop14);
        this->_data->city.add_bus_stop(stop15);
        this->_data->city.add_bus_stop(stop16);
        this->_data->city.add_curve(curve15);
        this->_data->city.add_curve(curve16);
        this->_data->city.add_curve(curve17);
        this->_data->city.add_curve(curve18);
        this->_data->city.add_curve(curve19);
        this->_data->city.add_curve(curve20);
        this->_data->city.add_curve(curve21);
        this->_data->city.add_curve(curve22);
        this->_data->city.add_curve(curve23);
        this->_data->city.add_curve(curve24);
        this->_data->city.add_curve(curve25);
        this->_data->city.add_curve(curve26);
        this->_data->city.add_curve(curve27);
        this->_data->city.add_curve(curve28);
        this->_data->city.add_curve(curve29);
        this->_data->city.add_curve(curve30);
        this->_data->city.add_curve(curve31);
        this->_data->city.add_curve(curve32);
        this->_data->city.add_curve(curve33);
        this->_data->city.add_curve(curve34);
        this->_data->city.add_curve(curve35);

        this->_data->city.initialize_bus_stops(0);

        Street street1 = util::StreetFactory(_data, 1, "Street1", 15, 1, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street2 = util::StreetFactory(_data, 2, "Street2", 1, 16, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street3 = util::StreetFactory(_data, 3, "Street3", 16, 17, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street4 = util::StreetFactory(_data, 4, "Street4", 17, 2, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street5 = util::StreetFactory(_data, 5, "Street5", 2, 18, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street6 = util::StreetFactory(_data, 6, "Street6", 18, 19, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street7 = util::StreetFactory(_data, 7, "Street7", 23, 36, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street8 = util::StreetFactory(_data, 8, "Street8", 20, 6, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street9 = util::StreetFactory(_data, 9, "Street9", 6, 25, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street10 = util::StreetFactory(_data, 10, "Street10", 25, 26, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street11 = util::StreetFactory(_data, 11, "Street11", 26, 11, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street12 = util::StreetFactory(_data, 12, "Street12", 11, 27, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street13 = util::StreetFactory(_data, 13, "Street13", 27, 37, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street14 = util::StreetFactory(_data, 14, "Street14", 32, 14, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street15 = util::StreetFactory(_data, 15, "Street15", 14, 31, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street16 = util::StreetFactory(_data, 16, "Street16", 31, 10, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street17 = util::StreetFactory(_data, 17, "Street17", 10, 30, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street18 = util::StreetFactory(_data, 18, "Street18", 30, 29, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street19 = util::StreetFactory(_data, 19, "Street19", 29, 5, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street20 = util::StreetFactory(_data, 20, "Street20", 5, 21, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street21 = util::StreetFactory(_data, 21, "Street21", 21, 24, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street22 = util::StreetFactory(_data, 22, "Street22", 22, 33, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street23 = util::StreetFactory(_data, 23, "Street23", 33, 9, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street24 = util::StreetFactory(_data, 24, "Street24", 9, 34, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street25 = util::StreetFactory(_data, 25, "Street25", 34, 35, 50.0f, 2.0f, 0.1f, 0.05f);
        Street street26 = util::StreetFactory(_data, 26, "Street26", 24, 23, 10.f, 2.0f, 0.1f, 0.05f);
        Street street27 = util::StreetFactory(_data, 27, "Street27", 19, 3, 10.f, 2.0f, 0.1f, 0.05f);
        Street street28 = util::StreetFactory(_data, 28, "Street28", 3, 20, 10.f, 2.0f, 0.1f, 0.05f);
        Street street29 = util::StreetFactory(_data, 29, "Street29", 20, 21, 10.f, 2.0f, 0.1f, 0.05f);
        Street street30 = util::StreetFactory(_data, 30, "Street30", 21, 4, 10.f, 2.0f, 0.1f, 0.05f);
        Street street31 = util::StreetFactory(_data, 31, "Street31", 4, 22, 10.f, 2.0f, 0.1f, 0.05f);
        Street street32 = util::StreetFactory(_data, 32, "Street32", 33, 8, 10.f, 2.0f, 0.1f, 0.05f);
        Street street33 = util::StreetFactory(_data, 33, "Street33", 8, 29, 10.f, 2.0f, 0.1f, 0.05f);
        Street street34 = util::StreetFactory(_data, 34, "Street34", 29, 25, 10.f, 2.0f, 0.1f, 0.05f);
        Street street35 = util::StreetFactory(_data, 35, "Street35", 25, 7, 10.f, 2.0f, 0.1f, 0.05f);
        Street street36 = util::StreetFactory(_data, 36, "Street36", 7, 18, 10.f, 2.0f, 0.1f, 0.05f);
        Street street37 = util::StreetFactory(_data, 37, "Street37", 17, 26, 10.f, 2.0f, 0.1f, 0.05f);
        Street street38 = util::StreetFactory(_data, 38, "Street38", 26, 30, 10.f, 2.0f, 0.1f, 0.05f);
        Street street39 = util::StreetFactory(_data, 39, "Street39", 30, 34, 10.f, 2.0f, 0.1f, 0.05f);
        Street street40 = util::StreetFactory(_data, 40, "Street40", 35, 31, 10.f, 2.0f, 0.1f, 0.05f);
        Street street41 = util::StreetFactory(_data, 41, "Street41", 31, 27, 10.f, 2.0f, 0.1f, 0.05f);
        Street street42 = util::StreetFactory(_data, 42, "Street42", 27, 12, 10.f, 2.0f, 0.1f, 0.05f);
        Street street43 = util::StreetFactory(_data, 43, "Street43", 12, 16, 10.f, 2.0f, 0.1f, 0.05f);
        Street street44 = util::StreetFactory(_data, 44, "Street44", 15, 13, 10.f, 2.0f, 0.1f, 0.05f);
        Street street45 = util::StreetFactory(_data, 45, "Street45", 13, 28, 10.f, 2.0f, 0.1f, 0.05f);
        Street street46 = util::StreetFactory(_data, 46, "Street46", 28, 32, 10.f, 2.0f, 0.1f, 0.05f);
        Street street47 = util::StreetFactory(_data, 47, "Street47", 36, 20, 10.f, 2.0f, 0.1f, 0.05f);
        Street street48 = util::StreetFactory(_data, 48, "Street48", 37, 28, 10.f, 2.0f, 0.1f, 0.05f);
    }   
    this->_data->is_music_on = true;
}
void Game::init_window()
{
    // creating our window object
    this->_data->window = new sf::RenderWindow();

    // in Windows at least, this must be called before creating the window
    float screen_scaling_factor = _platform.get_screen_scaling_factor(this->_data->window->getNativeHandle());

    // Use the screenScalingFactor to create video mode and set screen size - 720p by default
    this->_video_mode.size = {static_cast<uint>(SCREEN_WIDTH * screen_scaling_factor),
                              static_cast<uint>(SCREEN_HEIGHT * screen_scaling_factor)};

    // creating our window view using the video mode and disabling resizablilty
    this->_data->window->create(this->_video_mode, "Route Master", sf::Style::Titlebar | sf::Style::Close);
    _platform.set_icon(this->_data->window->getNativeHandle());

    // sets FPS vsync
    this->_data->window->setFramerateLimit(60);
}

// Constructors

Game::Game()
{
    this->init_variables();
    this->init_window();

    // running the new game
    this->run();
}

Game::~Game()
{
    delete this->_data->window;
}

// Accessors
bool Game::is_running() const
{
    return this->_data->window->isOpen();
}

void Game::run()
{
    // initializing a new Game

    // time initialization
    float new_time, frame_time, interpolation;
    float current_time = this->_clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    // Game Loop
    while (this->is_running())
    {

        this->_data->states.process_states();

        new_time = this->_clock.getElapsedTime().asSeconds();
        frame_time = new_time - current_time;

        if (frame_time > 0.25f)
        {
            frame_time = 0.25f;
        }

        current_time = new_time;
        accumulator += frame_time;

        while (accumulator >= dt)
        {
            this->_data->states.get_active_state()->update_inputs();
            this->_data->states.get_active_state()->update_state(dt);

            accumulator -= dt;
        }

        interpolation = accumulator / dt;
        this->_data->states.get_active_state()->draw_state(interpolation);
    }
}

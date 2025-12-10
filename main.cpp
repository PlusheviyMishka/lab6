#include "include/npc.h"
#include "include/bear.h"
#include "include/knight.h"
#include "include/orc.h"


class TextVisitor : public IFightVisitor {
private:
    TextVisitor(){};

public:
    static std::shared_ptr<IFightVisitor> get()
    {
        static TextVisitor instance;
        return std::shared_ptr<IFightVisitor>(&instance, [](IFightVisitor *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            std::cout << std::endl
                      << "Murder --------" << std::endl;
            attacker->print();
            defender->print();
        }
    }
};

class F_Visitor : public IFightVisitor {
private:
    std::ofstream file;

    F_Visitor()
    {
        file.open("log.txt");
    }

public:
    static std::shared_ptr<IFightVisitor> get()
    {
        static std::shared_ptr<IFightVisitor> instance(new F_Visitor());
        return instance;
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            file << std::endl
                 << "Murder -------- " << std::endl;
            attacker->print(file);
            defender->print(file);
        }
    }
};

std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type)
    {
        switch (type)
        {
        case BearType:
            result = std::make_shared<Bear>(is);
            break;
        case KnightType:
            result = std::make_shared<Knight>(is);
            break;
        case OrcType:
            result = std::make_shared<Orc>(is);
            break;
        }
    }
    else
        std::cerr << "error: unexpected NPC:" << type << std::endl;

    if (result){
        result->subscribe(TextVisitor::get());
        result->subscribe(F_Visitor::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y)
{
    std::shared_ptr<NPC> result;
    switch (type)
    {
    case BearType:
        result = std::make_shared<Bear>(x, y);
        break;
    case KnightType:
        result = std::make_shared<Knight>(x, y);
        break;
    case OrcType:
        result = std::make_shared<Orc>(x, y);
        break;
    default:
        break;
    }
    if (result){
        result->subscribe(TextVisitor::get());
        result->subscribe(F_Visitor::get());
    }

    return result;
}

void save(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open())
    {
        int count;
        is >> count;
        for (int i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    }
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array)
        n->print();
    return os;
}

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && (attacker->is_close(defender, distance)))
            {
                bool success{false};
                success = defender->accept(attacker);

                if (success)
                    dead_list.insert(defender);    
            }

    return dead_list;
}

int main()
{
    set_t array;

    for (size_t i = 0; i < 10; ++i)
        array.insert(factory(NpcType(std::rand() % 3 + 1),
                             std::rand() % 100,
                             std::rand() % 100));
    save(array, "npc.txt");
    array = load("npc.txt");

    for (size_t distance = 20; (distance <= 100) && !array.empty(); distance += 10)
    {
        auto dead_list = fight(array, distance);
        for (auto &d : dead_list)
            array.erase(d);
        std::cout << "Fight stats ----------" << std::endl
                  << "Distance: " << distance << std::endl
                  << "Killed: " << dead_list.size() << std::endl
                  << std::endl << std::endl;

    }
    std::cout << "Survivors:" << array;

    return 0;
}
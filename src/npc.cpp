#include "../include/npc.h"

NPC::NPC(NpcType t, int _x, int _y) : type(t), x(_x), y(_y) {}
NPC::NPC(NpcType t, std::istream &is) : type(t)
{
    is >> x;
    is >> y;
}
//observer подписка
void NPC::subscribe(std::shared_ptr<IFightVisitor> visitor)
{
   visitors.push_back(visitor);
}
//уведомление о победе или не победе  над defender
void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win)
{   //уведомление всем observers
    for (auto &o : visitors)
        o->on_fight(shared_from_this(), defender, win);
}
//проверка дистанции
bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const
{
    if (std::pow(x - other->x, 2) + std::pow(y - other->y, 2) <= std::pow(distance, 2))
        return true;
    else
        return false;
}
//визит -- на каждый случай обработка боя итог результат боя 
bool NPC::visit(std::shared_ptr<Orc> orc) {
    return this->fight(orc);
}
bool NPC::visit(std::shared_ptr<Knight> knight) {
    return this->fight(knight);
}
bool NPC::visit(std::shared_ptr<Bear> bear) {
    return this->fight(bear);
}

void NPC::save(std::ostream &os)
{
    os << x << std::endl;
    os << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc)
{
    os << "{ x:" << npc.x << ", y:" << npc.y << "} ";
    return os;
}
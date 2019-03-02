
#include "id.h"


using namespace SH;

#define ALL 0

ID::ID(const std::string& _type, int _floor)
    : m_type (_type)
    , m_floor (_floor)
    , m_room (ALL)
{}

ID::ID(const std::string& _type, int _floor, unsigned int _room)
    : m_type (_type)
    , m_floor (_floor)
    , m_room (_room)
{}



bool ID::operator==(const ID& _id) const
{
    return (m_type == _id.m_type && m_floor == _id.m_floor && m_room == _id.m_room);
}

bool ID::operator<(const ID& _id) const
{
    if (m_type != _id.m_type)
    {
        return m_type < _id.m_type;
    }
    if (m_floor != _id.m_floor)
    {
        return m_floor < _id.m_floor;
    }
    return (m_room < _id.m_room);
}
    

const std::string& ID::Type() const
{
    return m_type;
}
    

int ID::Floor() const
{
    return m_floor;
}

unsigned int ID::Room() const
{
    return m_room;
}

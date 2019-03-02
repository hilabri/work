#ifndef __ID_H__
#define __ID_H__

#include <string>


namespace SH{


class ID
{
public:
   
    ID() {}
    ID(const std::string& _type, int m_floor);
	ID(const std::string& _type, int m_floor, unsigned int m_room);
	//~ID();

    //ID(const ID& _id);
    //ID& operator=(const ID& _id);

    bool operator==(const ID& _id) const;
    bool operator<(const ID& _id) const;
    
    const std::string& Type() const;
    int Floor() const;
    unsigned int Room() const;

private:
    std::string m_type;
    int m_floor;
	unsigned int m_room;
};

inline ID AllFloor(const ID& _id) { return ID(_id.Type(), _id.Floor()); }

}
#endif


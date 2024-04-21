#ifndef CONTACT_H
#define CONTACT_H
enum class collisionTypes{
    none,
    mid,
    right,
    left,
    top,
    bottom
};

struct contact{
    collisionTypes type;
    float penetration;
};
#endif 
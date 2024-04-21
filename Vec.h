#ifndef VECTOR_H
#define VECTOR_H

class Vector{

public:
    float x, y;

	Vector();

	Vector(float x, float y);

	Vector operator+(Vector const& rhs) const;

	Vector& operator+=(Vector const& rhs);

	Vector operator*(float rhs) const;
};

#endif
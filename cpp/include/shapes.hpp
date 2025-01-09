#ifndef __SHAPES_HPP__ 
#define __SHAPES_HPP__

#include <string>
#include <iostream>

class Shapes
{
public:  
    Shapes(const std::string& name);
    virtual ~Shapes() = 0;
    inline void Draw() const;
    inline void Move() const;
private:
    const std::string m_str;
};

inline void Shapes::Draw() const
{
    std::cout << "Draw: " << m_str << std::endl;
}

inline void Shapes::Move() const
{
    std::cout << "  ";
    Draw();
}

class Line : public Shapes
{
public:
    Line();
};

class Circle : public Shapes
{
public:
    Circle();
};

class Rectangle : public Shapes
{
public:
    Rectangle();
};

class Square : public Shapes
{
public:
    Square();
};

#endif /* __SHAPES_HPP__ */
                    
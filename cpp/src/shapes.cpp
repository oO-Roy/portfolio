#include "shapes.hpp"

Shapes::Shapes(const std::string& name) : m_str(name)
{
    // nothing
}

Shapes::~Shapes()
{
    // nothing
}

Line::Line() : Shapes("Line")
{

}

Circle::Circle() : Shapes("Circle")
{

}

Rectangle::Rectangle() : Shapes("Rectangle")
{

}

Square::Square() : Shapes("Square")
{

}

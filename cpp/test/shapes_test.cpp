#include <list>
#include "shapes.hpp"

using namespace ilrd;

static void Test(std::list<Shapes*> list)
{
    while (!list.empty())
    {
        list.front()->Draw();
        list.front()->Move();
        list.pop_front();
    }
}

int main()
{
    std::list<Shapes*> shapesList;

    shapesList.push_back(new Line());
    shapesList.push_back(new Circle());
    shapesList.push_back(new Rectangle());
    shapesList.push_back(new Square());

    Test(shapesList);

    return 0;
}

#include "gameObject.h"

GameObject::GameObject()
{
    gravity = -0.35;
}

GameObject::~GameObject()
{
    //dtor
}

void GameObject::update()
{

}

string GameObject::debugReport()
{
    ostringstream report;
    report << "CLASS REPORT: GameObject\n"
      << "  Shape body:\n"
      << "    ShapeType type = ";
    switch (this->body.type) {
        case RECTANGLE:
            report << "RECTANGLE"; break;
        case TRIANGLE:
            report << "TRIANGLE"; break;
//        case CIRCLE:
//            report << "CIRCLE"; break;
        default:
            report << "none assigned"; break;
    }
    report << endl
      << "    Vec center = "<< vecPrint(this->body.center) <<"\n"
      << "    Vec corners[3] =";
    for(int cornerNum = 0; cornerNum < 3; cornerNum++) {
        report << vecPrint(this->body.corners[cornerNum]);
    }
    report << endl
      << "    float width = " << this->body.width << endl
      << "    float height = " << this->body.height << endl
      << "    float radius = " << this->body.radius << endl
      << "  ObjectType id = ";
    switch (this->id) {
        case HERO:
            report << "HERO"; break;
        case ENEMY:
            report << "ENEMY"; break;
        case SPIKE:
            report << "SPIKE"; break;
        case PLATFORM:
            report << "PLATFORM"; break;
        default:
            report << "none assigned"; break;
    }
    report << endl
      << "  int rgb[3] = " <<this->rgb[0]<<", "<<this->rgb[1]<<", "<<this->rgb[2]<<endl
      << "  float gravity = " << this->gravity << endl;
    return report.str();
}

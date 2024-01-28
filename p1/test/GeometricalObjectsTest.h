#ifndef GEOMETRICAL_OBJECTS_TEST
#define GEOMETRICAL_OBJECTS_TEST

#include <iomanip>

#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define RESET_TEXT "\033[0m"

class GeometricalObjectsTest
{
public:
    static void TestSum(Geometrical_objects &a, Geometrical_objects &b, Geometrical_objects &sol);
    static void TestSubtract(Geometrical_objects &a, Geometrical_objects &b, Geometrical_objects &sol);
    static void TestDotProduct(Geometrical_objects &a, Geometrical_objects &b, float sol);
    static void TestGetInverse(Base& a, Base& b);
    static void RunAllTests();
};

#endif

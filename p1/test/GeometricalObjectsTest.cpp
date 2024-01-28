#include "../Geometrical_objects.h"
#include "../Base.h"
#include "GeometricalObjectsTest.h"

void GeometricalObjectsTest::TestSum(Geometrical_objects &a, Geometrical_objects &b, Geometrical_objects &sol) {
    Geometrical_objects aux = a + b;
    if (aux == sol){
        cout << GREEN_TEXT << "Suma correcta:            " << a << " + " << b << " = " << sol << endl <<RESET_TEXT;
    }
    else{
        cout << RED_TEXT << "ERROR SUMA:               " << a << " + " << b << " != " << aux << GREEN_TEXT << setw(10) << sol << endl <<RESET_TEXT;
    }
}

void GeometricalObjectsTest::TestSubtract(Geometrical_objects &a, Geometrical_objects &b, Geometrical_objects &sol){
    Geometrical_objects aux = a - b;
    if (aux == sol){
        cout << GREEN_TEXT << "Resta correcta:            " << a << " - " << b << " = " << sol << endl <<RESET_TEXT;
    }
    else{
        cout << RED_TEXT << "ERROR RESTA:               " << a << " - " << b << " != " << aux << GREEN_TEXT << setw(10) << sol << endl <<RESET_TEXT;
    }
}

void GeometricalObjectsTest::TestDotProduct(Geometrical_objects &a, Geometrical_objects &b, float sol){
    float aux = Geometrical_objects::dot(a, b);
    if (aux == sol){
        cout << GREEN_TEXT << "Producto escalar correcto: " << a << " . " << b << " = " << sol << endl <<RESET_TEXT;
    }
    else{
        cout << RED_TEXT << "ERROR PRODUCTO ESCALAR:    " << a << " . " << b << " != " << aux << GREEN_TEXT << setw(10) << sol << endl <<RESET_TEXT;
    }
}

void GeometricalObjectsTest::TestGetInverse(Base& a, Base& b){
    Base::get_Inverse(a, b);
    
    if (a == b){
        cout << GREEN_TEXT << "Matriz inversa correcta" << endl <<RESET_TEXT;
    }
    else{
        cout << RED_TEXT << "ERROR MATRIZ INVERSA: \n" << a << "\n" << GREEN_TEXT << b <<  endl <<RESET_TEXT;
    }
}

void GeometricalObjectsTest::RunAllTests() {
    Geometrical_objects dir1(1.0, 0.0, 3.0, 0);
    Geometrical_objects dir2(-1.0, 4.0, 2.0, 0);

    Geometrical_objects point1(-24.0, 16.0, -2.0, 1);
    Geometrical_objects point2(2.0, -3.0, 4.0, 1);

    Base mTranslation(2.0, -1.0, 4.0, 1);
    cout << "Matriz translacion\n" << mTranslation << endl;

    Base mEscala(3.0, 1.0, 2.0, 2);
    cout << "Matriz escala\n" << mEscala << endl;

    Base mRotacion(-90, 1, 3);
    cout << "Matriz rotacion\n" << mRotacion << endl;

    Base mCambioBase(-5.0, -4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 4);
    cout << "Matriz cambio base\n" << mCambioBase << endl;

    Base mCambioBase2(2.0, 0, 0, 0, 2, 1.0, 0.0, 1.0, 2.0, 1.0, 1.0, 1.0, 4);
    cout << "Matriz cambio base 2\n" << mCambioBase2 << endl;

    //************************************************
    // TEST SUMA VECTORES
    //************************************************
    cout << "\nComprobando suma de vectores...\n";
    Geometrical_objects sol(0.0, 4.0, 5.0, 0);
    TestSum(dir1, dir2, sol);
    sol = sol.change_value(-23, 16, 1, 1);
    TestSum(dir1, point1, sol);
    sol = sol.change_value(3, -3, 7, 1);
    TestSum(dir1, point2, sol);
    sol = sol.change_value(-25, 20, 0, 1);
    TestSum(dir2, point1, sol);
    sol = sol.change_value(1, 1, 6, 1);
    TestSum(dir2, point2, sol);

    //************************************************
    // TEST RESTA VECTORES
    //************************************************
    cout << "\nComprobando resta de vectores...\n";
    sol = sol.change_value(2, -4, 1, 0);
    TestSubtract(dir1, dir2, sol);
    sol = sol.change_value(25, -16, 5, -1);
    TestSubtract(dir1, point1, sol);
    sol = sol.change_value(-1, 3, -1, -1);
    TestSubtract(dir1, point2, sol);
    sol = sol.change_value(23, -12, 4, -1);
    TestSubtract(dir2, point1, sol);
    sol = sol.change_value(-3, 7, -2, -1);
    TestSubtract(dir2, point2, sol);

    //************************************************
    // TEST PRODUCTOR ESCALAR 
    //************************************************
    cout << "\nComprobando producto escalar...\n";
    TestDotProduct(dir1, dir1, 10);
    TestDotProduct(dir2, dir2, 21);
    TestDotProduct(dir1, dir2, 5);
    TestDotProduct(point1, point1, 836);
    TestDotProduct(point2, point2, 29);
    TestDotProduct(point1, point2, -104);
    TestDotProduct(dir1, point1, -30);
    TestDotProduct(dir1, point2, 14);
    TestDotProduct(dir2, point1, 84);
    TestDotProduct(dir2, point2, -6);
   
    //************************************************
    // TEST PRODUCTO VECTORIAL VECTORES
    //************************************************

    /*   Test multiplicacion de matrices
     sol = sol.change_value(2, -4, 1, 0);
     TestSubtract(dir1, dir2, sol);
     sol = sol.change_value(25, -16, 5, -1);
     TestSubtract(dir1, point1, sol);
     sol = sol.change_value(-1, 3, -1, -1);
     TestSubtract(dir1, point2, sol);
     sol = sol.change_value(23, -12, 4, -1);
     TestSubtract(dir2, point1, sol);
     sol = sol.change_value(-3, 7, -2, -1);
     TestSubtract(dir2, point2, sol);
     */

    //************************************************
    // TEST MATRIZ INVERSA
    //************************************************

    Base solBase(2.0, -1.0, 4.0, 1);
    TestGetInverse(mTranslation, solBase);
    Base solBase1(1/3, 1, 0.5, 1);
    TestGetInverse(mEscala, solBase1);
    Base solBase2(1, 0, 0, 0, -0.4123711340206185568, 0.92783505154639175257, 0, -0.92783505154639175257, -0.4123711340206185568, 0, 0, 0, 4);
    TestGetInverse(mRotacion, solBase2);
    Base solBase3(0, 0, 0, 1);
    TestGetInverse(mCambioBase, solBase3); //la matriz inversa no existe
    Base solBase4(0.5, 0, 0, 0, 2/3, -1/3, 0.0, -1/3, 2/3, -0.5, -1/3, -1/3, 4);
    TestGetInverse(mCambioBase2, solBase4); //la matriz inversa no existe

    cout << RESET_TEXT;
}

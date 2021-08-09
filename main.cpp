#include <iostream>
#include "extras/presentacion.h"
using namespace std;

int main()
{
    Persona yo = Persona("Juan", 21,1.68);
    int edad;
    double estatura;
    string nombre;
    cout << "ingrese su edad" << endl;
    cin >> edad;
    cout << "ingrese su estatura" << endl;
    cin >> estatura;
    cout << "ingrese su nombre" << endl;
    cin >> nombre;

    cout << "Hola " + nombre + " tu estatura es " + to_string(estatura) + " y tu edad es " + to_string(edad) << endl;
    yo.mostrar();
    return 0;
}


#include <iostream>
using namespace std;

class Persona
{
private:
    string nombre;
    int edad;
    double estatura;

public:
    Persona(string, int, double);
    virtual void mostrar();
};

Persona::Persona(string _nombre, int _edad, double _estatura)
{
    nombre = _nombre;
    edad = _edad;
    estatura = _estatura;
}

void Persona::mostrar()
{
    cout << "hola mi nombre es " + nombre + " mi edad es " + to_string(edad) + " y mi estatura es de " + to_string(estatura) << endl;
}
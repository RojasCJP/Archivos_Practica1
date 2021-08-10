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
    double getestatura();
    void setEstatura(double);
    void setEdad(int);
    void setNombre(string);
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

double Persona::getestatura(){
    return this->estatura;
}

void Persona::setEstatura(double _estatura){
    this->estatura = _estatura;
}

void Persona::setEdad(int _edad){
    this->edad = _edad;
}

void Persona::setNombre(string _nombre){
    this->nombre = _nombre;
}
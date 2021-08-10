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
    yo.setEdad(edad);
    yo.setNombre(nombre);
    yo.setEstatura(estatura);
    yo.mostrar();

    if(yo.getestatura() >2){
        cout<<"usted es demasiado alto"<<endl;
    }else if (yo.getestatura()<1){
        cout<<"usted es un enano"<<endl;
    }else{
        cout<<"usted tiene una estatura normal"<<endl;
    }
    return 0;
}


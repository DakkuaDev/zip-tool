/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicaci�n con Interfaz Gr�fica de Usuario que permite tratar archivos Zip como documentos: A�adir, Quitar, Listar, Modificar y Guardar
* Adem�s contiene compatibilidad con scripting con Lua
* @Title: Middleware. Pr�ctica Final
* @Date: 10/05/2021
*/

#include <QtWidgets/QApplication>
#include "ZipTool.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ZipTool window;
    window.show();

    return app.exec();
}

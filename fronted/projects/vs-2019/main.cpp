/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicación con Interfaz Gráfica de Usuario que permite tratar archivos Zip como documentos: Añadir, Quitar, Listar, Modificar y Guardar
* Además contiene compatibilidad con scripting con Lua
* @Title: Middleware. Práctica Final
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

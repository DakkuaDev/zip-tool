/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicaci�n con Interfaz Gr�fica de Usuario que permite tratar archivos Zip como documentos: A�adir, Quitar, Listar, Modificar y Guardar
* Adem�s contiene compatibilidad con scripting con Lua
* @Title: Middleware. Pr�ctica Final
* @Date: 05/2021
*/

#pragma once

#include <QtWidgets/QWidget>
#include <memory>
#include "ui_ZipTool.h"
#include "ZipManager.h"

// Lua
#include "lua.hpp"

using namespace std;

class ZipTool : public QWidget
{
    Q_OBJECT


private:
    unique_ptr<ZipManager> manager;

    QString current_selected_file;
    Ui::ZipToolClass ui;

public:
    ZipTool(QWidget *parent = Q_NULLPTR);

    /* Singleton */
    static ZipTool * last_instance;

    /* Abro el contenido zip */
    void open_zip(const std::string& file);

    /* A�ado un archivo al zip */
    void add_archive(const std::string& file);

    /* Elimino un archivo del zip */
    void delete_archive(const std::string& file);


private slots:

    /* M�todo para abrir un archivo zip y listar su contenido */
    void on_click_open_zip();

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_list_view(QModelIndex index);

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_save_zipfile();

    /* Evento invocado al dar click sobre un archivo listado que se desea eliminar*/
    void on_click_delete_archive();

    /* Evento invocado al dar click para a�adir un nuevo archivo al zip */
    void on_click_add_Archive();

    /* Evento incovado para producir una ejecuci�n de lua */
    void on_click_execute_lua();

private:

    /* Actualizo y listo la lista de elementos del zip */
    void refresh_list();

};


#pragma once

#include <QtWidgets/QWidget>
#include <memory>
#include "ui_ZipTool.h"
#include "ZipManager.h"

#include "lua.hpp"

using namespace std;

class ZipTool : public QWidget
{
    Q_OBJECT

public:
    ZipTool(QWidget *parent = Q_NULLPTR);

    //static unique_ptr<ZipTool> last_instance;

private:
    unique_ptr<ZipManager> manager;

    QString current_selected_file;
    Ui::ZipToolClass ui;

private slots:

    /* Método para abrir un archivo zip y listar su contenido */
    void open_zip_file();

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_list_view(QModelIndex index);

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_save_zipfile();

    /* Evento invocado al dar click sobre un archivo listado que se desea eliminar*/
    void on_click_delete_archive();

    /* Evento invocado al dar click para añadir un nuevo archivo al zip */
    void on_click_add_Archive();

    /* Evento incovado para producir una ejecución de lua */
    void on_click_execute_lua();

private:

    void refresh_list();

};


// funciones de lua (C)

//void abrirFiichero(char nombre_fichero[]);
//void anadirFichero(char nuevo_fichero[]);
//void eliminarFichero(char ficheroEliminar[]);

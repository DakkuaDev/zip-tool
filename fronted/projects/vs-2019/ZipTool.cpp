/*
* @Author: Daniel Guerra Gallardo
* @Description: Aplicación con Interfaz Gráfica de Usuario que permite tratar archivos Zip como documentos: Añadir, Quitar, Listar, Modificar y Guardar
* Además contiene compatibilidad con scripting con Lua
* @Title: Middleware. Práctica Final
* @Date: 05/2021
*/

#include "ZipTool.hpp"

#include <QFileDialog>
#include <QStringListModel>
#include <QMessageBox>

// Lua
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"


using namespace std;

ZipTool * ZipTool::last_instance = NULL;

ZipTool::ZipTool(QWidget* parent)
    : QWidget(parent), manager(nullptr)
{
     ZipTool::last_instance = this;

    ui.setupUi(this);

    // Conectores
    connect(ui.btnOpenZip, SIGNAL(released()), this, SLOT(on_click_open_zip()));
    connect(ui.listZipPreview, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click_list_view(QModelIndex)));
    connect(ui.btnSaveContent, SIGNAL(released()), this, SLOT(on_click_save_zipfile()));
    connect(ui.btnDeleteArchive, SIGNAL(released()), this, SLOT(on_click_delete_archive()));
    connect(ui.btnAddArchive, SIGNAL(released()), this, SLOT(on_click_add_Archive()));
    connect(ui.btnExecuteLua, SIGNAL(released()), this, SLOT(on_click_execute_lua()));
}

void ZipTool::open_zip(const std::string& file) 
{
    // Creo mi singleton del ZipManager
    ZipManager manager(file);
    this->manager = make_unique<ZipManager>(manager);

    // Actualizo la UI
    ui.btnAddArchive->setEnabled(true);

    refresh_list();
}

void ZipTool::add_archive(const std::string& file) 
{
    // Llamo al Zip Manager (Backend)
    manager->create_file(file);

    // Actualizo la UI
    ui.btnDeleteArchive->setEnabled(false);

    refresh_list();
}

void ZipTool::delete_archive(const std::string& file) {

    // Llamo al Zip Manager (Backend)
    manager->delete_file(file);

    // Actualizo la UI
    ui.btnDeleteArchive->setEnabled(false);
    ui.txtZipContent->setText("");

    refresh_list();
}


void ZipTool::on_click_open_zip() 
{
    try {
        // Creo un 'File Dialog' que me permita abrir un selector de archivos de Windows
        QFileDialog dialog(this);

        dialog.setNameFilter(tr("Zip File (*.zip)"));
        dialog.setViewMode(QFileDialog::Detail);

        // Creo una listado de archivos
        QStringList fileNames;

        // Si se selecciona un archivo correctamente...
        if (dialog.exec()) 
        {
            fileNames = dialog.selectedFiles();

            // Y contiene más de un archivo...
            if (fileNames.size() > 0) 
            {
                // Llamo al ZipManager (Backend)
                this->open_zip(fileNames[0].toStdString());
            }
        }
        
    }
    catch (...) {
        // Si el usuario abre otro tipo de archivo o sucede algo, salta un mensaje advirtiéndole del fallo
        QMessageBox msgBox;
        msgBox.setText("An Error ocurred when you try to open the archive. Please try it once again.");
        msgBox.exec();

    }
}

void ZipTool::on_click_list_view(QModelIndex index) {

    // Recogo el nombre del archivo
    current_selected_file = index.data().toString();

    // Llamo al Zip Manager
    std::string content = manager->file_content(current_selected_file.toStdString());

    // Actualizo la UI
    ui.txtZipContent->setText(content.c_str());

    ui.btnSaveContent->setEnabled(true);
    ui.txtZipContent->setEnabled(true);
    ui.btnDeleteArchive->setEnabled(true);
}

void ZipTool::on_click_save_zipfile()
{
    // Actualizo el contenido actual
    auto new_content = ui.txtZipContent->toPlainText();
    
    // Llamo al Zip Manager
    manager->update_content(current_selected_file.toStdString(), new_content.toStdString());

    // Creo una alerta
    QMessageBox msgBox;
    msgBox.setText("Save file updated correctly");
    msgBox.exec();
}

void ZipTool::on_click_delete_archive()
{
    delete_archive(current_selected_file.toStdString());
}

void ZipTool::on_click_add_Archive()
{
    try
    {
        // Creo un 'File Dialog' que me permita abrir un selector de archivos de Windows
        QFileDialog dialog(this);

        //dialog.setNameFilter(tr("Zip File (*.zip)"));
        dialog.setViewMode(QFileDialog::Detail);

        // Creo una listado de archivos
        QStringList fileNames;

        // Si se selecciona un archivo correctamente...
        if (dialog.exec())
        {
            fileNames = dialog.selectedFiles();

            // Y contiene más de un archivo...
            for (auto iterator = fileNames.begin(); iterator != fileNames.end(); ++iterator)
            {
                this->add_archive(iterator->toStdString());
            }

        }

    }
    catch (...)
    {
        // Si el usuario abre otro tipo de archivo o sucede algo, salta un mensaje advirtiéndole del fallo
        QMessageBox msgBox;
        msgBox.setText("An Error ocurred when you try to open the archive. Please try it once again.");
        msgBox.exec();
    }
}

void ZipTool::refresh_list()
{
    auto file_list = manager->file_list();
    auto model = new QStringListModel(this);

    // Creo un objeto de Lista de Qt, recorro la lista generada e imprimo los valores
    QStringList qt_list;

    for (list<string>::iterator it = file_list.begin(); it != file_list.end(); it++)
    {
        qt_list << it->c_str();
    }

    // Se lo añado al modelo de Qt
    model->setStringList(qt_list);

    // Muestro el modelo en la UI
    ui.listZipPreview->setEnabled(true);
    ui.listZipPreview->setModel(model);
}


// --------------------------------------  Funciones Puente Lua / C -----------------------------------------//
static int open_zip_lua(lua_State* L) {

    const char* nombre_fichero = lua_tostring(L, 1);

    ZipTool::last_instance->open_zip(nombre_fichero);

    return 0;
}

static int add_archive_lua(lua_State* L) 
{
    const char* nombre_fichero = lua_tostring(L, 1);

    ZipTool::last_instance->add_archive(nombre_fichero);
    return 0;
}

static int delete_archive_lua(lua_State* L) 
{
    const char* nombre_fichero = lua_tostring(L, 1);

    ZipTool::last_instance->delete_archive(nombre_fichero);
    return 0;
}

// --------------------------------------  Funciones Puente Lua / C -----------------------------------------//

void ZipTool::on_click_execute_lua()
{

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Lua Files (*.lua)"));
    dialog.setViewMode(QFileDialog::Detail);

    // Creo una listado de archivos
    QStringList fileNames;

    // Si se selecciona un archivo correctamente...
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();

        // Y contiene más de un archivo...
        if (fileNames.size() > 0)
        {
            // Creo un nuevo estado de Lua (L) y abro sus librerias
            lua_State* L = luaL_newstate();
            luaL_openlibs(L);

            // Agrego una función del script que asocio con una función puente del archivo
            lua_pushcfunction(L, open_zip_lua);
            lua_setglobal(L, "open_zip");

            lua_pushcfunction(L, add_archive_lua);
            lua_setglobal(L, "add_archive");

            lua_pushcfunction(L, delete_archive_lua);
            lua_setglobal(L, "delete_archive");

            // Leo su contenido
            luaL_dofile(L, fileNames[0].toStdString().c_str());
         
            // Cierro el estado de Lua
            lua_close(L);
        }
    }
}



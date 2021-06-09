#include "ZipTool.hpp"
#include <QFileDialog>
#include <QStringListModel>
#include <QMessageBox>

// Lua
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"


using namespace std;

ZipTool::ZipTool(QWidget* parent)
    : QWidget(parent), manager(nullptr)
{
   // ZipTool::last_instance = make_unique<ZipTool>(this);

    ui.setupUi(this);

    // Conectores
    connect(ui.btnOpenZip, SIGNAL(released()), this, SLOT(open_zip_file()));
    connect(ui.listZipPreview, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click_list_view(QModelIndex)));
    connect(ui.btnSaveContent, SIGNAL(released()), this, SLOT(on_click_save_zipfile()));
    connect(ui.btnDeleteArchive, SIGNAL(released()), this, SLOT(on_click_delete_archive()));
    connect(ui.btnAddArchive, SIGNAL(released()), this, SLOT(on_click_add_Archive()));
    connect(ui.btnExecuteLua, SIGNAL(released()), this, SLOT(on_click_execute_lua()));
}


void ZipTool::open_zip_file() 
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
                // 1. Creo un objeto ZipManager y listo el archivo
                ZipManager manager(std::string(fileNames[0].toStdString()));
                this->manager = make_unique<ZipManager>(manager);

                refresh_list();

                ui.btnAddArchive->setEnabled(true);
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

    // Actualizo la casilla del texto con el contenido
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
    // Llamo al Zip Manager
    manager->delete_file(current_selected_file.toStdString());

    ui.btnDeleteArchive->setEnabled(false);

    // Borro el contenido que mostraba el archivo
    ui.txtZipContent->setText("");

    refresh_list();

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
                manager->create_file(iterator->toStdString());
            }

            refresh_list();
            ui.btnDeleteArchive->setEnabled(false);
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


void ZipTool::on_click_execute_lua()
{
    // Creo una alerta
    QMessageBox msgBox;
    msgBox.setText("Lua Open");
    msgBox.exec();

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
            lua_State* L = luaL_newstate();
            luaL_openlibs(L);
            luaL_dofile(L, fileNames[0].toStdString().c_str());
            lua_setglobal(L, "mymodule");
            //  lua_settop(L, 0);

            int top = lua_gettop(L);

            lua_getglobal(L, "mymodule");
            if (!lua_istable(L, -1)) {

                msgBox.setText("Error");
                msgBox.exec();
                //return 0;
            }

            lua_getfield(L, -1, "test");  // -1 means stack top.
            lua_call(L, 0, 1);
            if (lua_isinteger(L, -1))  /* integer? */ {
                msgBox.setText(QString("Resultado Open") + QString::number(lua_tointeger(L, -1)));
                msgBox.exec();
            }

            lua_close(L);
        }
    }
}

// TODO (Comunicación Lua - C++)
//void abrirFiichero(char nombre_fichero[]) {
//    //ZipTool::last_instance.open_zip(nombre_fichero);
//}
//void anadirFichero(char nuevo_fichero[]) {
//    //ZipTool::last_instancea.add_file(nombre_fichero);
//}
//void eliminarFichero(char ficheroEliminar[]) {
//    //ZipTool::last_instancea.delete_file(nombre_fichero);
//}


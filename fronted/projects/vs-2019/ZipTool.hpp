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

private slots:

    /* M�todo para abrir un archivo zip y listar su contenido */
    void open_zip_file();

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_list_view(QModelIndex index);

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_save_zipfile();

private:

    unique_ptr<ZipManager> manager;
    QString actual_archive;
    Ui::ZipToolClass ui;
};
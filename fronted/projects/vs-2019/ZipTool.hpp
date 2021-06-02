#pragma once

#include <QtWidgets/QWidget>
#include <memory>
#include "ui_ZipTool.h"
#include "ZipManager.h"

using namespace std;

class ZipTool : public QWidget
{
    Q_OBJECT

public:
    ZipTool(QWidget *parent = Q_NULLPTR);

private slots:

    /* Método para abrir un archivo zip y listar su contenido */
    void open_zip_file();

    /* Evento invocado al dar click sobre un archivo listado y poder modificar su contenido*/
    void on_click_list_view(QModelIndex index);

private:

    unique_ptr<ZipManager> manager;
    Ui::ZipToolClass ui;
};

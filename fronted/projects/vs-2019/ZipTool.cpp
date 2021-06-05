#include "ZipTool.hpp"
#include <QFileDialog>
#include <QStringListModel>
#include <QMessageBox>


using namespace std;

ZipTool::ZipTool(QWidget* parent)
    : QWidget(parent), manager(nullptr)
{
    ui.setupUi(this);

    // Conectores
    connect(ui.btnOpenZip, SIGNAL(released()), this, SLOT(open_zip_file()));
    connect(ui.listZipPreview, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click_list_view(QModelIndex)));
    connect(ui.btnSaveContent, SIGNAL(released()), this, SLOT(on_click_save_zipfile()));
}


void ZipTool::open_zip_file() 
{
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

            auto file_list = manager.file_list();
            auto model = new QStringListModel(this);

            // 2. Creo un objeto de Lista de Qt, recorro la lista generada e imprimo los valores
            QStringList qt_list;
            
            for (list<string>::iterator it = file_list.begin(); it != file_list.end(); it++)
            {
                qt_list << it->c_str();           
            }

            // 3. Se lo añado al modelo de Qt
            model->setStringList(qt_list);
         
            // 4. Muestro el modelo en la UI
            ui.listZipPreview->setModel(model);

            this->manager = make_unique<ZipManager>(manager);
        }
    }
}

void ZipTool::on_click_list_view(QModelIndex index) {

    // Recojo el contenido actual del archivo
    actual_archive = index.data().toString();

    // Llamo al Zip Manager
    std::string content = manager->file_content(actual_archive.toStdString());

    // Actualizo la casilla del texto con el contenido
    ui.txtZipContent->setText(content.c_str());

    ui.btnSaveContent->setEnabled(true);
}

void ZipTool::on_click_save_zipfile()
{
    // Actualizo el contenido actual
    auto new_content = ui.txtZipContent->toPlainText();
    
    // Llamo al Zip Manager
    manager->update_content(actual_archive.toStdString(), new_content.toStdString());

    // Creo una alerta
    QMessageBox msgBox;
    msgBox.setText("Se ha guardado correctamente el archivo actualizado");
    msgBox.exec();
}

// TODO: 
// 1. Ver que no se pueda abrir otro archivo que no sea un .zip sin que explote
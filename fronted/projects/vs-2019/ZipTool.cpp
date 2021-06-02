#include "ZipTool.hpp"
#include <QFileDialog>
#include <QStringListModel>


using namespace std;

ZipTool::ZipTool(QWidget* parent)
    : QWidget(parent), manager(nullptr)
{
    ui.setupUi(this);

    // Conectores
    connect(ui.btnOpenZip, SIGNAL(released()), this, SLOT(open_zip_file()));
    connect(ui.listZipPreview, SIGNAL(clicked(QModelIndex)), this, SLOT(on_click_list_view(QModelIndex)));
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
        }
    }
}

void ZipTool::on_click_list_view(QModelIndex index) {
    auto texto = index.data().toString();
    ui.txtZipContent->setText(texto);
}
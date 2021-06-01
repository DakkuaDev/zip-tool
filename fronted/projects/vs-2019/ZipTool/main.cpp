#include "ZipTool.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ZipTool window;
    window.show();

    return app.exec();
}

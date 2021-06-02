
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

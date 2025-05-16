#include "main.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MainWindow window;
  //window.show();
  WidgetSudokuBoard w;
 w.show();
  return app.exec();
}

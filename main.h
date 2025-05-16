#pragma once

#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QtCore>

class DLXSolver {
private:
  static constexpr int SIZE_COL = 9 * 9 * 4;
  static constexpr int SIZE_ROW = 9 * 9 * 9;
  static constexpr int SIZE_ONE = SIZE_ROW * 4;
  int ridx[SIZE_ROW];
  int csize[SIZE_COL];
  int left[SIZE_ONE + SIZE_COL + 1];
  int right[SIZE_ONE + SIZE_COL + 1];
  int up[SIZE_ONE + SIZE_COL + 1];
  int down[SIZE_ONE + SIZE_COL + 1];
  int row[SIZE_ONE];
  int col[SIZE_ONE];
  int idx;
  int ans[9][9];
  int step[81];

  void init() {
    memset(ridx, 0, sizeof ridx);
    memset(csize, 0, sizeof csize);
    for (int i = 0; i <= SIZE_COL; ++i) {
      left[i] = i - 1;
      right[i] = i + 1;
      up[i] = i;
      down[i] = i;
    }
    left[0] = SIZE_COL;
    right[SIZE_COL] = 0;
    idx = SIZE_COL;
  }

  void insert(int r, int c) {
    row[++idx] = r;
    col[idx] = c;
    csize[c]++;
    up[idx] = c;
    down[idx] = down[c];
    up[down[c]] = idx;
    down[c] = idx;
    if (ridx[r]) {
      left[idx] = ridx[r];
      right[idx] = right[ridx[r]];
      left[right[ridx[r]]] = idx;
      right[ridx[r]] = idx;
    } else {
      right[idx] = left[idx] = ridx[r] = idx;
    }
  }

  void sudoku_insert(int r, int c, int n) {
    int line = (r - 1) * 81 + (c - 1) * 9 + n;
    int dr = (r - 1) / 3;
    int dc = (c - 1) / 3 + 1;
    int block = dr * 3 + dc;
    insert(line, (r - 1) * 9 + n);
    insert(line, (c - 1) * 9 + n + 81);
    insert(line, (block - 1) * 9 + n + 81 * 2);
    insert(line, (r - 1) * 9 + c + 81 * 3);
  }

  void remove(int c) {
    right[left[c]] = right[c];
    left[right[c]] = left[c];
    for (int i = up[c]; i != c; i = up[i]) {
      for (int j = right[i]; j != i; j = right[j]) {
        down[up[j]] = down[j];
        up[down[j]] = up[j];
        csize[col[j]]--;
      }
    }
  }

  void recover(int c) {
    right[left[c]] = c;
    left[right[c]] = c;
    for (int i = down[c]; i != c; i = down[i]) {
      for (int j = left[i]; j != i; j = left[j]) {
        down[up[j]] = j;
        up[down[j]] = j;
        csize[col[j]]++;
      }
    }
  }

  bool dance(int dep) {
    if (!right[0]) {
      for (int i = 0; i < dep; ++i) {
        int n = step[i];
        ans[(n - 1) / 9 / 9][(n - 1) / 9 % 9] = (n - 1) % 9 + 1;
      }
      return 1;
    }

    int c = right[0];
    for (int i = right[c]; i != c; i = right[i]) {
      if (csize[i] < csize[c])
        c = i;
    }

    remove(c);
    for (int i = down[c]; i != c; i = down[i]) {
      step[dep] = row[i];
      for (int j = right[i]; j != i; j = right[j])
        remove(col[j]);
      if (dance(dep + 1))
        return 1;
      for (int j = left[i]; j != i; j = left[j])
        recover(col[j]);
    }
    recover(c);

    return 0;
  }

public:
  DLXSolver() {
    QCoreApplication* inst = QCoreApplication::instance();
    QString path = inst->applicationDirPath();
    path += "/sudoku.txt";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    init();

    for (int i = 1; i <= 9; ++i) {
      for (int j = 1; j <= 9; ++j) {
        int n;
        in >> n;
        if (n == 0) {
          for (int k = 1; k <= 9; ++k) {
            sudoku_insert(i, j, k);
          }
        } else {
          sudoku_insert(i, j, n);
        }
      }
    }
    file.close();

    if (!dance(0)) {
      qDebug() << "error!";
    }
  }
};

class Sudoku {
private:
  inline static QVector<int> POSSIBLE_VALUES[0x200];
  inline static int SIZE_VALUES[0x200];
  int CONSTRAINTS[81][3];
  int CELLS[81];
  int VALUES[81];
  bool FIXED[81];

  void clear_sudoku() {
    for (int i = 0; i < 81; ++i) {
      CELLS[i] = 0x1ff;
      VALUES[i] = 0;
      FIXED[i] = false;
    }
  }

public:
  explicit Sudoku() {
    POSSIBLE_VALUES[0] = QVector<int>(0);
    SIZE_VALUES[0] = 0;
    int temp[9];
    for (int i = 1; i <= 0x1ff; ++i) {
      int size = 0;
      int mask = 1;
      int j = 1;
      while (mask <= i) {
        if (i & mask) {
          temp[size++] = j;
        }
        ++j;
        mask <<= 1;
      }
      SIZE_VALUES[i] = size;
      POSSIBLE_VALUES[i] = QVector<int>(size);
      int* data = POSSIBLE_VALUES[i].data();
      memcpy(data, temp, size * sizeof(int));
    }

    for (int i = 0; i < 81; ++i) {
      CONSTRAINTS[i][0] = i / 9;
      CONSTRAINTS[i][1] = i % 9;
      int dx = i / 9 / 3;
      int dy = i % 9 / 3;
      CONSTRAINTS[i][2] = dx * 3 + dy + 1;
    }

    clear_sudoku();
  }

  QVector<int> getCandidates(int index) const {
    return POSSIBLE_VALUES[CELLS[index]];
  }
};

class Label : public QLabel {
public:
  explicit Label(QWidget* parent = nullptr) : QLabel(parent) {
    setAlignment(Qt::AlignCenter);
    setStyleSheet(R"(
                      QLabel {
                      margin: 20px;
                      font-size: 40px;
                      }
                      )");
  }
};

class WidgetSudokuBoard : public QWidget {
  Q_OBJECT
protected:
  void paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPen pen;
    pen.setWidthF(0.4);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    int side = qMin(width(), height());
    painter.scale(side / 100.0, side / 100.0);
    painter.drawLine(5, 5, 5, 95);
    painter.drawLine(35, 5, 35, 95);
    painter.drawLine(65, 5, 65, 95);
    painter.drawLine(95, 5, 95, 95);
    painter.drawLine(5, 5, 95, 5);
    painter.drawLine(5, 35, 95, 35);
    painter.drawLine(5, 65, 95, 65);
    painter.drawLine(5, 95, 95, 95);
    pen.setWidthF(0.1);
    painter.setPen(pen);
    painter.drawLine(5, 15, 95, 15);
    painter.drawLine(5, 25, 95, 25);
    painter.drawLine(5, 45, 95, 45);
    painter.drawLine(5, 55, 95, 55);
    painter.drawLine(5, 75, 95, 75);
    painter.drawLine(5, 85, 95, 85);
    painter.drawLine(15, 5, 15, 95);
    painter.drawLine(25, 5, 25, 95);
    painter.drawLine(45, 5, 45, 95);
    painter.drawLine(55, 5, 55, 95);
    painter.drawLine(75, 5, 75, 95);
    painter.drawLine(85, 5, 85, 95);
    painter.resetTransform();
    pen.setColor(Qt::darkGray);
    pen.setWidth(0);
    painter.setPen(pen);
    QFont font("Consolas");
    font.setPointSizeF(1.5);
    painter.setFont(font);
    painter.scale(side / 100.0, side / 100.0);
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        int dx = i * 10;
        int dy = j * 10;
        QVector<int> list = sudoku.getCandidates(i + j * 9);
        for (int k = 0; k < list.size(); ++k) {
          int x = candPosition[list[k] * 2 - 2] + dx;
          int y = candPosition[list[k] * 2 - 1] + dy;
          QPoint point(x, y);
          painter.drawText(point, QString("%1").arg(list[k]));
        }
      }
    }
  }

private:
  Sudoku sudoku;
  int candPosition[18] = {6,  8,  9,  8, 12, 8, 6,  11, 9,
                          11, 12, 11, 6, 14, 9, 14, 12, 14};

public:
  explicit WidgetSudokuBoard(QWidget* parent = nullptr) : QWidget(parent) {
    resize(500, 500);
    setWindowTitle(tr("Sudoku"));
  }
};

class MainWindow : public QMainWindow {
public:
  explicit MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
    setWindowTitle("");
  }

protected:
};
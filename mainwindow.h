#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <opencv2/opencv.hpp>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui
{
class ErodeTool;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString fileName;
    QPixmap pix;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    Mat img;
    QImage image;
    bool isChecked = false;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_open_clicked();

    void on_save_clicked();

    void on_verticalSlider_valueChanged(int value);

    void on_reset_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::ErodeTool *ui;
};
#endif // MAINWINDOW_H

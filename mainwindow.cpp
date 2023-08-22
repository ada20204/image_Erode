#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <opencv2/opencv.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ErodeTool)
{
    ui->setupUi(this);

    // 将checkBox 和 reset 的信号关联
    connect(ui->checkBox, SIGNAL(stateChanged(int)), ui->reset, SLOT(click()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked()
{
    QString newfileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                       "/home",
                                                       tr("Images (*.png *.xpm *.jpg)"));
    if (newfileName.isEmpty())
    {
        return;
    }
    else
    {
        fileName = newfileName;
    }

    if (fileName.isEmpty())
    {
        return;
    }

    ui->filepath->setText(fileName);
    ui->open->setText("重新选择");

    // 显示图片到graphicsView
    // QPixmap pix(fileName);
    // QGraphicsScene *scene = new QGraphicsScene(this);
    // QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pix);

    // scene->addItem(item);
    // ui->graphicsView->setScene(scene);
    // ui->graphicsView->show();

    // 使用opencv读取图片
    img = imread(fileName.toStdString());

    // QString imgName = "C:/Users/HaSee/Desktop/221327.png";
    // Mat img = imread(imgName.toLatin1().data());
    // note: 文件名中不能有中文或者空格

    if (img.empty())
    {
        return;
    }

    Mat imgtmp;
    cvtColor(img, imgtmp, COLOR_BGR2RGB);
    image = QImage((const unsigned char *)(imgtmp.data), imgtmp.cols, imgtmp.rows, imgtmp.cols * imgtmp.channels(), QImage::Format_RGB888);

    pix = QPixmap::fromImage(image);
    scene = new QGraphicsScene(this);
    item = new QGraphicsPixmapItem(pix);

    scene->addItem(item);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void MainWindow::on_save_clicked()
{
    if (fileName.isEmpty())
    {
        return;
    }
    // 保存图片
    QString fileSaveName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(), tr("Images (*.png *.xpm *.jpg)"));

    if (!fileSaveName.isEmpty())
    {
        // 保存 QImage 到文件
        if (image.save(fileSaveName, "png", 100))
        {
            qDebug() << "Image saved successfully.";
        }
        else
        {
            qDebug() << "Failed to save image.";
        }
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    if (fileName.isEmpty())
    {
        return;
    }

    // 定义结构元素 用来腐蚀和膨胀
    Mat struct1, struct2;
    struct1 = getStructuringElement(0, Size(3, 3)); // 矩形结构元素
    struct2 = getStructuringElement(1, Size(3, 3)); // 十字结构元素

    Mat erodeImg;
    // 判断图片是否是白色背景
    if (value >= 0)
    {
        // 腐蚀图片缓存
        erode(img, erodeImg, isChecked ? struct1 : struct2, Point(-1, -1), value);

        // 更新图片
        cvtColor(erodeImg, erodeImg, COLOR_BGR2RGB);
        image = QImage((const unsigned char *)(erodeImg.data), erodeImg.cols, erodeImg.rows, erodeImg.cols * erodeImg.channels(), QImage::Format_RGB888);
        item->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        // 将白底图片转换为黑底图片
        Mat imgtmp;
        bitwise_not(img, imgtmp);
        erode(imgtmp, erodeImg, isChecked ? struct1 : struct2, Point(-1, -1), abs(value));
        bitwise_not(erodeImg, imgtmp);
        cvtColor(imgtmp, imgtmp, COLOR_BGR2RGB);
        image = QImage((const unsigned char *)(imgtmp.data), imgtmp.cols, imgtmp.rows, imgtmp.cols * imgtmp.channels(), QImage::Format_RGB888);
        item->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::on_reset_clicked()
{
    if (fileName.isEmpty())
    {
        return;
    }

    // 重置滑动条
    ui->verticalSlider->setValue(0);

    // 重置图片
    Mat imgtmp;
    cvtColor(img, imgtmp, COLOR_BGR2RGB);
    image = QImage((const unsigned char *)(imgtmp.data), imgtmp.cols, imgtmp.rows, imgtmp.cols * imgtmp.channels(), QImage::Format_RGB888);
    item->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1)
    {
        isChecked = true;
    }
    else
    {
        isChecked = false;
    }
}

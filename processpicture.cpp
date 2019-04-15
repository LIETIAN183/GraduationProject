#include "processpicture.h"
#include <QImage>
#include <QString>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QMessageBox>
using namespace cv;
using namespace std;

ProcessPicture::ProcessPicture(QObject *parent) : QObject(parent)
{
    find_flag = true;
}

//读取图片到Mat
void ProcessPicture::ReadPicture(QString path)
{
    image = imread(path.toStdString(), IMREAD_COLOR);
    //滤波，去噪点
    GaussianBlur(image, image, Size(5, 5), 0, 0, BORDER_DEFAULT);
    //备份
    back = image.clone();
}

//返回图片
QImage ProcessPicture::ReturnImage()
{

    if(image.empty())
    {
        image = imread("/Users/lietian/Desktop/IMG_2088.jpg", IMREAD_COLOR);

    }

    if(image.type() == CV_8UC1) //灰度图
    {
        cvtColor(image, result, COLOR_GRAY2RGB);
    }
    else if (image.type() == CV_8UC3) //彩色图
    {
        cvtColor(image, result, COLOR_BGR2RGB);
    }
    return QImage(static_cast<uchar *>(result.data), result.cols, result.rows, QImage::Format_RGB888);
}

//找到边界
void ProcessPicture::FindBoundary()
{
    //待提升精度--------------------------------------------------------
    image = back.clone();
    //---------------------处理得到二值图像--------
    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(image, image, Size(5, 5), 0, 0, BORDER_DEFAULT);
    //方法1
    //threshold(gray,gray,100,255,THRESH_BINARY | THRESH_OTSU);

    /*Failure Method 叶片太亮则失效，分割方法太简陋
    //二值化为黑白图
    threshold(gray,gray,90,255,THRESH_BINARY);
    //滤波降噪
    medianBlur(gray,gray,21);
    //填充背景，取反得到孔洞图像
    Mat temp=gray.clone();
    floodFill(temp,Point(0,0),Scalar::all(0));
    bitwise_not(gray,gray);
    //add效果重叠白色区域，即高亮区域
    add(temp,gray,gray);
    */
    //方法2
    //边缘扫描
    Canny(gray, gray, 10, 30);
    //闭合边缘曲线
    Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
    morphologyEx(gray, gray, MORPH_CLOSE, element);
    //填充轮廓内部
    Mat temp = gray.clone();
    //四角填充背景 -1因为是类似数组的序号
    int height = gray.rows;
    int width = gray.cols;
    floodFill(temp, Point(0, 0), Scalar::all(255));
    floodFill(temp, Point(0, height - 1), Scalar::all(255));
    floodFill(temp, Point(width - 1, height - 1), Scalar::all(255));
    floodFill(temp, Point(width - 1, 0), Scalar::all(255));
    //----
    bitwise_not(temp, temp);
    add(temp, gray, gray);
    //去除轮廓外细小噪点
    medianBlur(gray, gray, 99);
    //方法3
    //adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,1);
    //medianBlur(gray,gray,3);
    //-----------------获得轮廓--------
    vector<vector<Point>> contours;//存储轮廓点
    vector<Vec4i> hierarchy;//存储不同轮廓间的信息

    findContours(gray, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //判断是否只得到一条轮廓
    if(hierarchy.size() != 1)
    {
        //cout << hierarchy.size() << endl;
        QMessageBox::warning(nullptr, "warning", "FIND BOUNDARY FAILURE");
        find_flag = false;
        return;
    }
    vector<vector<Point>> approxPoint(contours.size());
    //获取控制点
    approxPolyDP(contours[0], approxPoint[0], 10, true);
    //赋值到boundary数组
    boundary.assign(approxPoint[0].begin(), approxPoint[0].end());
    //-------
    //画边缘
    //drawContours(image, approxPoint, -1, Scalar::all(255), 3);
}

bool ProcessPicture::ReturnFlag()
{
    return find_flag;
}

vector<Point> ProcessPicture::ReturnBoundary()
{
    return boundary;
}

QImage ProcessPicture::drawBoundary(vector<Point> points)
{
    image = back.clone();
    //画边缘
    vector<vector<Point>> temp = {points};
    drawContours(image, temp, -1, Scalar::all(255), 3);
    return ReturnImage();
}

int ProcessPicture::width()
{
    return image.cols;
}

int ProcessPicture::height()
{
    return image.rows;
}

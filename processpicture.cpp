#include "processpicture.h"
#include <QImage>
#include <QString>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QOpenGLFunctions>
#include <QMessageBox>
#include <QTime>
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
    cout << "Mat width:" << result.cols << ";height:" << result.rows << endl;
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
    //imshow("gray", gray);
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
    approxPolyDP(contours[0], approxPoint[0], 7, true);
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

//一使用<=就出错？why //==也出错？
bool PointSort(const Point &a, const Point &b)//y小的在前，相同时x小的在前
{
    return a.y < b.y ? true : (a.y > b.y ? false : (a.x < b.x ? true : false));
}

vector<float> ProcessPicture::PointList(vector<Point> points)
{

    int min_x, max_x, min_y, max_y;
    min_x = max_x = points[0].x;
    min_y = max_y = points[0].y;
    for(vector<Point>::iterator i = points.begin(); i != points.end(); i++)
    {
        //cout << i->x << "," << i->y << endl;
        max_x = i->x > max_x ? i->x : max_x;
        min_x = i->x < min_x ? i->x : min_x;
        max_y = i->y > max_y ? i->y : max_y;
        min_y = i->y < min_y ? i->y : min_y;
    }
    cout << min_x << ',' << max_x << ',' << min_y << ',' << max_y << endl;

    //image刷全白
    Mat tmp(image.rows, image.cols, CV_8UC1, Scalar::all(0));
    image = tmp.clone();

    vector<vector<Point>> temp = {points};
    drawContours(image, temp, -1, Scalar::all(255), 1);
    //imshow("show", image);

    sort(points.begin(), points.end(), PointSort);
    /*
    for(vector<Point>::iterator x = points.begin(); x != points.end(); x++)
    {
        cout << "[" << x->x << "," << x->y << "]" << endl;
    }
    */
    vector<Point> middle;

    //每一个控制点叶片两半边对应插值
    for(vector<Point>::iterator i = points.begin(); i != points.end(); i++)
    {
        if(i != points.begin() && i->y == (i - 1)->y) //第一个元素不需要向前比较
        {
            continue;
        }

        if(i != points.end() - 1 && i->y == (i + 1)->y) //最后一个元素不需要向后比较
        {
            middle.push_back(*i);
            middle.push_back(*(i + 1));
            continue;
        }

        vector<Point> temp;
        //判断该点在叶片的左端还是右端
        //int mid = (max_x - min_x) / 2 + min_x;
        for(int j = min_x; j <= max_x; j++)
        {
            if(image.at<uchar>(Point(j, i->y)) == 255)
            {
                temp.push_back(Point(j, i->y));
                break;
            }
        }
        for(int j = max_x; j >= min_x; j--)
        {
            if(image.at<uchar>(Point(j, i->y)) == 255)
            {
                temp.push_back(Point(j, i->y));
                break;
            }
        }

        if(temp.size() == 2)
        {
            middle.push_back(temp.at(0));
            middle.push_back(temp.at(1));
        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "插值出错");
            cout << temp << endl;
        }
        /*
        cout << temp.size() << ";";
        for(vector<Point>::iterator x = temp.begin(); x != temp.end(); x++)
        {
            cout << "[" << x->x << "," << x->y << "],";
        }
        cout << endl;
        */
    }
    cout << middle.size() << endl;
    vector<Point> middle2;//存储插入6个控制点后的控制点数组
    int a = 0, b = 0, delta = 0;
    for(vector<Point>::iterator i = middle.begin(); i != middle.end(); i += 2)
    {
        a = i->x;
        b = (i + 1)->x;
        delta = (b - a) / 6;
        middle2.push_back(*i);
        for(int j = 1; j <= 5; j++)
        {
            middle2.push_back(Point(i->x + j * delta, i->y));
        }
        middle2.push_back(*(i + 1));
    }
    int t = (middle.size() / 2 - 1) % 3;

    //补全控制点
    if(t != 0)
    {
        cout << "t:" << t << endl;
        //获取倒数两个点
        Point p1 = *(middle.end() - 2);
        Point p2 = middle.back();
        for(int j = 0; j < 3 - t; j++)
        {
            middle2.push_back(p1);
            for(int x = 1; x <= 5; x++)
            {
                middle2.push_back(Point(p1.x + x * delta, p1.y));

            }
            middle2.push_back(p2);
        }
    }

    cout << middle2.size() << endl;

    //此时已获得所有控制顶点
    //------------
    //二维点转换三维点
    vector<Point3i> points3i;
    //uint w = 0;
    int w = 0, v = 0;
    for(vector<Point>::iterator i = middle2.begin(); i != middle2.end(); i++)
    {
        if(v % 7 == 0)
        {
            w += 10;
        }
        //qsrand(w++);
        points3i.push_back(Point3i(i->x, i->y, w));

        /*
        if((v - 3) % 7 == 0)
        {
        points3i.push_back(Point3i(i->x, i->y, w));
        w += 10;
        }
        else
        {
        points3i.push_back(Point3i(i->x, i->y, 0));
        }
        */
        //points3i.push_back(Point3i(i->x, i->y, 0/*qrand() % (max_x - min_x) / 15*/));

        v++;
    }

    //----------------------


    //----随机扰动z值


    //-----之后再扰动会出错
    //重新排序控制点以获得可以用于绘制bezier surface的控制点
    vector<Point3i> middle_result;
    //以4x4为构造一个bezier surface；i代表每一块曲线的第一给元素的id
    for(int i = 0; i < static_cast<int>(middle2.size() - 7); i += 21)
    {
        for(int j = 0; j <= 21; j += 7)
        {
            for(int k = 0; k <= 3; k++)
            {
                middle_result.push_back(points3i.at(static_cast<unsigned long>(i + j + k)));
            }
        }

        for(int j = 0; j <= 21; j += 7)
        {
            for(int k = 0; k <= 3; k++)
            {
                middle_result.push_back(points3i.at(static_cast<unsigned long>(i + 3 + j + k)));
            }
        }
    }
    cout << "middle_result Size:" << middle_result.size() << endl;
    //cout << middle2 << endl;

    //构造结束，获得所有控制点对应mat图案上的坐标
    //平移叶片中央到坐标原点，同时反转y轴，因mat y轴向下，opengl y轴向上
    vector<GLfloat> res;
    for(vector<Point3i>::iterator i = middle_result.begin(); i != middle_result.end(); i++)
    {
        i->y = -i->y;
        i->x -= min_x;
        i->y += min_y;
        i->x -= (max_x - min_x) / 2;
        i->y += (max_y - min_y) / 2;
        res.push_back(i->x * 1.75f / (max_x - min_x));
        res.push_back(i->y * 1.75f / (max_x - min_x));
        res.push_back(i->z * 1.75f / (max_x - min_x));
    }

    return res;
}

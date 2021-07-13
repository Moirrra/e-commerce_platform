#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include <QWidget>
#include <QScrollArea>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPushButton>
class ImageView:public QWidget
{
    Q_OBJECT
public:
    ImageView(QWidget *parent = NULL);
    // 设置图片列表;
    void setImageList(QStringList imageFileNameList);
    // 添加图片;
    void addImage(QString imageFileName);
    // 开始播放;
    void startPlay();


private:
    // 初始化图片切换按钮;
    void initChangeImageButton();
    // 绘图事件;
    void paintEvent(QPaintEvent *event);
    // 鼠标点击事件;
    void mousePressEvent(QMouseEvent* event);
    // 用来做图片切换滑动效果，目前以透明度作为切换效果;
    QScrollArea* m_imagePlayWidget;
    // 图片列表;
    QList<QString> m_imageFileNameList;
    // 图片切换时钟;
    QTimer m_imageChangeTimer;
    // 当前显示图片index;
    int m_currentDrawImageIndx;
    // 切换图片;
    QPixmap m_currentPixmap;
    QPixmap m_nextPixmap;
    // 图片切换动画类;
    QPropertyAnimation* m_opacityAnimation;
    // 按钮列表;
    QList<QPushButton*> m_pButtonChangeImageList;

signals:
    void clickImg(int i);

public slots:
    // 图片切换时钟;
    void onImageChangeTimeout();
    // 图片切换按钮点击;
    void onImageSwitchButtonClicked(int buttonId);
};

#endif // IMAGEVIEW_H

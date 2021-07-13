#include "ImageView.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QButtonGroup>

#define CHANGETIME 3000
#define DURATION 800
#define WIDTH 580
#define HEIGHT 380

/*
 * @brief 构造函数
 * @param parent 父窗口
 */
ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
    , m_currentDrawImageIndx(0)
{
    // 添加ImageOpacity属性;
    this->setProperty("ImageOpacity", 1.0);

    // 动画切换类;
    m_opacityAnimation = new QPropertyAnimation(this, "ImageOpacity");
    // 这里要设置的动画时间小于图片切换时间;
    m_opacityAnimation->setDuration(DURATION);
    // 设置ImageOpacity属性值的变化范围;
    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);
    // 透明度变化及时更新绘图;
    connect(m_opacityAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    // 设置图片切换时钟槽函数;
    connect(&m_imageChangeTimer, SIGNAL(timeout()), this, SLOT(onImageChangeTimeout()));

    this->setFixedSize(QSize(WIDTH, HEIGHT));

    this->setWindowFlags(Qt::FramelessWindowHint);
}

/*
 * @brief 初始化轮播图按键
 */
void ImageView::initChangeImageButton()
{
    // 注意图片过多按钮可能放置不下;
    QButtonGroup* changeButtonGroup = new QButtonGroup;
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    for (int i = 0; i < m_imageFileNameList.count(); i++)
    {
        QPushButton* pButton = new QPushButton;
        pButton->setFixedSize(QSize(16, 16));
        pButton->setCheckable(true);
        pButton->setStyleSheet("QPushButton{border-image:url(:/img/icon/white.png);}\
                               QPushButton:checked{border-image:url(:/img/icon/orange.png);}");
                               changeButtonGroup->addButton(pButton, i);
                m_pButtonChangeImageList.append(pButton);
        hLayout->addWidget(pButton);
    }
    hLayout->addStretch();
    hLayout->setSpacing(10);
    hLayout->setMargin(0);
    connect(changeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onImageSwitchButtonClicked(int)));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addLayout(hLayout);
    mainLayout->setContentsMargins(0, 0, 0, 30);
}

/*
 * @brief 设置轮播图列表
 */
void ImageView::setImageList(QStringList imageFileNameList)
{
    m_imageFileNameList = imageFileNameList;
}

/*
 * @brief 添加图片
 */
void ImageView::addImage(QString imageFileName)
{
    m_imageFileNameList.append(imageFileName);
}

/*
 * @brief 开始播放轮播图
 */
void ImageView::startPlay()
{
    // 添加完图片之后，根据图片多少设置图片切换按钮;
    initChangeImageButton();
    if (m_imageFileNameList.count() == 1)
    {
        m_pButtonChangeImageList[m_currentDrawImageIndx]->setChecked(true);
    }
    else if (m_imageFileNameList.count() > 1)
    {
        m_pButtonChangeImageList[m_currentDrawImageIndx]->setChecked(true);
        m_currentPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));
        m_imageChangeTimer.start(CHANGETIME);
        update();
    }
}

/*
 * @brief 轮播图切换
 */
void ImageView::onImageChangeTimeout()
{
    // 设置前后的图片;
    m_currentPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));
    m_currentDrawImageIndx++;
    if (m_currentDrawImageIndx >= m_imageFileNameList.count())
    {
        m_currentDrawImageIndx = 0;
    }
    m_nextPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));

    m_pButtonChangeImageList[m_currentDrawImageIndx]->setChecked(true);

    // 动画类重新开始;
    m_opacityAnimation->start();
}

/*
 * @brief 画图事件
 * @param event 事件
 */
void ImageView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect imageRect = this->rect();

    // 如果图片列表为空，显示默认图片;
    if (m_imageFileNameList.isEmpty())
    {
        QPixmap backPixmap = QPixmap(":/img/pic/default.jpg");
        painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
    // 如果只有一张图片;
    else if (m_imageFileNameList.count() == 1)
    {
        QPixmap backPixmap = QPixmap(m_imageFileNameList.first());
        painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
    // 多张图片;
        else if (m_imageFileNameList.count() > 1)
        {
            float imageOpacity = this->property("ImageOpacity").toFloat();
            painter.setOpacity(1);
            painter.drawPixmap(imageRect, m_nextPixmap.scaled(imageRect.size()));
            painter.setOpacity(imageOpacity);
            painter.drawPixmap(imageRect, m_currentPixmap.scaled(imageRect.size()));
        }
    }

/*
 * @brief 点击轮播图按钮
 * @param buttonId 按键编号
 */
void ImageView::onImageSwitchButtonClicked(int buttonId)
{
    m_currentDrawImageIndx = buttonId - 1;
    if (m_currentDrawImageIndx == -1)
    {
        m_currentDrawImageIndx = m_imageFileNameList.count() - 1;
    }

    onImageChangeTimeout();
    m_imageChangeTimer.start(CHANGETIME);
    update();
}

/*
 * @brief 点击图片事件
 * @param event 事件
 */
void ImageView::mousePressEvent(QMouseEvent* event)
{
    //点击图片发出信号
    qDebug() << m_currentDrawImageIndx;
    emit clickImg(m_currentDrawImageIndx);
//    return __super::mousePressEvent(event);
}


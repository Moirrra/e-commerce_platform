#include "goodsmanage.h"
#include "ui_goodsmanage.h"
#include <QLabel>

/*
 * @brief 构造函数
 * @param parent 父窗口
 * @param ui ui界面
 */
GoodsManage::GoodsManage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GoodsManage)
{
    ui->setupUi(this);

    contentWidget = new QListWidget(this);
    contentWidget->setMovement(QListView::Static);
    contentWidget->setIconSize(QSize(25,25));
    contentWidget->setMaximumWidth(135);
    contentWidget->setSpacing(12);

    pagesWidget = new QStackedWidget(this);
    pagesWidget->setMinimumSize(600,560);


    addGoodsPage = new AddGoods();
    stockPage = new Stock();
    setGoodsPage = new SetGoods();
    pagesWidget->addWidget(addGoodsPage);
    pagesWidget->addWidget(stockPage);
    pagesWidget->addWidget(setGoodsPage);

    createIcons();
    contentWidget->setCurrentRow(0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    ui->centralwidget->setLayout(mainLayout);
}

/*
 * @brief 析构函数
 */
GoodsManage::~GoodsManage()
{
    delete ui;
}

/*
 * @brief 添加图标至控件
 */
void GoodsManage::createIcons()
{
    QListWidgetItem *addGoods = new QListWidgetItem(contentWidget);
    addGoods->setIcon(QIcon(":/img/icon/rukuguanli.png"));
    addGoods->setText(tr("添加商品"));
    addGoods->setTextAlignment(Qt::AlignHCenter);
    addGoods->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *remainSet = new QListWidgetItem(contentWidget);
    remainSet->setIcon(QIcon(":/img/icon/kucunguanli.png"));
    remainSet->setText(tr("库存调整"));
    remainSet->setTextAlignment(Qt::AlignHCenter);
    remainSet->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *setDetails = new QListWidgetItem(contentWidget);
    setDetails->setIcon(QIcon(":/img/icon/xiugai.png"));
    setDetails->setText(tr("商品展示"));
    setDetails->setTextAlignment(Qt::AlignHCenter);
    setDetails->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentWidget, &QListWidget::currentItemChanged, this, &GoodsManage::changePage);
}

/*
 * @brief 切换页
 */
void GoodsManage::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentWidget->row(current));
}

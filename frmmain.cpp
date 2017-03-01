#include "frmmain.h"
#include "ui_frmmain.h"
#include "./Api/iconhelper.h"
#include "./Api/myhelper.h"
#include "./AppConfig/myapp.h"
#include "QsLog/QsLog.h"
#include "./qcustomplot.h"
FrmMain::FrmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    QLOG_INFO()<<"app start...";
    myHelper::FormInCenter(this);
    this->InitStyle();
    //定义两个可变数组存放绘图的坐标数据
        QVector<double> x(101),y(101),x1(101),y1(101);//分别存放x和y坐标的数据,101为数据长度
        //添加数据，我们这里演示y=x^3,为了正负对称，我们x从-10到+10
        for(int i=0;i<101;i++)
        {
            x[i] = i/5 - 10;
            y[i] = x[i] * x[i] * x[i];
            x1[i]=x[i];
            y1[i]=x[i]*100;
        }

        //向绘图区域QCustomPlot(从widget提升来的)添加一条曲线
        ui->qCustomPlot1->addGraph();
        //添加数据
        ui->qCustomPlot1->graph(0)->setData(x,y);
        //向绘图区域QCustomPlot(从widget提升来的)添加一条曲线
        ui->qCustomPlot1->addGraph();
        //添加数据
        ui->qCustomPlot1->graph(1)->setData(x1,y1);
        //设置坐标轴标签名称
        ui->qCustomPlot1->xAxis->setLabel("x");
        ui->qCustomPlot1->yAxis->setLabel("y");

        //设置坐标轴显示范围,否则我们只能看到默认的范围
        ui->qCustomPlot1->xAxis->setRange(-11,11);
        ui->qCustomPlot1->yAxis->setRange(-1100,1100);

        //重绘，这里可以不用，官方例子有，执行setData函数后自动重绘
        //我认为应该用于动态显示或者是改变坐标轴范围之后的动态显示，我们以后探索
        //ui->qCustomPlot->replot();

        connect(ui->btnMainPage,SIGNAL(clicked(bool)),this,SLOT(slotSetMainPage()));
        connect(ui->btnTissueHandle,SIGNAL(clicked(bool)),this,SLOT(slotSetTisseHandle()));
        connect(ui->btnSysClear,SIGNAL(clicked(bool)),this,SLOT(slotSetSysClear()));
}
void FrmMain::slotSetMainPage()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void FrmMain::slotSetTisseHandle()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void FrmMain::slotSetSysClear()
{
    ui->stackedWidget->setCurrentIndex(2);
}
FrmMain::~FrmMain()
{
    delete ui;
}

void FrmMain::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    changSkinMenu=NULL;
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    //IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    ui->lab_Ico->setPixmap(QPixmap(myApp::AppPath+"images/logo.ico"));
    ui->lab_Title->setText(myApp::SoftTitle);
    connect(ui->btnMenu,SIGNAL(clicked(bool)),this,SLOT(slotChangeDisplay()));

    menuBar = new QMenuBar(ui->widget_MenuBar);
    menuBar->setGeometry(QRect(0,0,3000,40));
    /*一级菜单*/
    QMenu* menuAutoRun = menuBar->addMenu(tr("&自动运行"));
    QMenu* menuManualRun = menuBar->addMenu(tr("&手动运行"));
    QMenu* menuProMgr = menuBar->addMenu(tr("&程序管理"));
    QMenu* menuSoupMgr = menuBar->addMenu(tr("&药液管理"));
    QMenu* menuSysMgr = menuBar->addMenu(tr("&系统管理"));
    QMenu* menuDataMgr = menuBar->addMenu(tr("&数据管理"));
    QMenu* menuHelp = menuBar->addMenu(tr("&帮助"));
    /*二级菜单*/
    /*自动运行*/
    QAction *actTissueHandle = menuAutoRun->addAction(QIcon(""),tr("组织处理"));
    QAction *actAutoClear = menuAutoRun->addAction(QIcon(""),tr("自动清洗"));
    QAction *actSetTimer = menuAutoRun->addAction(QIcon(""),tr("定时操作"));
    QAction *appExit = menuAutoRun->addAction(QIcon(""),tr("退出"));
    /*手动运行*/
    QAction *actDehydration = menuManualRun->addAction(QIcon(""),tr("脱水"));
    QAction *actWixdip = menuManualRun->addAction(QIcon(""),tr("浸蜡"));
    QAction *actClear1 = menuManualRun->addAction(QIcon(""),tr("清洗1"));
    QAction *actClear2 = menuManualRun->addAction(QIcon(""),tr("清洗2"));
    QAction *actOperateControl = menuManualRun->addAction(QIcon(""),tr("处理缸控制"));
    /*程序管理*/
    QAction *actTissueHandleMgr = menuProMgr->addAction(QIcon(""),tr("组织处理"));
    QAction *actAutoClearMgr = menuProMgr->addAction(QIcon(""),tr("自动清洗"));
    /*药液管理*/
    QAction *actRealTimeMonitor = menuSoupMgr->addAction(QIcon(""),tr("实时监控"));
    QAction *actSoupChange = menuSoupMgr->addAction(QIcon(""),tr("药液更换"));
    /*系统管理*/
    QAction *actSoupMgr = menuSysMgr->addAction(QIcon(""),tr("药液配置"));
    QAction *actLimitMgr = menuSysMgr->addAction(QIcon(""),tr("权限配置"));
    QAction *actOutMgr = menuSysMgr->addAction(QIcon(""),tr("输出配置"));
    QAction *actYunServer = menuSysMgr->addAction(QIcon(""),tr("云服务器配置"));
    QAction *actBaoMaiMgr = menuSysMgr->addAction(QIcon(""),tr("包埋配置"));
    /*数据管理*/
    QAction *actProcessLog = menuDataMgr->addAction(QIcon(""),tr("程序操作记录"));
    QAction *actSoupChangeLog = menuDataMgr->addAction(QIcon(""),tr("药液更换记录"));
    QAction *actTissueHandleLog = menuDataMgr->addAction(QIcon(""),tr("组织处理记录"));
    QAction *actAutoClearLog = menuDataMgr->addAction(QIcon(""),tr("自动清洗记录"));
    QAction *actErrorLog = menuDataMgr->addAction(QIcon(""),tr("故障记录"));
    //帮助菜单下的项
    QAction *softwareInfo = menuHelp->addAction(QIcon(":/images/image/info.ico"),tr("关于本软件"));
    QAction *contactUs = menuHelp->addAction(QIcon(""),tr("使用说明"));
    QAction *actRepair = menuHelp->addAction(QIcon(""),tr("维修保养"));
    QAction *actFalut = menuHelp->addAction(QIcon(""),tr("故障排查"));

    ui->lcdNumber->setMode(QLCDNumber::Dec);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    clockTimer = new QTimer(this);
    clockTimer->setInterval(500);
    clockTimer->start(500);
    connect(clockTimer,SIGNAL(timeout()),this,SLOT(slotUpdateClock()));
    ui->lab_SoftTitle->setText("全自动快速组织处理系统");

}
void FrmMain::slotUpdateClock()
{
    QDateTime datetime =QDateTime::currentDateTime();
    QString datetimestr = datetime.toString("yyyy-MM-dd hh:mm:ss");
    ui->lcdNumber->display(datetimestr);
}
bool FrmMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmMain::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void FrmMain::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void FrmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmMain::slotChangeDisplay()
{
    static QAction *gray;
    static QAction *navy;
    static QAction *black;
    static QAction *blue;

    if (changSkinMenu == NULL)
    {
        changSkinMenu = new QMenu(this);

        gray = new QAction(changSkinMenu);
        navy = new QAction(changSkinMenu);
        black = new QAction(changSkinMenu);
        blue = new QAction(changSkinMenu);

        gray->setText(tr("灰色"));
        navy->setText(tr("天蓝色"));
        black->setText(tr("黑色"));
        blue->setText(tr("蓝色"));

        changSkinMenu->addAction(gray);
        changSkinMenu->addAction(navy);
        changSkinMenu->addAction(black);
        changSkinMenu->addAction(blue);

        changSkinMenu->setStyleSheet("border: 1px solid black");
        connect(gray, SIGNAL(triggered()),this, SLOT(slotchangedToGray()));
        connect(navy, SIGNAL(triggered()),this, SLOT(slotchangedToNavy()));
        connect(black, SIGNAL(triggered()),this, SLOT(slotchangedToBlack()));
        connect(blue, SIGNAL(triggered()),this, SLOT(slotchangedToBlue()));

    }
    QPoint pos;
    pos.setX(0);
    pos.setY(ui->btnMenu->height());
    changSkinMenu->exec(ui->btnMenu->mapToGlobal(pos));
}
void FrmMain::slotchangedToGray()
{
    myHelper::SetStyle("gray");//灰色风格
}
void FrmMain::slotchangedToBlue()
{
    myHelper::SetStyle("blue");//蓝色色风格
}
void FrmMain::slotchangedToBlack()
{
    myHelper::SetStyle("black");//黑色色风格
}
void FrmMain::slotchangedToNavy()
{
    myHelper::SetStyle("navy");//天蓝色色风格
}

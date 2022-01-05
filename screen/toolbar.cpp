#include "toolbar.h"
#include <QVariant>
#include <QApplication>

#include "screen/workspace.h"
#include "core/utils.h"
#include "core/useroper.h"

ToolBar::ToolBar(Workspace *workspace):QWidget(workspace->widget())
{
    setAttribute(Qt::WA_StyledBackground);

    this->m_workspace = workspace;

    this->setProperty("class","bar");
    this->setCursor(Qt::ArrowCursor);

    createChild();
}

void ToolBar::createChild()
{
    m_layout = new QHBoxLayout();

    m_layout->setContentsMargins(ts(2),ts(2),ts(2),ts(2));
    m_layout->setSpacing(ts(1));

    this->setLayout(m_layout);

    createCreateButtons();
    createFunctionButons();

    connect(UserOper::instance(),SIGNAL(changed()),this,SLOT(operChanged()));
}

void ToolBar::addSeperator()
{
    m_layout->addSpacing(ts(2));

    QFrame *splite = new QFrame();
    splite->setObjectName("vSplit");
    splite->setFrameShape(QFrame::VLine);
    splite->setLineWidth(ts(1));
    m_layout->addWidget(splite);

    m_layout->addSpacing(ts(2));
}

void ToolBar::highlightCreateBtn(QString shapeType)
{
    int index = m_createBtnKeyList.indexOf(shapeType);
    QAbstractButton *btn = btnGroup->button(index);

    btn->setChecked(true);
}

void ToolBar::createCreateButtons()
{
    btnGroup = new QButtonGroup(this);

    createSingleCreateButton(btnGroup,Utils::forRectKey(),QChar(0xe906),tr("rectangle"));
    createSingleCreateButton(btnGroup,Utils::forEllipseKey(),QChar(0xe903),tr("ellipse"));
    createSingleCreateButton(btnGroup,Utils::forLineKey(),QChar(0xe905),tr("line"));
    createSingleCreateButton(btnGroup,Utils::forArrowKey(),QChar(0xe900),tr("arrow"));
    createSingleCreateButton(btnGroup,Utils::forCurveKey(),QChar(0xe90a),tr("pie"));
    createSingleCreateButton(btnGroup,Utils::forTextKey(),QChar(0xe90b),tr("text"));
    createSingleCreateButton(btnGroup,Utils::forMosaicKey(),QChar(0xe909),tr("mosaic"));

    btnGroup->setExclusive(true);

    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(createBtnClicked(int)));
}

void ToolBar::createFunctionButons()
{
    addSeperator();

    m_undoBtn = createSingleFunctionButton(QChar(0xe908),tr("undo"));
    m_redoBtn = createSingleFunctionButton(QChar(0xe90c),tr("redo"));

    m_undoBtn->setEnabled(false);
    m_redoBtn->setEnabled(false);

    addSeperator();

    QPushButton *downloadBtn = createSingleFunctionButton(QChar(0xe907),tr("download"));
    QPushButton *closeBtn = createSingleFunctionButton(QChar(0xe904),tr("exit"),"cancelBtn");
    QPushButton *clipboardBtn = createSingleFunctionButton(QChar(0xe902),tr("finish"),"saveBtn");

    connect(m_undoBtn,SIGNAL(clicked()),this,SLOT(undo()));
    connect(m_redoBtn,SIGNAL(clicked()),this,SLOT(redo()));

    connect(downloadBtn,SIGNAL(clicked()),this,SLOT(downloadBtnClicked()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeProgramBtnClicked()));
    connect(clipboardBtn,SIGNAL(clicked()),this,SLOT(saveBtnClicked()));
}

void ToolBar::createSingleCreateButton(QButtonGroup *group, QString shapeType, QString iconStr,QString tipStr)
{
    QPushButton *btn = new QPushButton();

    btn->setObjectName(shapeType);
    btn->setCheckable(true);
    btn->setProperty("forTip","mainBtn");
    btn->setText(iconStr);
    btn->setToolTip(tipStr);

    group->addButton(btn,m_createBtnKeyList.count());
    m_layout->addWidget(btn);

    m_createBtnKeyList.append(shapeType);
}

QPushButton *ToolBar::createSingleFunctionButton(QString iconStr,QString tipStr,QString objectName)
{
    QPushButton *btn = new QPushButton();

    if(objectName.isEmpty() == false)
    {
        btn->setObjectName(objectName);
    }

    btn->setProperty("forTip","mainBtn");
    //btn->setFont(m_workspace->iconfont());
    btn->setText(iconStr);

    btn->setToolTip(tipStr);

    m_layout->addWidget(btn);

    return btn;
}

void ToolBar::undo()
{
    UserOper::undo();
}

void ToolBar::redo()
{
    UserOper::redo();
}

void ToolBar::operChanged()
{
    m_undoBtn->setEnabled(UserOper::undoNum()>0);
    m_redoBtn->setEnabled(UserOper::redoNum()>0);
}

void ToolBar::createBtnClicked(int index)
{
    QString shapeType = m_createBtnKeyList[index];

    emit createChanged(shapeType);
}

void ToolBar::downloadBtnClicked()
{
    emit download();
}

void ToolBar::closeProgramBtnClicked()
{
    emit closeProgram();
}

void ToolBar::saveBtnClicked()
{
    emit save();
}

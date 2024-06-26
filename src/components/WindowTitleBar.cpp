#include "WindowTitleBar.h"

#include <QIcon>
#include <QApplication>
#include <QStyle>
#include <QPainter>

WindowTitleBar::WindowTitleBar(QWidget *parent): QWidget(parent){
    this->setContentsMargins(0,0,0,0);
    this->setFixedHeight(30);
    this->tabTitleBar = new QWidget();

    QSizePolicy retainWidth = this->tabTitleBar->sizePolicy();
    retainWidth.setRetainSizeWhenHidden(true);
    this->tabTitleBar->setSizePolicy(retainWidth);

    this->tabTitleBar->setContentsMargins(0,0,0,0);
    this->tabTitleBarLayout = new QHBoxLayout(this->tabTitleBar);
    this->tabTitleBarLayout->setContentsMargins(15,0,15,0);
    this->titleBarLayout = new QHBoxLayout(this);
    this->titleBarLayout->setContentsMargins(0,0,0,0);
    this->titleBarLayout->setAlignment(Qt::AlignCenter);
    this->windowButtonsLayout = new QHBoxLayout();
    this->windowButtonsLayout->setAlignment(Qt::AlignCenter);
    this->windowButtonsLayout->setContentsMargins(0,0,0,0);

    this->sideBarButton = new IconButton(":/icons/white/sidebar.png");
    this->groupSelectorButton = new IconButton(":/icons/white/group-selector.png");
    this->backButton = new IconButton(":/icons/white/chevron-left.png");
    this->forwardButton = new IconButton(":/icons/white/chevron-right.png");
    this->reloadButton = new IconButton(":/icons/white/reload.png");
    this->copyLinkButton = new IconButton(":/icons/white/link.png");
    this->addressBox = new AddressBox("search or enter link.");
    this->siteSettingsButton = new IconButton(":/icons/white/page-settings.png");

    this->splitTabMenu = new SplitTabMenu();
    this->splitTabMenu->setButtonIcon(":/icons/white/split.png");

    // auto colorizePixmap = [](const QPixmap &pixmap, const QColor &color) {
    //     QPixmap coloredPixmap(pixmap.size());
    //     coloredPixmap.fill(Qt::transparent);

    //     QPainter painter(&coloredPixmap);
    //     painter.setCompositionMode(QPainter::CompositionMode_Source);
    //     painter.drawPixmap(0, 0, pixmap);
    //     painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    //     painter.fillRect(coloredPixmap.rect(), color);
    //     painter.end();

    //     return coloredPixmap;
    // };

    // Creating window titlebar buttons with white icons
    // QStyle *style = qApp->style();
    // QIcon minimizeIcon = style->standardIcon(QStyle::SP_TitleBarMinButton);
    // QIcon maximizeIcon = style->standardIcon(QStyle::SP_TitleBarMaxButton);
    // QIcon closeIcon = style->standardIcon(QStyle::SP_TitleBarCloseButton);

    // QColor whiteColor(Qt::white);

    // minimizeIcon = QIcon(colorizePixmap(minimizeIcon.pixmap(30, 30), whiteColor));
    // maximizeIcon = QIcon(colorizePixmap(maximizeIcon.pixmap(30, 30), whiteColor));
    // closeIcon = QIcon(colorizePixmap(closeIcon.pixmap(30, 30), whiteColor));

    this->connect(this->sideBarButton, &IconButton::clicked, this, [=](){
        emit this->toggleSideBarRequested();
    });

    this->connect(this->addressBox, &AddressBox::searchRequested, this, [=](){
        emit this->searchRequested();
    });

    this->connect(this->backButton, &IconButton::clicked, this, [=](){
        emit this->previousPageRequested();
    });

    this->connect(this->forwardButton, &IconButton::clicked, this, [=](){
        emit this->nextPageRequested();
    });

    this->connect(this->reloadButton, &IconButton::clicked, this, [=](){
        emit this->reloadRequested();
    });

    this->connect(this->copyLinkButton, &IconButton::clicked, this, [=](){
        emit this->copyLinkRequested();
    });

    this->connect(this->splitTabMenu, &SplitTabMenu::splitTabLeftRequested, this, [=](){
        emit this->splitTabLeftRequested();
    });

    this->connect(this->splitTabMenu, &SplitTabMenu::splitTabRightRequested, this, [=](){
        emit this->splitTabRightRequested();
    });

    this->connect(this->groupSelectorButton, &IconButton::clicked, this, [=](){
        emit this->showGroupBar();
    });

    this->minimize = new IconButton(":/icons/minimize.png");
    this->minimize->scale(13, 13);
    this->maximize = new IconButton(":/icons/maximize.png");
    this->maximize->scale(13, 13);
    this->close = new IconButton(":/icons/close.png");
    this->close->scale(13, 13);

    this->titleBarLayout->addWidget(this->sideBarButton);
    this->titleBarLayout->addWidget(this->groupSelectorButton);
    this->tabTitleBarLayout->addWidget(this->backButton);
    this->tabTitleBarLayout->addWidget(this->forwardButton);
    this->tabTitleBarLayout->addWidget(this->reloadButton);
    this->tabTitleBarLayout->addStretch();
    this->tabTitleBarLayout->addWidget(this->copyLinkButton);
    this->tabTitleBarLayout->addWidget(this->addressBox);
    this->tabTitleBarLayout->addWidget(this->siteSettingsButton);
    this->tabTitleBarLayout->addStretch();
    this->tabTitleBarLayout->addWidget(this->splitTabMenu);

    this->windowButtonsLayout->addWidget(this->minimize);
    this->windowButtonsLayout->addWidget(this->maximize);
    this->windowButtonsLayout->addWidget(this->close);

    this->titleBarLayout->addWidget(this->tabTitleBar);
    this->titleBarLayout->addLayout(this->windowButtonsLayout);
}

QPushButton* WindowTitleBar::minimizeButton(){
    return this->minimize;
}

QPushButton* WindowTitleBar::maximizeButton(){
    return this->maximize;
}

QPushButton* WindowTitleBar::closeButton(){
    return this->close;
}

void WindowTitleBar::setTitle(QString title){
    this->addressBox->setBlank(false);
    this->addressBox->setText(title);
}

void WindowTitleBar::setTitleBarVisible(bool visible){
    this->tabTitleBar->setVisible(visible);
}

WindowTitleBar::~WindowTitleBar(){
    delete this->tabTitleBarLayout;
    delete this->windowButtonsLayout;
    delete this->titleBarLayout;
    delete this->minimize;
    delete this->maximize;
    delete this->close;
    delete this->sideBarButton;
    delete this->addressBox;
    delete this->copyLinkButton;
    delete this->siteSettingsButton;
    delete this->backButton;
    delete this->forwardButton;
    delete this->reloadButton;
    delete this->groupSelectorButton;
}

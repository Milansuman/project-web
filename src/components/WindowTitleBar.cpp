#include "WindowTitleBar.h"

#include <QIcon>
#include <QApplication>
#include <QStyle>

WindowTitleBar::WindowTitleBar(QWidget *parent): QWidget(parent){
    this->setContentsMargins(0,0,0,0);
    this->setFixedHeight(30);
    this->tabTitleBar = new QWidget();

    QSizePolicy retainWidth = this->tabTitleBar->sizePolicy();
    retainWidth.setRetainSizeWhenHidden(true);
    this->tabTitleBar->setSizePolicy(retainWidth);

    this->tabTitleBar->setContentsMargins(0,0,0,0);
    this->tabTitleBarLayout = new QHBoxLayout(this->tabTitleBar);
    this->tabTitleBarLayout->setContentsMargins(0,0,0,0);
    this->titleBarLayout = new QHBoxLayout(this);
    this->titleBarLayout->setContentsMargins(0,0,0,0);
    this->titleBarLayout->setAlignment(Qt::AlignCenter);
    this->windowButtonsLayout = new QHBoxLayout();
    this->windowButtonsLayout->setAlignment(Qt::AlignCenter);
    this->windowButtonsLayout->setContentsMargins(0,0,0,0);

    this->sideBarButton = new IconButton(":/icons/sidebar.png");
    this->backButton = new IconButton(":/icons/chevron-left.png");
    this->forwardButton = new IconButton(":/icons/chevron-right.png");
    this->reloadButton = new IconButton(":/icons/reload.png");
    this->copyLinkButton = new IconButton(":/icons/link.png");
    this->addressBox = new AddressBox("search or enter link.");
    this->siteSettingsButton = new IconButton(":/icons/page-settings.png");

    this->splitTabMenu = new SplitTabMenu();

    //Creating window titlebar buttons
    QStyle *style = qApp->style();
    QIcon minimizeIcon = style->standardIcon(QStyle::SP_TitleBarMinButton);
    QIcon maximizeIcon = style->standardIcon(QStyle::SP_TitleBarMaxButton);
    QIcon closeIcon = style->standardIcon(QStyle::SP_TitleBarCloseButton);

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

    this->minimize = new QPushButton();
    this->minimize->setIcon(minimizeIcon);
    this->minimize->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );

    this->maximize = new QPushButton();
    this->maximize->setIcon(maximizeIcon);
    this->maximize->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );

    this->close = new QPushButton();
    this->close->setIcon(closeIcon);
    this->close->setStyleSheet(
        "QPushButton{"
        "   background: transparent;"
        "   border: none;"
        "}"
    );

    this->titleBarLayout->addWidget(this->sideBarButton);
    this->tabTitleBarLayout->addWidget(this->backButton);
    this->tabTitleBarLayout->addWidget(this->forwardButton);
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
}
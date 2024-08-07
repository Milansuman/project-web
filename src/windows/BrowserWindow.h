#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPaintEvent>
#include <QRegion>
#include <QPropertyAnimation>

#include "../components/WindowTitleBar.h"
#include "../components/TabManager.h"
#include "../components/SideBar.h"
#include "SpotlightDialog.h"

#ifdef _WIN32
#include <windows.h>
#endif

class BrowserWindow : public QMainWindow
{
    Q_OBJECT
private:
    WindowTitleBar *titleBar;
    TabManager *tabManager;
    QVBoxLayout *layout;
    QHBoxLayout *contentLayout;
    QWidget *centralWidget;
    SideBar *sideBar;
    SpotlightDialog *spotlightDialog;

    bool isMaximized;
    QPropertyAnimation *sideBarAnimation;

    void hideSideBar();
    bool isEdgePosition(QPointF position);
    QFlags<Qt::Edge> getEdgePosition(QPointF position);

#ifdef _WIN32
    void enableBlurBehind();
#endif

public:
    BrowserWindow(QSize size, QWidget *parent = nullptr);
    BrowserWindow(QUrl url, QSize size, QWidget *parent = nullptr);
    void toggleSideBar();

    // #ifdef __linux__
    void show();
    // #endif
    ~BrowserWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};
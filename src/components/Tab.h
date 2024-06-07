#pragma once

#include <QWidget>
#include <QWebEngineProfile>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>

#include "WebView.h"
#include "TabTitleBar.h"
#include "../windows/SearchDialog.h"

class Tab : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *layout;
    WebView *webview;
    TabTitleBar *tabTitleBar;
    SearchDialog *searchDialog;
public:
    Tab(QWebEngineProfile *profile, QWidget *parent=nullptr);
    Tab(QWebEngineProfile *profile, QString url, QWidget *parent=nullptr);
    void setTitleBarVisible(bool visible);
    ~Tab();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
signals:
    void closeTabRequested();
    void splitTabLeftRequested();
    void splitTabRightRequested();
};
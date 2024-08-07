#pragma once

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QPaintEvent>
#include <QEvent>
#include <QQuickWidget>

class WebView : public QWebEngineView{
    Q_OBJECT
private:
    QQuickWidget *pageSurface;
    QColor getTopColor();
public:
    WebView(QWebEngineProfile *profile, QWidget *parent = nullptr);
    bool event(QEvent *event);
    WebView(QWidget *parent=nullptr);
protected:
    bool eventFilter(QObject *object, QEvent *event);
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type) override;
signals:
    void colorChanged(QColor color);
};
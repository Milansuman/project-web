#include "SpotlightDialog.h"

#include <QPainterPath>
#include <QPainter>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QList>
#include <QUrl>
#include <QPainter>

SpotlightDialog::SpotlightDialog(QWidget *parent): QDialog(parent), m_pos(0), m_group(0), initialized(false){
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->layout = new QVBoxLayout(this);

    this->initialize();
}

void SpotlightDialog::initialize(){
    this->webview = new WebView();
    this->webview->page()->setBackgroundColor(QColor(0, 0, 0, 1));
    //this->webview->load(QUrl("http://localhost:5173"));

    this->channel = new QWebChannel(this->webview->page());
    this->webview->page()->setWebChannel(this->channel);

    this->tabsApi = new TabsApi();
    this->channel->registerObject("tabs", this->tabsApi);

    this->connect(this->tabsApi, &TabsApi::splitTabRequested, this, [=](QUrl url){
        emit this->splitTabRequested(url);
    });

    this->connect(this->tabsApi, &TabsApi::splitTabHomeRequested, this, [=](){
        emit this->splitTabHomeRequested();
    });

    this->connect(this->tabsApi, &TabsApi::newTabRequested, this, [=](QUrl url){
        emit this->newTabRequested(url);
    });

    this->connect(this->tabsApi, &TabsApi::splitTabFlipRequested, this, [=](){
        emit this->splitTabFlipRequested();
    });

    this->connect(this->tabsApi, &TabsApi::addTabsRequested, this, [=](QList<QList<QUrl>> tabsList){
        emit this->addTabsRequested(tabsList);
    });

    this->connect(this->tabsApi, &TabsApi::loadUrl, this, [=](int group, int tab, QString url){
        emit this->loadUrl(group, tab, url);
    });

    this->fileApi = new FileApi();
    this->channel->registerObject("fs", this->fileApi);

    this->dialogApi = new DialogApi();
    this->channel->registerObject("dialog", this->dialogApi);

    this->ollamaApi = new OllamaApi();
    this->channel->registerObject("ollama", this->ollamaApi);

    this->searchSuggestionsApi = new SearchSuggestionsApi();
    this->channel->registerObject("searchSuggestions", this->searchSuggestionsApi);

    this->channel->registerObject("misc", this);

    this->connect(this->dialogApi, &DialogApi::closeDialogRequested, this, [=](){
        this->accept();
    });

    QWebEngineScript script;
    script.setName("WebChannelScript");
    script.setSourceCode(R"(
        var script = document.createElement('script');
        script.src = 'qrc:///qtwebchannel/qwebchannel.js';

        script.onload = function() {
            new QWebChannel(qt.webChannelTransport, function(channel) {
                window.tabs = channel.objects.tabs;
                window.fs = channel.objects.fs;
                window.dialog = channel.objects.dialog;
                window.suggestions = {
                    getDuckDuckGoSuggestions: (text) => {
                        return new Promise((resolve, reject) => {
                            channel.objects.searchSuggestions.getDuckDuckGoSuggestions(text);

                            channel.objects.searchSuggestions.duckDuckGoSuggestionsGenerated.connect((response) => {
                                resolve(response);
                            });
                        });
                    }
                }
                window.ai = {
                    generate: (model, prompt) => {
                        return new Promise((resolve, reject) => {
                            channel.objects.ollama.generate(model, prompt);

                            channel.objects.ollama.responseGenerated.connect((response) => {
                                resolve(response);
                            })
                        });
                    }
                };

                window.currentTab = channel.objects.misc.tab;
                window.currentGroup = channel.objects.misc.group;

                channel.objects.misc.tabChanged.connect((tab) => {
                    window.currentTab = tab;
                });

                channel.objects.misc.groupChanged.connect((group) => {
                    window.currentGroup = group;
                });
            });

            function inject(fn) {
                const script = document.createElement('script')
                script.text = `(${fn.toString()})();`
                document.documentElement.appendChild(script)
            }


            function run() {
                Object.defineProperty(navigator, 'userAgent', {
                    get: function () { return 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36'; }
                });

                console.log(navigator.userAgent)

                window.ai =  {
                    canCreateGenericSession: async () => "readily",
                    canCreateTextSession: async () => "readily",
                    defaultTextSessionOptions: async () => {},
                    createTextSession: async () => {
                    function createStreamFromApi(message) {
                        let cumulativeContent = '';
                        const stream = new ReadableStream({
                        start(controller) {
                            fetch('http://localhost:11434/api/chat', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/json'
                            },
                            body: JSON.stringify({
                                model: OLLAMA_MODEL_NAME,
                                messages: [
                                {
                                    role: 'user',
                                    content: message
                                }
                                ]
                            })
                            })
                            .then(response => {
                            const reader = response.body.getReader();
                            const decoder = new TextDecoder();
                            
                            function push() {
                                reader.read().then(({ done, value }) => {
                                if (done) {
                                    controller.close();
                                    return;
                                }
                                
                                const text = decoder.decode(value, { stream: true });
                                const jsonObjects = text.trim().split('\n').map(line => JSON.parse(line));
                                for (const jsonObject of jsonObjects) {
                                    if (jsonObject.message && jsonObject.message.content) {
                                    cumulativeContent += jsonObject.message.content
                                    controller.enqueue(cumulativeContent);
                                    }
                                }
                                
                                push();
                                });
                            }
                            
                            push();
                            })
                            .catch(error => {
                            console.error(error);
                            controller.error(error);
                            });
                        }
                        });
                        
                        return stream;
                    }


                    return {
                        promptStreaming: createStreamFromApi
                    }
                    }
                }
            }


            inject(run)
        };
        document.getElementsByTagName('head')[0].appendChild(script);
    )");

    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setRunsOnSubFrames(true);

    //WebView *devtools = new WebView();
    //devtools->page()->setInspectedPage(this->webview->page());

    this->webview->page()->scripts().insert(script);
    this->layout->addWidget(this->webview);
    //this->layout->addWidget(devtools);
}

void SpotlightDialog::finalize(){
    this->webview->load(QUrl("qrc:/extensions/spotlight/index.html"));
    //this->webview->load(QUrl("http://localhost:5173"));
}

void SpotlightDialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setBrush(QBrush(QColor(0, 0, 0, 80)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void SpotlightDialog::showEvent(QShowEvent *event){
    if(!this->initialized){
        this->finalize();
        this->initialized = true;
    }
    QDialog::showEvent(event);
}

void SpotlightDialog::open(int pos, int group){
    this->m_pos = pos;
    this->m_group = group;
    emit this->tabChanged(pos);
    emit this->groupChanged(group);
    this->setFixedSize(this->parentWidget()->size());
    this->move(this->parentWidget()->mapToGlobal(QPoint(0, 0)));
    QDialog::open();
}

int SpotlightDialog::getTab(){
    return this->m_pos;
}

int SpotlightDialog::getGroup(){
    return this->m_group;
}

SpotlightDialog::~SpotlightDialog() = default;
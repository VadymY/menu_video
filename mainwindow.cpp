#include "mainwindow.h"
#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QApplication>

static int x_2, y_2, w_2, h_2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    //! [0]

    //! [1]
    QWidget *topFiller = new QWidget(this);
    //    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //    setLang = new QAction(tr("En"), this);
    //    newAct->setShortcuts(QKeySequence::New);
    //    newAct->setStatusTip(tr("Create a new file"));
    //    connect(setLang, &QAction::triggered, this, &MainWindow::setLanguages);
    //    topFiller->addAction(setLang);
    //    topFiller->show();

    //    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
    //                              "invoke a context menu</i>"));
    //    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    //    infoLabel->setAlignment(Qt::AlignCenter);


    QGridLayout *layout = new QGridLayout;
    //    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->addWidget(topFiller, 0, 0, 1, 1);
    //    topFiller->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_videoWidget = new QVideoWidget(this);
    //    m_videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_videoWidget, 1, 0 , 24, 20);
    //    m_videoWidget->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //    layout->addWidget(infoLabel);
    QWidget *bottomFiller = new QWidget(this);
    QVBoxLayout * v_box = new QVBoxLayout(bottomFiller);

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setRange(0, 99);
    m_slider->setValue(0);
    //    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //    QHBoxLayout * hbox_play = new QHBoxLayout(bottomFiller);
    QHBoxLayout * hbox = new QHBoxLayout(bottomFiller);
    play_bt = new QPushButton(tr("Play"));
    play_bt->setFixedSize(50, m_height);
    pause_bt = new QPushButton(tr("Pause"));
    pause_bt->setFixedSize(50, m_height);
    stop_bt = new QPushButton(tr("Stop"));
    stop_bt->setFixedSize(50, m_height);
    mute_bt = new QPushButton(tr("UnMute"));
    QIcon mute_icon(":/sound/volume.png");
    mute_bt->setIcon(mute_icon);
    mute_bt->setFixedSize(90, m_height);

    duration_bt = new QPushButton();
    duration_bt->setFixedSize(180, m_height);

    QComboBox * rate_cont = new QComboBox();
    rate_cont->resize(40, 40);
    rate_cont->addItem("0.5x");
    rate_cont->addItem("1.0x");
    rate_cont->addItem("1.5x");
    rate_cont->addItem("2.0x");
    rate_cont->setCurrentIndex(1);
    rate_cont->setFixedSize(70, m_height);

    hbox->addWidget(play_bt);
    hbox->addWidget(pause_bt);
    hbox->addWidget(stop_bt);
    hbox->addWidget(mute_bt);
    hbox->addWidget(duration_bt);
    hbox->addWidget(rate_cont);
    m_volume = new QSlider(Qt::Horizontal);
    m_volume->setRange(0, 99);
    m_volume->setValue(30);
    m_volume->setMaximumWidth(200);
    hbox->addWidget(m_volume);
    fr_get = new QPushButton(tr("Screenshot"));
    fr_get->setFixedSize(100, m_height);
    hbox->addWidget(fr_get);

    QPushButton * seek_left = new QPushButton(tr("Left"));
    seek_left->setFixedSize(50, m_height);
    hbox->addWidget(seek_left);

//    QTextEdit * seek_dur = new QTextEdit("1");
//    seek_dur->setPlainText("1000");
//    seek_dur->setFixedSize(70, m_height);
//    hbox->addWidget(seek_dur);

    QComboBox * seek_meas = new QComboBox();
    seek_meas->addItem("ms");
    seek_meas->addItem("sec");
    seek_meas->addItem("min");
    seek_meas->addItem("hour");
    seek_meas->setFixedSize(65, m_height);
    hbox->addWidget(seek_meas);

    QPushButton * seek_right = new QPushButton(tr("Right"));
    seek_right->setFixedSize(50, m_height);
    hbox->addWidget(seek_right);

    connect(play_bt, &QPushButton::clicked, this, [&](){
        if(m_player->isMetaDataAvailable()){
            m_player->play();
            m_2player->play();
            //            m_videoWidget->size();
            //            QVideoSurfaceFormat frm = QVideoFrame::Format_ARGB32;
            //            mysurf->start(frm);

            play_bt->setStyleSheet(m_select_str);
            pause_bt->setStyleSheet(m_normal_str);
            stop_bt->setStyleSheet(m_normal_str);
            this->duration = m_player->duration();
            duration_str = getTime(this->duration);
            this->cur_position = m_player->position();
            QString dur  = getTime(this->duration);
            QString cur_pos  = getTime(this->cur_position);
            m_slider->setMaximum(999);
            m_player->setVolume(30);
            m_volume->setValue(30);
            percent_play = (this->cur_position  * 1000u) / this->duration;
            duration_bt->setText(cur_pos + QString("/") + dur);
            QString message = tr("Playing video");
            statusBar()->showMessage(message);
        }
    });
    connect(pause_bt, &QPushButton::clicked, this, [&](){
        m_player->pause();
        m_2player->pause();
        play_bt->setStyleSheet(m_normal_str);
        pause_bt->setStyleSheet(m_select_str);
        stop_bt->setStyleSheet(m_normal_str);
        QString message = tr("Paused video");
        statusBar()->showMessage(message);
    });
    connect(stop_bt, &QPushButton::clicked, this, [&](){
        m_player->stop();
        m_2player->stop();
        m_slider->setValue(0);
        play_bt->setStyleSheet(m_normal_str);
        pause_bt->setStyleSheet(m_normal_str);
        stop_bt->setStyleSheet(m_select_str);
        QString message = tr("Stopped video");
        statusBar()->showMessage(message);
    });
    connect(mute_bt, &QPushButton::clicked, this, [&](){
        m_mute  = !m_mute;
        if(m_mute){
            mute_bt->setText(tr("Mute"));
            QIcon mute_icon(":/sound/no_volume.png");
            mute_bt->setIcon(mute_icon);
            m_player->setMuted(true);
            QString message = tr("Muted video");
            statusBar()->showMessage(message);
        }
        else{
            mute_bt->setText(tr("UnMute"));
            QIcon mute_icon(":/sound/volume.png");
            mute_bt->setIcon(mute_icon);
            m_player->setMuted(false);
            QString message = tr("Unmuted video");
            statusBar()->showMessage(message);
        }
    });

    connect(rate_cont, SIGNAL(currentIndexChanged(int)), this, SLOT(index_rate(int)));
    mysurf = new MyVideoSurface();
    m_player = new QMediaPlayer(m_videoWidget);
    m_2player = new QMediaPlayer(m_videoWidget);
    m_player->setVideoOutput(m_videoWidget);
    m_2player->setVideoOutput(mysurf);
    m_2player->setMuted(true);

    connect(fr_get, &QPushButton::clicked, this, [&](){
        mysurf->is_can_write = true;
        mysurf->pos_play = m_player->position();
    });

    connect(m_player, &QMediaPlayer::positionChanged, this, [&](){
        if(m_player->isMetaDataAvailable()){
            qint64 new_cur_position = m_player->position();
            if (new_cur_position > this->cur_position - 1000u){
                this->cur_position = m_player->position();
                QString cur_pos  = getTime(this->cur_position);
                duration_bt->setText(cur_pos + QString("/") + duration_str);
                int mn = m_slider->minimum();
                int mx = m_slider->maximum();
                int new_percent_play = (this->cur_position  * 1000u)/ this->duration;
                if (new_percent_play != percent_play){
                    m_slider->setValue(new_percent_play);
                    percent_play = new_percent_play;
                }
            }
        }
    });

    connect(m_slider, &QSlider::sliderReleased, m_player, [&](){
        int mn = m_slider->minimum();
        int mx = m_slider->maximum();
        int vl = m_slider->value();
        float play_pos = (float)(vl - mn) / (mx  - mn);
        this->cur_position = m_player->position();
        float rel = (float)this->cur_position / this->duration;
        float koef = play_pos / rel;
        this->cur_position *= koef;
        m_player->setPosition(this->cur_position);
        m_2player->setPosition(this->cur_position);
    });

    connect(m_volume, &QSlider::sliderMoved, m_player, [&](int volume){
        m_player->setVolume(volume);
    });

    hbox->setAlignment(Qt::AlignLeft);
    v_box->addWidget(m_slider);
    v_box->addLayout(hbox);
    layout->addWidget(bottomFiller , 60, 0, 1, 20);
    widget->setLayout(layout);


    //! [1]
    translator = new QTranslator;
    //! [2]
    createActions();
    createMenus();
    //    SetWindowTextA(this->windowHandle(), tr("Main window"));

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);
}

MainWindow::~MainWindow()
{

}

void MainWindow::index_rate(int idx)
{
    switch(idx){
    case 0:
        emit m_player->setPlaybackRate(0.5);
        emit m_2player->setPlaybackRate(0.5);
        break;
    case 1:
        emit m_player->setPlaybackRate(1.0);
        emit m_2player->setPlaybackRate(1.0);
        break;
    case 2:
        emit m_player->setPlaybackRate(1.5);
        emit m_2player->setPlaybackRate(1.5);
        break;
    case 3:
        emit m_player->setPlaybackRate(2.0);
        emit m_2player->setPlaybackRate(2.0);
        break;
    }
}

QString MainWindow::getTime(qint64 duration)
{
    QString str("");
    qint64 ms = duration % 1000u;
    duration /= 1000u;
    qint64 sec = duration % 60u;
    duration /= 60u;
    qint64 min = duration % 60u;
    duration /= 60u;
    qint64 hour = duration % 60u;
    if (hour > 0u){
        str = QString::number(hour) + QString{":"};
    }
    str += QString::number(min) + QString{":"};
    str += QString::number(sec) + QString{":"};
    str += QString::number(ms);
    return str;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //    QMenu menu(this);
    //    menu.addAction(cutAct);
    //    menu.addAction(copyAct);
    //    menu.addAction(pasteAct);
    //    menu.exec(event->globalPos());
}

void MainWindow::retranslateUi(QMainWindow *MainWindow)
{
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
    fileMenu->setTitle(QApplication::translate("MainWindow", "Video", nullptr));
    fileAct->setText(QApplication::translate("MainWindow", "Open from file", nullptr));
    streamAct->setText(QApplication::translate("MainWindow", "Open from stream", nullptr));
    hdmiAct->setText(QApplication::translate("MainWindow", "Open from HDMI", nullptr));
    printAct->setText(QApplication::translate("MainWindow", "Print...", nullptr));
    exitAct->setText(QApplication::translate("MainWindow", "Exit", nullptr));

    langMenu->setTitle(QApplication::translate("MainWindow", "Language", nullptr));
    setLangEn->setText(QApplication::translate("MainWindow", "English", nullptr));
    setLangUkr->setText(QApplication::translate("MainWindow", "Ukrainian", nullptr));
    setLangRu->setText(QApplication::translate("MainWindow", "Russian", nullptr));

    play_bt->setText(QApplication::translate("MainWindow", "Play", nullptr));
    pause_bt->setText(QApplication::translate("MainWindow", "Pause", nullptr));
    stop_bt->setText(QApplication::translate("MainWindow", "Stop", nullptr));

    if(m_mute){
        mute_bt->setText(QApplication::translate("MainWindow", "Mute", nullptr));
    }
    else{
        mute_bt->setText(QApplication::translate("MainWindow", "UnMute", nullptr));
    }
    int h = 5;
} // retranslateUi

void MainWindow::setEnglish()
{
    setLangEn->setChecked(true);
    setLangUkr->setChecked(false);
    setLangRu->setChecked(false);
    translator->load(":/langs/main_en.qm");
    qApp->installTranslator(translator);
    retranslateUi(this);
    int h = 5;
}

void MainWindow::setUkraine()
{
    setLangUkr->setChecked(true);
    setLangEn->setChecked(false);
    setLangRu->setChecked(false);
    translator->load(":/langs/main_ukr.qm");
    qApp->installTranslator(translator);
    retranslateUi(this);
    int h = 5;
}

void MainWindow::setRussia()
{
    setLangRu->setChecked(true);
    setLangEn->setChecked(false);
    setLangUkr->setChecked(false);
    translator->load(":/langs/main_ru.qm");
    qApp->installTranslator(translator);
    retranslateUi(this);
    int h = 5;
}

void MainWindow::newFile()
{
    QString selfilter = tr("JPEG (*.mp4 *.mkv *.mov)");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open video file"), "./", tr("All files (*.*);; Video Files (*.mp4 *.mkv *.mov)"),
                                                    &selfilter);

    m_player->setMedia(QUrl::fromLocalFile(fileName));
    m_2player->setMedia(QUrl::fromLocalFile(fileName));
    this->duration = m_player->duration();
    QString dur  = QString::number(this->duration);
    duration_bt->setText(QString("-/") + dur);
    //    m_player->setMedia(QUrl::fromLocalFile("E://My//RemoteJob//2022//Develop neural network//QT_VIDEO_2//Video//2_Advanced.mp4"));
    // "2 С+Advanced.mp4"
    // E://My//RemoteJob//2022//Develop neural network//QT_VIDEO_2//Video/2_Advanced.mp4
    //    m_player->play();
    //    infoLabel->setText(tr("Invoked <b>File|New</b>"));
    mysurf->file_video = fileName;
}

void MainWindow::open()
{
    //    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}

void MainWindow::save()
{
    //    infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::print()
{
    //    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::undo()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::redo()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void MainWindow::paste()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::bold()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void MainWindow::italic()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void MainWindow::leftAlign()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void MainWindow::rightAlign()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void MainWindow::justify()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
}

void MainWindow::center()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void MainWindow::setLineSpacing()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::setParagraphSpacing()
{
    //    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}

void MainWindow::about()
{
    //    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
                       tr("The <b>Menu</b> example shows how to create "
                          "menu-bar menus and context menus."));
}

void MainWindow::aboutQt()
{
    //    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

//! [4]
void MainWindow::createActions()
{
    //! [5]
    fileAct = new QAction(tr("Open from file"), this);
    fileAct->setShortcuts(QKeySequence::New);
    fileAct->setStatusTip(tr("Open video from file"));
    connect(fileAct, &QAction::triggered, this, &MainWindow::newFile);
    //! [4]

    streamAct = new QAction(tr("Open from stream"), this);
    streamAct->setShortcuts(QKeySequence::Open);
    streamAct->setStatusTip(tr("Open video from stream"));
    connect(streamAct, &QAction::triggered, this, &MainWindow::open);
    //! [5]

    hdmiAct = new QAction(tr("Open from HDMI"), this);
    hdmiAct->setShortcuts(QKeySequence::Save);
    hdmiAct->setStatusTip(tr("Open video from HDMI device"));
    connect(hdmiAct, &QAction::triggered, this, &MainWindow::save);

    printAct = new QAction(tr("Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    printAct->setDisabled(true);
    connect(printAct, &QAction::triggered, this, &MainWindow::print);

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    //    undoAct = new QAction(tr("&Undo"), this);
    //    undoAct->setShortcuts(QKeySequence::Undo);
    //    undoAct->setStatusTip(tr("Undo the last operation"));
    //    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    //    redoAct = new QAction(tr("&Redo"), this);
    //    redoAct->setShortcuts(QKeySequence::Redo);
    //    redoAct->setStatusTip(tr("Redo the last operation"));
    //    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    //    cutAct = new QAction(tr("Cu&t"), this);
    //    cutAct->setShortcuts(QKeySequence::Cut);
    //    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
    //                            "clipboard"));
    //    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    //    copyAct = new QAction(tr("&Copy"), this);
    //    copyAct->setShortcuts(QKeySequence::Copy);
    //    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
    //                             "clipboard"));
    //    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    //    pasteAct = new QAction(tr("&Paste"), this);
    //    pasteAct->setShortcuts(QKeySequence::Paste);
    //    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
    //                              "selection"));
    //    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    //    boldAct = new QAction(tr("&Bold"), this);
    //    boldAct->setCheckable(true);
    //    boldAct->setShortcut(QKeySequence::Bold);
    //    boldAct->setStatusTip(tr("Make the text bold"));
    //    connect(boldAct, &QAction::triggered, this, &MainWindow::bold);

    //    QFont boldFont = boldAct->font();
    //    boldFont.setBold(true);
    //    boldAct->setFont(boldFont);

    //    italicAct = new QAction(tr("&Italic"), this);
    //    italicAct->setCheckable(true);
    //    italicAct->setShortcut(QKeySequence::Italic);
    //    italicAct->setStatusTip(tr("Make the text italic"));
    //    connect(italicAct, &QAction::triggered, this, &MainWindow::italic);

    //    QFont italicFont = italicAct->font();
    //    italicFont.setItalic(true);
    //    italicAct->setFont(italicFont);

    //    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    //    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
    //                                       "paragraph"));
    //    connect(setLineSpacingAct, &QAction::triggered, this, &MainWindow::setLineSpacing);

    //    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    //    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    //    connect(setParagraphSpacingAct, &QAction::triggered,
    //            this, &MainWindow::setParagraphSpacing);

    //    aboutAct = new QAction(tr("&About"), this);
    //    aboutAct->setStatusTip(tr("Show the application's About box"));
    //    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    //    aboutQtAct = new QAction(tr("About &Qt"), this);
    //    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    //    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    //    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);

    //    leftAlignAct = new QAction(tr("&Left Align"), this);
    //    leftAlignAct->setCheckable(true);
    //    leftAlignAct->setShortcut(tr("Ctrl+L"));
    //    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    //    connect(leftAlignAct, &QAction::triggered, this, &MainWindow::leftAlign);

    //    rightAlignAct = new QAction(tr("&Right Align"), this);
    //    rightAlignAct->setCheckable(true);
    //    rightAlignAct->setShortcut(tr("Ctrl+R"));
    //    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    //    connect(rightAlignAct, &QAction::triggered, this, &MainWindow::rightAlign);

    //    justifyAct = new QAction(tr("&Justify"), this);
    //    justifyAct->setCheckable(true);
    //    justifyAct->setShortcut(tr("Ctrl+J"));
    //    justifyAct->setStatusTip(tr("Justify the selected text"));
    //    connect(justifyAct, &QAction::triggered, this, &MainWindow::justify);

    //    centerAct = new QAction(tr("&Center"), this);
    //    centerAct->setCheckable(true);
    //    centerAct->setShortcut(tr("Ctrl+E"));
    //    centerAct->setStatusTip(tr("Center the selected text"));
    //    connect(centerAct, &QAction::triggered, this, &MainWindow::center);

    setLangEn = new QAction(tr("English"), this);
    setLangEn->setCheckable(true);
    setLangEn->setStatusTip(tr("Set english language"));
    setLangEn->setChecked(true);
    connect(setLangEn, &QAction::triggered, this, &MainWindow::setEnglish);

    setLangUkr = new QAction(tr("Ukrainian"), this);
    setLangUkr->setCheckable(true);
    setLangUkr->setStatusTip(tr("Set ukrainian language"));
    connect(setLangUkr, &QAction::triggered, this, &MainWindow::setUkraine);

    setLangRu = new QAction(tr("Russian"), this);
    setLangRu->setCheckable(true);
    setLangRu->setStatusTip(tr("Set russian language"));
    connect(setLangRu, &QAction::triggered, this, &MainWindow::setRussia);

    //! [6] //! [7]
    //    alignmentGroup = new QActionGroup(this);
    //    alignmentGroup->addAction(leftAlignAct);
    //    alignmentGroup->addAction(rightAlignAct);
    //    alignmentGroup->addAction(justifyAct);
    //    alignmentGroup->addAction(centerAct);
    //    leftAlignAct->setChecked(true);
    //! [6]
}
//! [7]

//! [8]
void MainWindow::createMenus()
{
    //! [9] //! [10]
    fileMenu = menuBar()->addMenu(tr("Video"));
    fileMenu->addAction(fileAct);
    //! [9]
    fileMenu->addAction(streamAct);
    //! [10]
    fileMenu->addAction(hdmiAct);
    fileMenu->addAction(printAct);
    //! [11]
    fileMenu->addSeparator();
    //! [11]
    fileMenu->addAction(exitAct);

    //    editMenu = menuBar()->addMenu(tr("&Edit"));
    //    editMenu->addAction(undoAct);
    //    editMenu->addAction(redoAct);
    //    editMenu->addSeparator();
    //    editMenu->addAction(cutAct);
    //    editMenu->addAction(copyAct);
    //    editMenu->addAction(pasteAct);
    //    editMenu->addSeparator();

    //    helpMenu = menuBar()->addMenu(tr("&Help"));
    //    helpMenu->addAction(aboutAct);
    //    helpMenu->addAction(aboutQtAct);
    //! [8]
    //!
    langMenu = menuBar()->addMenu(tr("Language"));
    langMenu->addAction(setLangEn);
    langMenu->addAction(setLangUkr);
    langMenu->addAction(setLangRu);

    //! [12]
    //    formatMenu = editMenu->addMenu(tr("&Format"));
    //    formatMenu->addAction(boldAct);
    //    formatMenu->addAction(italicAct);
    //    formatMenu->addSeparator()->setText(tr("Alignment"));
    //    formatMenu->addAction(leftAlignAct);
    //    formatMenu->addAction(rightAlignAct);
    //    formatMenu->addAction(justifyAct);
    //    formatMenu->addAction(centerAct);
    //    formatMenu->addSeparator();
    //    formatMenu->addAction(setLineSpacingAct);
    //    formatMenu->addAction(setParagraphSpacingAct);
}


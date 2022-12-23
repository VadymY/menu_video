#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include "myvideosurface.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QPushButton;
class QSlider;
QT_END_NAMESPACE

class QVideoWidget;
class QMediaPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void index_rate(int idx);
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();
//! [1]

    //! [2]
    void setEnglish();
    void setUkraine();
    void setRussia();
private:
    void createActions();
    void createMenus();
//! [2]

//! [3]
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QMenu *langMenu;
    QActionGroup *alignmentGroup;
    QAction *fileAct;
    QAction *streamAct;
    QAction *hdmiAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
//    QLabel *infoLabel;

    QAction *setLangEn;
    QAction *setLangUkr;
    QAction *setLangRu;

    QVideoWidget *m_videoWidget;
    QMediaPlayer *m_player;
    QMediaPlayer *m_2player;

    QTranslator *translator;
    QPushButton * mute_bt;
    QPushButton * duration_bt;
    QSlider *m_slider;
    QSlider *m_volume;

    QPushButton * play_bt;
    QPushButton * pause_bt;
    QPushButton * stop_bt;
    QPushButton * fr_get;
    MyVideoSurface * mysurf;

    qint64 cur_position;
    qint64 duration;
    QString duration_str;
    int percent_play;

    void retranslateUi(QMainWindow *MainWindow);

    QString getTime(qint64 duration);

private:
    QString m_select_str = QString("font: bold");
    QString m_normal_str = QString("");
    bool m_mute = false;
    int m_height = 30;
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioInput>
#include <QAudioDevice>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class AudioProcessor;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_inputDevice_changed(int index);
    void updateTunerDisplay(const QString &note, double cents);

private:
    Ui::MainWindow *ui;
    QAudioInput *audioInput;
    AudioProcessor *processor;
    QComboBox *deviceCombo;
};

#endif // MAINWINDOW_H
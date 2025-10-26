#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audioprocessor.h"
#include <QAudioDevice>
#include <QAudioInput>
#include <QMediaDevices>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    deviceCombo = ui->deviceCombo; // Connect to QComboBox in UI
    processor = new AudioProcessor(this);

    // Populate audio input devices
    for (const auto &device : QMediaDevices::audioInputs()) {
        deviceCombo->addItem(device.description(), QVariant::fromValue(device));
    }

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);
    connect(deviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_inputDevice_changed);
    connect(processor, &AudioProcessor::noteDetected, this, &MainWindow::updateTunerDisplay);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_startButton_clicked() {
    if (audioInput) {
        audioInput->stop();
        delete audioInput;
        audioInput = nullptr;
        ui->startButton->setText("Start");
    } else {
        QAudioDevice device = deviceCombo->currentData().value<QAudioDevice>();
        QAudioFormat format;
        format.setSampleRate(44100);
        format.setChannelCount(1);
        format.setSampleFormat(QAudioFormat::Int16);

        audioInput = new QAudioInput(device, format, this);
        processor->start(audioInput);
        ui->startButton->setText("Stop");
    }
}

void MainWindow::on_inputDevice_changed(int index) {
    if (audioInput) {
        audioInput->stop();
        delete audioInput;
        audioInput = nullptr;
        ui->startButton->setText("Start");
    }
}

void MainWindow::updateTunerDisplay(const QString &note, double cents) {
    ui->noteLabel->setText(note);
    ui->tuningBar->setValue(static_cast<int>(cents));
}
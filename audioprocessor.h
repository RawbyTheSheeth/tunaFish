#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>
#include <QAudioInput>

class AudioProcessor : public QObject {
    Q_OBJECT

public:
    explicit AudioProcessor(QObject *parent = nullptr);
    void start(QAudioInput *input);

signals:
    void noteDetected(const QString &note, double cents);

private:
    void processAudio(const QByteArray &data);
    QString frequencyToNote(double frequency);
    double calculateCents(double frequency, double targetFrequency);
};

#endif // AUDIOPROCESSOR_H
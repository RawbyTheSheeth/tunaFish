#include "audioprocessor.h"
#include <QIODevice>
#include <QDebug>
#include <cmath>

// Simple FFT placeholder (replace with FFTW or similar for production)
struct FFT {
    static double getDominantFrequency(const QByteArray &data, int sampleRate) {
        // Placeholder: Analyze raw audio data (16-bit PCM)
        // For now, return a dummy frequency
        return 440.0; // A4 for testing
    }
};

AudioProcessor::AudioProcessor(QObject *parent) : QObject(parent) {}

void AudioProcessor::start(QAudioInput *input) {
    QIODevice *device = input->start();
    connect(device, &QIODevice::readyRead, this, [this, device]() {
        QByteArray data = device->readAll();
        processAudio(data);
    });
}

void AudioProcessor::processAudio(const QByteArray &data) {
    double frequency = FFT::getDominantFrequency(data, 44100);
    QString note = frequencyToNote(frequency);
    double cents = calculateCents(frequency, 440.0); // A4 as reference
    emit noteDetected(note, cents);
}

QString AudioProcessor::frequencyToNote(double frequency) {
    // Convert frequency to MIDI note number
    double midiNote = 69.0 + 12.0 * std::log2(frequency / 440.0);
    int noteIndex = static_cast<int>(std::round(midiNote)) % 12;
    static const QStringList notes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int octave = static_cast<int>(std::floor(midiNote / 12.0)) - 1;
    return notes[noteIndex] + QString::number(octave);
}

double AudioProcessor::calculateCents(double frequency, double targetFrequency) {
    return 1200.0 * std::log2(frequency / targetFrequency);
}
#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H

#include <QString>

class SettingsStore {
public:
    static SettingsStore& getInstance();
    SettingsStore(const SettingsStore&) = delete;
    SettingsStore& operator=(const SettingsStore&) = delete;

    int checkerBoardWidth = 10;
    int checkerBoardLength = 10;
    QString cameraModel = "Standard";
    int squareSize = 20;

private:
    SettingsStore();
};

#endif // SETTINGSSTORE_H

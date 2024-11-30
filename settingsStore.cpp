#include "settingsStore.h"

SettingsStore& SettingsStore::getInstance() {
    static SettingsStore instance;
    return instance;
}

SettingsStore::SettingsStore() {}


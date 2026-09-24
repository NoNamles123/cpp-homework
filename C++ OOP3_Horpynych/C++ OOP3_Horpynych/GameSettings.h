#pragma once

class GameSettings
{
private:
    int m_nVolume;
    int m_nWidth;
    int m_nHeight;
    char m_difficulty[20];
    char m_language[20];

    GameSettings();
    GameSettings(const GameSettings&) = delete;
    GameSettings& operator=(const GameSettings&) = delete;

public:
    static GameSettings& getInstance();

    void setVolume(int volume);
    void setDifficulty(const char* difficulty);
    void setResolution(int width, int height);
    void setLanguage(const char* language);
    void print();
};

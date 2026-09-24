#include "GameSettings.h"
#include <iostream>
#include <cstring>

using namespace std;

GameSettings::GameSettings()
    : m_nVolume{ 100 }, m_nWidth{ 1920 }, m_nHeight{ 1080 }
{
    strcpy_s(m_difficulty, 20, "Normal");
    strcpy_s(m_language, 20, "English");
}

GameSettings& GameSettings::getInstance()
{
    static GameSettings instance;
    return instance;
}

void GameSettings::setVolume(int volume)
{
    m_nVolume = volume;
}

void GameSettings::setDifficulty(const char* difficulty)
{
    strcpy_s(m_difficulty, 20, difficulty);
}

void GameSettings::setResolution(int width, int height)
{
    m_nWidth = width;
    m_nHeight = height;
}

void GameSettings::setLanguage(const char* language)
{
    strcpy_s(m_language, 20, language);
}

void GameSettings::print()
{
    cout << "Volume: " << m_nVolume << endl;
    cout << "Difficulty: " << m_difficulty << endl;
    cout << "Resolution: " << m_nWidth << "x" << m_nHeight << endl;
    cout << "Language: " << m_language << endl;
}

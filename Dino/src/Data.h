#pragma once

// Data
struct Data
{
    struct HighScoreEntry
    {
        uint16_t score;
        char name[3];
    };
    HighScoreEntry highscores[3];

    static constexpr uint16_t saveDataLocation = 272;

    void save()
    {
        EEPROM.put(saveDataLocation, highscores);
    }

    void load()
    {
        EEPROM.get(saveDataLocation, highscores);
    }

    void clear()
    {
        auto data2 = reinterpret_cast<uint8_t *>(&highscores);
        for(size_t index = 0; index < sizeof(highscores); ++index)
            data2[index] = 0;
        
        save();
    }
};
Data data;

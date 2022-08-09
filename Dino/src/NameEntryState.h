#pragma once

// NameEntryState
struct NameEntryState
{
    uint8_t nameCursor;
    uint8_t letter[3];

    void update()
    {
        updateNameCursor();
        
        if(arduboy.justPressed(A_BUTTON))
        {
            shift();
            data.save();

            this->nameCursor = 0;

            gameState = GameState::HighScore;
        }
    }

    void draw()
    {
        arduboy.setCursor(textToMiddle(14), 2);
        arduboy.print(F("New Highscore!"));

        arduboy.setCursor((textToMiddle(3) + (this->nameCursor * 6)), 14);
        arduboy.print(F("^"));

        Sprites::drawSelfMasked((textToMiddle(3) + (this->nameCursor * 6)), 24, DownArrow, 0);

        arduboy.setCursor(textToMiddle(3), 20);
        arduboy.print(F("___"));

        arduboy.setCursor(textToMiddle(3), 18);
        for (size_t index = 0; index < 3; index++)
            arduboy.print(alphabet[this->letter[index]]);

        arduboy.setCursor(textToMiddle(6 + countDigits(score)), 35);
        arduboy.print(F("Score:"));
        arduboy.print(score);

        arduboy.setCursor(textToMiddle(12), 45);
        arduboy.print(F("A:Enter Name"));
    }

    void shift()
    {
        for(size_t index = 0; index < 3; ++index)
        {
            if(score > data.highscores[index].score)
            {
                for(size_t nextIndex = 2; nextIndex > index; --nextIndex)
                    data.highscores[nextIndex] = data.highscores[nextIndex - 1];

                data.highscores[index].score = score;

                for(size_t letterIndex = 0; letterIndex < 3; ++letterIndex)
                    data.highscores[index].name[letterIndex] = alphabet[this->letter[letterIndex]];
                
                break;
            }
        }
    }

    void updateNameCursor()
    {
        if(arduboy.justPressed(RIGHT_BUTTON) && this->nameCursor < 2)
            this->nameCursor += 1;
        else if(arduboy.justPressed(LEFT_BUTTON) && this->nameCursor > 0)
            this->nameCursor -= 1;

        if(arduboy.justPressed(DOWN_BUTTON) && this->letter[this->nameCursor] < 26)
            this->letter[this->nameCursor] += 1;
        else if(arduboy.justPressed(UP_BUTTON) && this->letter[this->nameCursor] > 0)
            this->letter[this->nameCursor] -= 1;
    }
};
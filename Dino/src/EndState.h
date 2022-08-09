#pragma once

// EndState
struct EndState
{
    void update()
    {
        if(score > data.highscores[2].score && !dino.autoJump)
            gameState = GameState::NameEntry;
        

        if(arduboy.justPressed(B_BUTTON))
            gameState = GameState::HighScore;
        else if(arduboy.justPressed(A_BUTTON))
            gameState = GameState::Menu;
    }

    void draw()
    {
        arduboy.drawLine(0, 20, Arduboy2::width(), 20);
        Sprites::drawSelfMasked((Arduboy2::width() - dinoDuckWidth) / 2, 12, dinoDuckImg, 2);
        Sprites::drawSelfMasked(20, (21 - cactusHeight), cactusImg, 0);

        arduboy.setCursor( textToMiddle(10), 22 );
        arduboy.print(F("GAME OVER!"));

        setCursorForScore(8, 32, score);

        arduboy.print(F("Score:"));
        arduboy.print(score);

        arduboy.setCursor( textToMiddle(9), 42 );
        arduboy.print(F("A:Restart"));

        arduboy.setCursor( textToMiddle(12), 52 );
        arduboy.print(F("B:Highscores"));
    }
};
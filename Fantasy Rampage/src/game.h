#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "card_info.h"

#define DECK_SIZE                       48

#define HAND_SIZE                       5
#define IN_PLAY_SIZE                    2
#define MAX_ROUNDS                      24

#define PLAYER_P                        0  //Player 1, or Player (Human)
#define PLAYER_C                        1  //Player 2, or Computer

#define GAME_MODE_BASIC                 0
#define GAME_MODE_ADVANCED              1

#define PLAYER_MODE_1_PLAYER            0 //1 Player (Human/Computer)
#define PLAYER_MODE_2_PLAYER            1 //2 Player (Human/Human)

#define IMG_BONUS                       0
#define IMG_WAIT                        1
#define IMG_MINUS_1                     2
#define IMG_MINUS_2                     3

#define GAME_START_ROUND                0
#define GAME_SHOW_START_ROUND           1
#define GAME_SHOW_DRAW_CARD             2
#define GAME_SHOW_PLAYER_1_HAND         3
#define GAME_SHOW_PLAYER_2_HAND         4
#define GAME_START_PLAYER_1_HAND        5
#define GAME_START_PLAYER_2_HAND        6
#define GAME_COMPUTER_PLAY_HAND         7
#define GAME_SHOW_COMPUTER_CARD_PLAYED  8
#define GAME_SHOW_2_PLAYER_CARD_PLAYED  9
#define GAME_DETERMINE_ROUND_WINNER     10
#define GAME_SHOW_CARDS_IN_PLAY         11
#define GAME_SHOW_WINNER                12
#define GAME_SHOW_PLAYER_WAIT           13
#define GAME_SHOW_REALLY_QUIT           14

char deck[DECK_SIZE];
char player_p_hand[HAND_SIZE];
char player_c_hand[HAND_SIZE];
char in_play[IN_PLAY_SIZE];
char drawn[IN_PLAY_SIZE];
char round_winner[MAX_ROUNDS];

char player_p_hand_size = HAND_SIZE;
char player_c_hand_size = HAND_SIZE;

char deck_set_up = false;
char deck_ptr = 0;
bool card_in_play = false;
bool deck_drawn = false;

char last_winner = PLAYER_P;
char round_count = 0;
char PLAYER_P_score = 0;
char PLAYER_C_score = 0;
char curr_player = PLAYER_P;  //current player (2 player mode)

char hand_ptr = 0;

char wait_ctr = 0;
char wait_loop = 0;

char game_mode = GAME_MODE_BASIC;
char player_mode = PLAYER_MODE_1_PLAYER;
char disp_state = GAME_SHOW_DRAW_CARD;
char prev_disp_state = disp_state;
char current_suit = SUIT_EARTH;

boolean quitYesNo = false ;

//------------------------

MapInfo get_map_info(char info) {
  int index = (info * 2);
  MapInfo ret;
  ret.x = pgm_read_byte_near(map_info_data + index);
  ret.y = pgm_read_byte_near(map_info_data + (index + 1));
  return ret;
}

CardInfo get_card_info ( char card) {
  int index = (card * 4);
  CardInfo ret;

  if (card > -1) {
    ret.suit = pgm_read_byte_near(card_info_data + index);
    ret.power = pgm_read_byte_near(card_info_data + (index + 1));
    ret.img = pgm_read_byte_near(card_info_data + (index + 2));
    ret.ability = pgm_read_byte_near(card_info_data + (index + 3));
  } else {
    ret.suit = 0;
    ret.power = 0;
    ret.img = 0;
    ret.ability = 0;
  }
  return ret;
}

void setup_deck () {
  memset(deck, 0, sizeof(deck));
  for ( char i = 0; i < DECK_SIZE; i++) {
    deck[i] = i;
  }
}

void shuffle_deck() {
  for ( char a = DECK_SIZE - 1; a > 0; a--)
  {
    char r = random(a + 1);
    if (r != a)
    {
      char temp = deck[a];
      deck[a] = deck[r];
      deck[r] = temp;
    }
  }
}

char draw_from_deck( char *hand) {
  //find first empty space in hand
  char empty_spot_ptr = 0;
  for (char i = 0; i < HAND_SIZE; i++) {
    if (hand[i] == -1) {
      empty_spot_ptr = i;
      break;
    }
  }
  char draw = deck[deck_ptr];
  hand[empty_spot_ptr] = draw;
  deck_ptr++;
  if (deck_ptr == DECK_SIZE) deck_drawn = true;

  return draw;
}

void sort_hand(char *hand) {
  for (char i = 0; i < (HAND_SIZE - 1); i++) {
    for (char o = 0; o < (HAND_SIZE - (i + 1)); o++) {
      CardInfo currCard  = get_card_info(hand[o]);
      CardInfo nextCard  = get_card_info(hand[o + 1]);

      //in advanced game mode, if the current suit == the card's
      //suit it gets a +1 power boost
      if (game_mode == GAME_MODE_ADVANCED) {
        if (currCard.suit == current_suit) currCard.power++;
        if (nextCard.suit == current_suit) nextCard.power++;
      }

      if (currCard.power < nextCard.power) {
        char t = hand[o];
        hand[o] = hand[o + 1];
        hand[o + 1] = t;
      }
    }
  }
}

char get_hand_size(char *hand) {
  char hand_size = HAND_SIZE;
  for (char i = 0; i < (HAND_SIZE); i++) {
    if (hand[i] == -1) hand_size--;
  }
  return hand_size;
}

void display_card (char x, char y, char card, char modifier) {
  char curr_modifier = 0;

  CardInfo inf = get_card_info(card);

  if (game_mode == GAME_MODE_ADVANCED) {
    if (inf.suit == current_suit) curr_modifier = 1;
    curr_modifier += modifier;

    switch (curr_modifier) {
      case 0:
        break;

      case 1:
        sprites.drawOverwrite(x + 49, y - 7, cardBonus, IMG_BONUS);
        break;

      case -1:
        sprites.drawOverwrite(x + 49, y - 7, cardBonus, IMG_MINUS_1);
        break;

      case -2:
        sprites.drawOverwrite(x + 49, y - 7, cardBonus, IMG_MINUS_2);
        break;
    }
  }

  byte j = 0;
  byte k = 0;
  while ((j + (k * 7)) < 42)
  {
    if (j > 6)
    {
      j = 0;
      k++;
    }
    sprites.drawPlusMask(x + (8 * j), y + (8 * k), cardBorders_plus_mask, pgm_read_byte(&cardShape[j + (k * 7)]));
    j++;
  }
  sprites.drawOverwrite(x + 4, y + 4, cardPower, inf.power - 4);
  sprites.drawOverwrite(x + 44, y + 4, cardElements, inf.suit);
  sprites.drawOverwrite(x + 12, y + 16, monsters, inf.img);

  if (game_mode == GAME_MODE_ADVANCED) {
    if (inf.ability > -1) {
      sprites.drawOverwrite(x + 20, y + 7, cardAbility, inf.ability);
      if ((inf.ability == ABILITY_MAGIC) || (inf.ability == ABILITY_HUNT)) {
        sprites.drawOverwrite(x + 29, y + 8, cardBonus, IMG_MINUS_2);
      } else {
        sprites.drawOverwrite(x + 29, y + 8, cardBonus, IMG_MINUS_1);
      }
    }
  }

}

void display_card_info(char x, char y, char card) {
  CardInfo inf = get_card_info(card);

  byte j = 0;
  byte k = 0;
  while ((j + (k * 9)) < 54)
  {
    if (j > 8)
    {
      j = 0;
      k++;
    }
    sprites.drawOverwrite(x + (8 * j), y + (8 * k), cardInfo, pgm_read_byte(&cardInfoShape[j + (k * 9)]));
    j++;
  }

  print_progmem(x + 10, y + 5, text_element);
  sprites.drawOverwrite(x + 54, y + 3, cardElements, inf.suit);
  print_progmem(x + 10, y + 13, text_power);

  //draw bonus, if it is active on the card

  if (inf.suit == current_suit) {
    print_number(x + 48, y + 13, inf.power);
    sprites.drawSelfMasked(x + 55, y + 13, cardBonus, IMG_BONUS);
  }
  else {
    print_number(x + 56, y + 13, inf.power);
  }

  //if the card has an ability, draw the appropriate ability info

  if (inf.ability > -1) {
    switch (inf.ability) {
      case ABILITY_TOXIC:
        print_progmem(x + 14, y + 21, text_toxic);
        break;
      case ABILITY_BURN:
        print_progmem(x + 14, y + 21, text_burn);
        break;
      case ABILITY_SWOOP:
        print_progmem(x + 14, y + 21, text_swoop);
        break;
      case ABILITY_SPEAR:
        print_progmem(x + 14, y + 21, text_spear);
        break;
      case ABILITY_MAGIC:
        print_progmem(x + 14, y + 21, text_magic);
        break;
      case ABILITY_HUNT:
        print_progmem(x + 14, y + 21, text_hunt);
        break;
    }
    sprites.drawSelfMasked(x + 43, y + 20, cardAbility, inf.ability);

    if ((inf.ability == ABILITY_MAGIC) || (inf.ability == ABILITY_HUNT)) {
      sprites.drawSelfMasked(x + 52, y + 21, cardBonus, IMG_MINUS_2);
    } else {
      sprites.drawSelfMasked(x + 52, y + 21, cardBonus, IMG_MINUS_1);
    }


    //draw elements with vulnurabilty to the ability
    print_progmem(x + 19, y + 29, text_card);
    byte k = 0;
    for (char j = 0; j < 4; j++) {
      if (j != inf.suit) {
        sprites.drawSelfMasked(x + 20 + (12 * k), y + 36, cardElements, j);
        k++;
      }
    }
  }
}

void startPlayRound() {
  round_count++;
  if (round_count <= MAX_ROUNDS) {
    //empty out in play cards
    card_in_play = false;
    memset(in_play, -1, sizeof(in_play));
    drawn[PLAYER_P] = -1;
    drawn[PLAYER_C] = -1;

    //players draw cards
    if (!deck_drawn) drawn[PLAYER_P] = draw_from_deck(player_p_hand);
    if (!deck_drawn) drawn[PLAYER_C] = draw_from_deck(player_c_hand);

    disp_state = GAME_SHOW_START_ROUND;
  } else {
    disp_state = GAME_SHOW_WINNER;
    songPlayed = false;
  }
}

void computer_play_hand() {
  sort_hand(player_c_hand);
  player_c_hand_size = get_hand_size(player_c_hand);

  //if no cards are in play
  if (!card_in_play) {
    //play highest rank card in the hand
    in_play[PLAYER_C] = player_c_hand[0];
    player_c_hand[0] = -1;
  } else {
    //otherwise
    //compare card in play to cards in hand
    char card_in_play = in_play[PLAYER_P];
    CardInfo currCard  = get_card_info(card_in_play);

    //in advanced game mode, if the current suit == the card's
    //suit it gets a +1 power boost
    if (game_mode == GAME_MODE_ADVANCED) {
      if (currCard.suit == current_suit) currCard.power++;
    }

    //start with lowest rank card by default
    char card_to_play = player_c_hand_size - 1;
    //play lowest rank card that has greater power than what is in play
    for (char i = player_c_hand_size - 1; i >= 0; i--) {
      //cannot play an empty slot
      if (player_c_hand[i] > -1) {
        CardInfo thisCard = get_card_info(player_c_hand[i]);

        if (game_mode == GAME_MODE_ADVANCED) {
          if (thisCard.suit == current_suit) thisCard.power++;

          //look to see what ability modifiers are present on the other card and apply
          if ((currCard.ability > -1) && (currCard.suit != thisCard.suit)) {
            if ((currCard.ability == ABILITY_MAGIC) || (currCard.ability == ABILITY_HUNT)) {
              thisCard.power -= 2;
            } else {
              thisCard.power --;
            }
          }

        }

        if (thisCard.power > currCard.power) {
          card_to_play = i;
          break;
        }
      } else {
        card_to_play--;
      }
    }

    in_play[PLAYER_C] = player_c_hand[card_to_play];
    player_c_hand[card_to_play] = -1;
  }
  if (last_winner == PLAYER_C) {
    disp_state = GAME_SHOW_COMPUTER_CARD_PLAYED;
  } else {
    disp_state = GAME_DETERMINE_ROUND_WINNER;
  }
}

void determine_round_winner() {
  CardInfo pCard  = get_card_info(in_play[PLAYER_P]);
  CardInfo cCard  = get_card_info(in_play[PLAYER_C]);

  //in advanced game mode, if the current suit == the card's
  //suit it gets a +1 power boost

  //If card(s) have abilities it will also affect their power
  if (game_mode == GAME_MODE_ADVANCED) {
    if (pCard.suit == current_suit) pCard.power++;
    //look at the opponent's card to see if it has an ability (if of a different suit)
    if ((cCard.ability > -1) && (cCard.suit != pCard.suit)) {
      if ((cCard.ability == ABILITY_MAGIC) || (cCard.ability == ABILITY_HUNT)) {
        pCard.power -= 2;
      } else {
        pCard.power --;
      }
    }

    if (cCard.suit == current_suit) cCard.power++;
    //look at the opponent's card to see if it has an ability (if of a different suit)
    if ((pCard.ability > -1) && (cCard.suit != pCard.suit)) {
      if ((pCard.ability == ABILITY_MAGIC) || (pCard.ability == ABILITY_HUNT)) {
        cCard.power -= 2;
      } else {
        cCard.power --;
      }
    }
  }

  if (pCard.power == cCard.power) {
    if (last_winner == PLAYER_P) {
      //player still wins
      PLAYER_P_score++;
      round_winner[round_count - 1] = PLAYER_P;
    } else {
      PLAYER_C_score++;
      round_winner[round_count - 1] = PLAYER_C;
    }
  } else {
    if (pCard.power > cCard.power) {
      last_winner = PLAYER_P;
      PLAYER_P_score++;
      round_winner[round_count - 1] = PLAYER_P;
    } else {
      last_winner = PLAYER_C;
      PLAYER_C_score++;
      round_winner[round_count - 1] = PLAYER_C;
    }
  }
  disp_state = GAME_SHOW_CARDS_IN_PLAY;
}

void stateShowPlayerWait() {
  print_progmem(9, 4, text_pass);
  print_progmem(44, 18, text_player);
  print_number(79, 18, curr_player + 1);

  for (char i = 0; i < wait_loop; i++) {
    sprites.drawSelfMasked(16 * i + 36, 44, cardBonus, IMG_WAIT);
  }

  if (wait_loop <= 3) {
    wait_ctr++;
    if (wait_ctr > 15) {
      wait_ctr = 0;
      wait_loop++;
    }

  } else {

    print_progmem(14, 32, text_play);

    if (arduboy.justPressed(A_BUTTON | B_BUTTON))
    {
      if (!deck_drawn) {
        disp_state = GAME_SHOW_DRAW_CARD;
      } else {
        if (curr_player == PLAYER_P) disp_state = GAME_START_PLAYER_1_HAND;
        if (curr_player == PLAYER_C) disp_state = GAME_START_PLAYER_2_HAND;
      }
    }
  }

}

void start_player_wait() {
  wait_ctr = 0;
  wait_loop = 0;
  disp_state = GAME_SHOW_PLAYER_WAIT;
  stateShowPlayerWait();
}

void startGame() {
  memset(round_winner, -1, sizeof(round_winner));
  deck_ptr = 0;
  setup_deck();
  shuffle_deck();
  if (game_mode == GAME_MODE_ADVANCED) current_suit = random(4);

  drawn[PLAYER_P] = -1;
  drawn[PLAYER_C] = -1;
  round_count = 0;

  hand_ptr = 0;

  last_winner = PLAYER_P;
  PLAYER_P_score = 0;
  PLAYER_C_score = 0;

  card_in_play = false;
  deck_drawn = false;

  player_p_hand_size = HAND_SIZE;
  player_c_hand_size = HAND_SIZE;

  //empty hand
  memset(player_p_hand, -1, sizeof(player_p_hand));
  memset(player_c_hand, -1, sizeof(player_c_hand));

  memset(in_play, -1, sizeof(in_play));

  //draw initial cards
  for (char i = 0; i < 4; i++) {
    draw_from_deck(player_p_hand);
    draw_from_deck(player_c_hand);
  }

  startPlayRound();
}

void showScore()
{
  if (player_mode == PLAYER_MODE_1_PLAYER) {
    print_progmem(0, 0, text_player);
    print_progmem(73, 0, text_computer);
  }
  else {
    print_progmem(0, 0, text_player_1);
    print_progmem(73, 0, text_player_2);

  }
  print_score(46, 0, PLAYER_P_score);
  print_score(119, 0, PLAYER_C_score);
}

//------------------------
void stateMenuPlay()
{
  gameState = STATE_GAME_SELECT_GAME_MODE;
  // TODO: ATM.stop();
};

void stateGameSelectPlayerMode() {
  sprites.drawSelfMasked(0, 4, titleName, 0);
  print_progmem(19, 24, text_select_player_mode);
  print_progmem(32, 36, text_1_player);
  print_progmem(32, 48, text_2_player);

  if (arduboy.justPressed(UP_BUTTON)) player_mode = PLAYER_MODE_1_PLAYER;
  if (arduboy.justPressed(DOWN_BUTTON)) player_mode = PLAYER_MODE_2_PLAYER;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    startGame();
    gameState = STATE_GAME_PLAYING;
  }
  print_progmem(26, 36 + (player_mode * 12), text_pointer);
}

void stateGameSelectGameMode() {
  sprites.drawSelfMasked(0, 4, titleName, 0);
  print_progmem(24, 24, text_select_game_mode);
  print_progmem(32, 36, text_basic);
  print_progmem(32, 48, text_advanced);

  if (arduboy.justPressed(UP_BUTTON)) game_mode = GAME_MODE_BASIC;
  if (arduboy.justPressed(DOWN_BUTTON)) game_mode = GAME_MODE_ADVANCED;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    stateGameSelectPlayerMode();
    gameState = STATE_GAME_SELECT_PLAYER_MODE;
  }
  print_progmem(26, 36 + (game_mode * 12), text_pointer);
}

void stateShowStartRound() {

  showScore();

  if (game_mode == GAME_MODE_ADVANCED) {
    print_progmem(32, 8, text_start_round);
    sprites.drawOverwrite(0, 16, mapView, 0);

    print_progmem(68, 18, text_boost);
    print_progmem(68, 26, text_element);
    sprites.drawOverwrite(120, 21, cardElements, current_suit);

    print_progmem(68, 40, text_area);
    if (player_mode == PLAYER_MODE_1_PLAYER) {
      print_progmem(68, 48, text_player);
      sprites.drawSelfMasked(123, 48, mapMarker, blinking);
      print_progmem(68, 56, text_computer);
      sprites.drawSelfMasked(123, 56, mapMarker, 2 + blinking);
    }
    else {
      print_progmem(68, 48, text_player_1);
      sprites.drawSelfMasked(118, 48, mapMarker, blinking);
      print_progmem(68, 56, text_player_2);
      sprites.drawSelfMasked(118, 56, mapMarker, 2 + blinking);
    }


    for (char i = 0; i < MAX_ROUNDS; i++) {
      if (round_winner[i] > -1) {
        MapInfo curr = get_map_info(i);
        sprites.drawSelfMasked(curr.x + 0, curr.y + 16, mapMarker, (2 * round_winner[i]) + blinking);
      }
    }

  }
  else print_progmem(32, 29, text_start_round);
  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    //if 1 player mode, then go on to draw
    if (player_mode == PLAYER_MODE_1_PLAYER) {
      if (!deck_drawn) {
        disp_state = GAME_SHOW_DRAW_CARD;
      } else {
        //use last_winner to detremine who can play first:
        //if last_winner was computer (PLAYER_C) then
        //call computer_play_hand
        if (last_winner == PLAYER_C) {
          computer_play_hand();
        } else {
          disp_state = GAME_START_PLAYER_1_HAND;
        }
      }
    } else {
      //if 2 player mode start the player wait
      curr_player = last_winner;
      start_player_wait();
    }
  }
}

void stateShowDrawCard() {
  print_progmem(22, 0, text_your_draw);
  if (game_mode == GAME_MODE_ADVANCED) {
    display_card (0, 16, drawn[curr_player], 0);
    display_card_info (61, 16, drawn[curr_player]);
  } else {
    display_card (36, 16, drawn[curr_player], 0);
  }
  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {

    if (player_mode == PLAYER_MODE_1_PLAYER) {
      //use last_winner to detremine who can play first:
      //if last_winner was computer (PLAYER_C) then
      //call computer_play_hand
      if (last_winner == PLAYER_C) {
        computer_play_hand();
      } else {
        disp_state = GAME_START_PLAYER_1_HAND;
      }
    } else {
      //In 2 player mode, PLAYER_C = player 2
      if (card_in_play) {
        disp_state = GAME_SHOW_2_PLAYER_CARD_PLAYED;
      } else {
        if (curr_player == PLAYER_C) {
          disp_state = GAME_START_PLAYER_2_HAND;
        } else {
          disp_state = GAME_START_PLAYER_1_HAND;
        }
      }
    }
  }
}

void stateStartPlayer1Hand () {
  hand_ptr = 0;
  sort_hand(player_p_hand);
  player_p_hand_size = get_hand_size(player_p_hand);
  disp_state = GAME_SHOW_PLAYER_1_HAND;
}

void stateStartPlayer2Hand () {
  hand_ptr = 0;
  sort_hand(player_c_hand);
  player_c_hand_size = get_hand_size(player_c_hand);
  disp_state = GAME_SHOW_PLAYER_2_HAND;
}

void stateShowPlayer1Hand () {
  print_progmem(17, 0, text_pick_card_to_play);
  //print_number (100, 0, player_p_hand_size);

  //draw forward to hand_ptr
  for (char i = 0; i < hand_ptr; i++)
  {
    display_card((i * 16) + 4 + ( 8 * (5 - player_p_hand_size)), 24, player_p_hand[i], 0);
  }

  for (char i = player_p_hand_size - 1; i > (hand_ptr - 1); i--)
  {
    if (i > hand_ptr) display_card((i * 16) + 4 + ( 8 * (5 - player_p_hand_size)), 24, player_p_hand[i], 0);
    else display_card((i * 16) + 4 + (8 * (5 - player_p_hand_size)), 16, player_p_hand[i], 0);
  }

  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(RIGHT_BUTTON) && (hand_ptr < player_p_hand_size)) hand_ptr++;
  if (arduboy.justPressed(LEFT_BUTTON) && (hand_ptr > 0)) hand_ptr--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    in_play[PLAYER_P] = player_p_hand[hand_ptr];
    player_p_hand[hand_ptr] = -1;

    if (player_mode == PLAYER_MODE_1_PLAYER) {
      if (last_winner == PLAYER_C) {
        disp_state = GAME_DETERMINE_ROUND_WINNER;
      } else {
        card_in_play = true;
        disp_state = GAME_COMPUTER_PLAY_HAND;
      }
    } else {
      if (last_winner == PLAYER_C) {
        disp_state = GAME_DETERMINE_ROUND_WINNER;
      } else {
        card_in_play = true;
        curr_player = PLAYER_C;
        start_player_wait();
      }
    }

  }
}

//only used in 2 player mode
void stateShowPlayer2Hand () {
  print_progmem(17, 0, text_pick_card_to_play);
  //print_number (100, 0, player_c_hand_size);

  //draw forward to hand_ptr
  for (char i = 0; i < hand_ptr; i++) {
    display_card((i * 16) + 4 + ( 8 * (5 - player_p_hand_size)), 24, player_c_hand[i], 0);
  }

  for (char i = player_c_hand_size - 1; i > (hand_ptr - 1); i--)
  {
    if (i > hand_ptr)display_card((i * 16) + 4 + ( 8 * (5 - player_p_hand_size)), 24, player_c_hand[i], 0);
    else display_card((i * 16) + 4 + ( 8 * (5 - player_p_hand_size)), 16, player_c_hand[i], 0);
  }

  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(RIGHT_BUTTON) && (hand_ptr < player_c_hand_size)) hand_ptr++;
  if (arduboy.justPressed(LEFT_BUTTON) && (hand_ptr > 0)) hand_ptr--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    in_play[PLAYER_C] = player_c_hand[hand_ptr];
    player_c_hand[hand_ptr] = -1;

    if (last_winner == PLAYER_C) {
      card_in_play = true;
      curr_player = PLAYER_P;
      start_player_wait();
    } else {
      disp_state = GAME_DETERMINE_ROUND_WINNER;
    }

  }
}

void stateShowComputerCardPlayed() {
  print_progmem(26, 0, text_computer);
  print_progmem(72, 0, text_played);
  display_card (36, 16, in_play[PLAYER_C], 0);
  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    disp_state = GAME_START_PLAYER_1_HAND;
  }
}

void stateShow2PlayerCardPlayed() {
  //to do: Determine previous player and show that card
  char prev_player = PLAYER_C;
  if (curr_player == PLAYER_C) prev_player = PLAYER_P;

  print_progmem(26, 0, text_player);
  print_number(61, 0, prev_player + 1);
  print_progmem(72, 0, text_played);

  display_card (36, 16, in_play[prev_player], 0);
  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    if (curr_player == PLAYER_C) {
      disp_state = GAME_START_PLAYER_2_HAND;
    } else {
      disp_state = GAME_START_PLAYER_1_HAND;
    }
  }
}

void stateShowCardsInPlay() {
  CardInfo player_p_card = get_card_info(in_play[PLAYER_P]);
  CardInfo player_c_card = get_card_info(in_play[PLAYER_C]);
  char player_p_mod = 0;
  char player_c_mod = 0;

  showScore();

  //determine modifiers for advanced play
  if (game_mode == GAME_MODE_ADVANCED) {
    if ((player_p_card.ability > -1) && (player_p_card.suit != player_c_card.suit)) {
      if ((player_p_card.ability == ABILITY_MAGIC) || (player_p_card.ability == ABILITY_HUNT)) {
        player_p_mod = -2;
      } else {
        player_p_mod = -1;
      }
    }

    if ((player_c_card.ability > -1) && (player_p_card.suit != player_c_card.suit)) {
      if ((player_c_card.ability == ABILITY_MAGIC) || (player_c_card.ability == ABILITY_HUNT)) {
        player_c_mod = -2;
      } else {
        player_c_mod = -1;
      }
    }
  }

  if (in_play[PLAYER_P] > -1) {
    if (last_winner == PLAYER_P) print_progmem(17, 8, text_win);    // 17 = (64 - 30)/2
    display_card (0, 16, in_play[PLAYER_P], player_c_mod);
  }
  if (in_play[PLAYER_C] > -1) {
    if (last_winner == PLAYER_C) print_progmem(89, 8, text_win);    // 89 = 72 + (64-30)/2
    display_card (72, 16, in_play[PLAYER_C], player_p_mod);
  }
  if (arduboy.pressed(UP_BUTTON)) disp_state = GAME_SHOW_REALLY_QUIT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    if (game_mode == GAME_MODE_ADVANCED) current_suit = random(4);
    disp_state = GAME_START_ROUND;
  }
}

void stateShowWinner() {
  showScore();

  if (PLAYER_P_score > PLAYER_C_score) {
    //player wins
    if (!songPlayed)
    {
      // TODO: ATM.play(victorySong);
      songPlayed = true;
    }
    print_progmem(51, 56, text_win);
    sprites.drawSelfMasked(52, 26, winningBadge, 0);
    if (player_mode == PLAYER_MODE_1_PLAYER) {
      print_progmem(49, 16, text_player);
    } else {
      print_progmem(44, 16, text_player_1);
    }

  }
  else {
    if (PLAYER_C_score > PLAYER_P_score) {
      //computer wins
      print_progmem(51, 56, text_win);
      sprites.drawSelfMasked(52, 26, winningBadge, 1);
      if (player_mode == PLAYER_MODE_1_PLAYER) {
        if (!songPlayed)
        {
          // TODO: ATM.play(lostSong);
          songPlayed = true;
        }
        print_progmem(44, 16, text_computer);
      } else {
        print_progmem(44, 16, text_player_2);
        if (!songPlayed)
        {
          // TODO: ATM.play(victorySong);
          songPlayed = true;
        }
      }

    }
    else {
      //tie
      print_progmem(54, 16, text_tie);
      sprites.drawSelfMasked(26, 26, winningBadge, 0);
      sprites.drawSelfMasked(78, 26, winningBadge, 1);
    }
  }

  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    // TODO: ATM.play(themeSong);
  }
}

void stateShowReallyQuit() {
  print_progmem(32, 8, text_really);
  print_progmem(29, 16, text_quit);
  print_progmem(44, 28, text_yes_no);
  sprites.drawSelfMasked((40 + (!quitYesNo * 26)), 26, selectorTop, 0);
  sprites.drawPlusMask((39 + (!quitYesNo * 26)), 34, selectorMid_plus_mask, 0);

  if (arduboy.pressed(LEFT_BUTTON)) quitYesNo = true;
  else if (arduboy.pressed(RIGHT_BUTTON)) quitYesNo = false;
  else if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    switch (quitYesNo)
    {
      case true:
        gameState = STATE_MENU_MAIN;
        // TODO: ATM.play(themeSong);
        break;
      case false:
        disp_state = prev_disp_state;
        break;
    }
  }
}

void stateGamePlaying()
{
  gameState = STATE_GAME_PLAYING;
  switch (disp_state) {
    case GAME_START_ROUND:
      startPlayRound();
      break;

    case GAME_SHOW_START_ROUND:
      stateShowStartRound();
      break;

    case GAME_SHOW_DRAW_CARD:
      stateShowDrawCard();
      break;

    case GAME_START_PLAYER_1_HAND:
      stateStartPlayer1Hand();
      break;

    case GAME_START_PLAYER_2_HAND:
      stateStartPlayer2Hand();
      break;

    case GAME_SHOW_PLAYER_1_HAND:
      stateShowPlayer1Hand();
      break;

    case GAME_SHOW_PLAYER_2_HAND:
      stateShowPlayer2Hand();
      break;

    case GAME_COMPUTER_PLAY_HAND:
      computer_play_hand();
      break;

    case GAME_SHOW_COMPUTER_CARD_PLAYED:
      stateShowComputerCardPlayed();
      break;

    case GAME_SHOW_2_PLAYER_CARD_PLAYED:
      stateShow2PlayerCardPlayed();
      break;

    case GAME_DETERMINE_ROUND_WINNER:
      determine_round_winner();
      break;

    case GAME_SHOW_CARDS_IN_PLAY:
      stateShowCardsInPlay();
      break;

    case GAME_SHOW_WINNER:
      stateShowWinner();
      break;

    case GAME_SHOW_PLAYER_WAIT:
      stateShowPlayerWait();
      break;

    case GAME_SHOW_REALLY_QUIT:
      stateShowReallyQuit();
      break;
  }
};


//-----------

#endif

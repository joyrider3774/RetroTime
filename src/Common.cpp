#include <SDL.h>
#include <inttypes.h>
#include <string.h>
#include <iostream>
#include "Common.h"

//Modes
ModeDesc GMModes[Modes] = {
    {
        GMRetroCarousel,
        "Retro Carousel",
	    "In Retro Carousel mode you will play all games for 2 minutes after each other\n" 
	    "You can not die by losing lives. Points will be deducted from the score when\n"
	    "dying. The aim is to get the highest score, accumulated over all the games"
    },
    {	
	    GMGame,
	    "Lives Mode",
	    "In Lives mode you play a single game and start with a fixed amount of lives.\n" 
	    "Dying will lose you one live. The game is over if the lives reaches 0.\n"  
	    "The aim of the game is to get the highest score possible without dying."
    },
    {
        GMTimeAttack,
        "Time Attack",
        "In Time Attack mode you play a single game for 5 minutes. You can not die by\n"
	    "losing lives. Points will be deducted from the score when dying.\n" 
	    "The aim is to get the highest score during these 5 minutes of playing."
    }
};

//GameDescriptions
GameDesc GSGames[Games] = {
    {
        GSRamItInit,//GSSpaceInvaders,
        "Invaders",
	    "Invaders is a game based on Space Invaders. Rows of enemies\n"
	    "are closing in you. You have to shoot them down before they\n" 
	    "reach your mining asteroids. Shooting down enemies gains\n"
	    "points, shooting down all enemies gains extra points\n" 
	    "You can move with dpad of left joystick and shoot with (A).\n" 
	    "If enemies reach your mining asteroids you will lose a life.\n" 
	    "Avoid being shot or you will also lose a life.",
	    "a game based on Space Invaders (Arcade)"
    },
	{
	    GSRamItInit, //gsbreakout,
	    "Brick Breaker",
	    "Brick Breaker is a game based on Breakout. Rows of blocks\n" 
	    "have to be destroyed by touching them with the ball.\n" 
	    "You control the  paddle at the bottom of the screen with\n" 
	    "the left joystick or dpad. Keeping (A) pressed speeds up the\n"
	    "paddle. You have to let the ball bounce of your paddle to\n" 
	    "send it back in the playfield. Destroying blocks gains points.\n"
	    "Fail to bounce back the ball and you lose a life",
	    "a game based on Breakout (Arcade)"

    },
    {   
        GSRamItInit,//gsfrog,
	    "Toady",
	    "Toady is a game based on Frogger. You control the robot\n"
	    "by left joystick or dpad and you have to keep moving up.\n" 
	    "Each time you move up to a place you had not reached yet,\n" 
	    "you will gain points. Avoid getting hit by traffic, don't\n" 
	    "fall into water, don't exceed the edges of the playfield\n" 
	    "or you will lose a life. Collect diffrent fruit to gain\n" 
	    "extra score.",
	    "a game based on Frogger (Arcade)"
    },
	{
	    GSSnakeInit,//gssnake,
	    "Snakey",
	    "Snakey is a game based on well snake. You control a snake's\n"
	    "head (red block) by the left joystick or dpad. The snake\n"
	    "can move in four directions and wants food (green blocks).\n" 
	    "Each time you eat food the snake's body will grow. Eat\n" 
	    "food to gain points, the longer your snake is to more\n" 
	    "points you will gain. Touching the snake's body or going\n" 
	    "outside the playfield will lose you a life.",
	    "a game based on Snake (Nokia 3310)",
    },
    {
	    GSRamItInit,//gspang,
	    "Bubble Buster",
	    "Bubble Buster is a game based on Pang. You control a player\n"
	    "that needs to pop bubbles. You can control the player using the left\n"
	    "joystick or dpad and shoot at bubbles using (A) button. If you hit a\n"
	    "bubble it will split up into smaller bubbles and you gain points. Get\n"
	    "hit by a bubble and you will lose a life, making you temporary\n"
	    "invincible but you can not shoot during this time. Clearing all bubbles\n"
	    "gives you extra points.",
	    "a game based on Pang (Arcade)",
    },
    {
	    GSTetrisInit,
	    "Block Stacker",
	    "Block Stacker is a game of tetris. You control the blocks\n"
	    "with the left joystick or dpad. You need to stack them in such a\n"
	    "manner that they form complete rows. Once this is done that row\n"
	    "will be removed. Speed increases gradually and having no place\n"
	    "at the top to place a block will lose you a life. You can rotate\n"
	    "blocks using (A) and quick drop them using (B).",
	    "a game based on Tetris"
    },
    {
	    GSRamItInit,
	    "Color Invasion",
	    "Color Invasion is a game based on Ram-It. You control the\n"
	    "cannon with the left joystick or dpad. You need to shoot the invading\n"
	    "colored bars completely of the screen using (A). Points are awarded when\n"
	    "the color bar is completely of the screen, the closer a bar had gotten\n"
	    "to the cannon the more points you will gain. You also gain extra points\n"
	    "for clearing all bars from the screen. When a colored bar reaches the\n"
	    "cannon rail you will lose a life.",
	    "a game based on Ram-It (Atari 2600)"
    },
    {
	    GSEddyInit,//gseddy,
	    "Faster Dave",
	    "Faster Dave is a game based on Fast Eddie. You control Dave with the\n" 
	    "left joystick or dpad. You need to avoid or jump over enemies with\n" 
	    "the (A) button while collecting the magic balls. When you collect 9\n" 
	    "balls the end level key is reachable. You are safe on ladders. Points\n" 
	    "are received for collecting magic balls and getting the end level\n"
	    "key. Collecting the 10th magic ball is not required. When you touch\n"
	    "an enemy you will a lose life. Every 5 levels, level layout changes.",
	    "a game based on Fast Eddie (Atari 2600)"
    }
};

//PauseMenus
//order must equal order from pmvalues !!!
PauseMenuDesc PMPauseMenus[PauseMenus] = 
{ 
	{PMContinue,    "Continue"},
    {PMQuit,		"Quit"},
	{PMMusicVol,	"Music Volume: "},
	{PMSoundVol,	"Sound Volume: "},
	{PMMotionBlur,	"Sprite Ghosting Effect: "},
	{PMGameHelp,	"Game Help"},
	{PMCrt, 		"Crt Effect: "}
};

//GamePauseMenus
GPGamePauseMenusDesc GPGamePauseMenus[Games] = {
	{GSSpaceInvaders,   {PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}}, 
	{GSBreakout,	    {PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}}, 
	{GSFrog,		    {PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}},
	{GSSnake,			{PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}},
	{GSPang,			{PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}},
	{GSTetris,	  	    {PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}},
	{GSRamIt,			{PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}},
	{GSEddy,			{PMContinue, PMGameHelp, PMMotionBlur, PMCrt, PMMusicVol, PMSoundVol, PMQuit}}
};

//MainMenus
MainMenusDesc MMMainMenus[MainMenus] = 
{
	{MMStart,       "Start Game"},
	{MMHighScores,  "High Scores"},
	{MMOptions,     "Options"},
	{MMHelp,		"Help"},
	{MMCredits,		"Credits"},
	{MMQuit,		"Quit"}
};

//OptionsMenus
OptionMenusDesc OMOptionMenus[OptionMenus] = 
{
	{OMMusicVol,		"Music Volume: "},
	{OMSoundVol,		"Sound Volume: "},
	{OMMotionBlur,      "Sprite Ghosting Effect: "},
	{OMCrt, 			"Crt Effect: "},
	{OMResetHighScores, "Reset Highscores"},
	{OMBack,		    "Back"}
};

//CrtOptions
CrtOptDesc CrtOptions[Crts] =
{ 
	{0, "Off"},
	{1,	"Lines 2"},
	{2,	"Lines 4"},
	{3,	"Lines 8"},
	{4,	"Dots"},
	{5,	"Cross"}
};

Uint32 ColorToUint(int R, int G, int B)
{
	return (Uint32)((R << 16) + (G << 8) + (B << 0));
}

SDL_Color UintToColour(Uint32 colour)
{
	SDL_Color tempcol;
	tempcol.a = 255;
	tempcol.r = (colour >> 16) & 0xFF;
	tempcol.g = (colour >> 8) & 0xFF;
	tempcol.b = colour & 0xFF;
	return tempcol;
}
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cstdlib>
#include "MyVector3.h"
#include <SFML/Audio/SoundBuffer.hpp>
#include<SFML/Audio/Sound.hpp>

/// <summary>
/// main game class
/// </summary>
class Game
{
public:
	Game();
	~Game();
	void run();

private:
	//sfml loop stuff
	void processEvents(); //process events 
	void update(sf::Time t_deltaTime); //update every 1/60th
	void render(); //draw method
	void processMouseEvents(sf::Event t_mouseEvent); //process events
	
	void setupStuff(); //function that initialises a load of stuff
	void checkForCushionTouchin(MyVector3 &t_position, MyVector3 &t_velocity); //check if balls collide with cushion
	void checkForBallTouchin(MyVector3 &t_position, MyVector3 &t_velocity, MyVector3 &t_positionTwo, MyVector3 &t_velocityTwo); //ball collision check
	void noBallMovement(MyVector3 &t_velocity, MyVector3 &t_velocityTwo, MyVector3 &t_velocityThree); //checks if all balls are still
	//function to check for collisions - returns a bool
	bool checkForCollision(MyVector3 &t_position, MyVector3 &t_velocity, MyVector3 &t_positionTwo, MyVector3 &t_velocityTwo); 

	//these functions need little explaining - they check if balls have touched each other
	void checkForBallTouchinAgainstRed(MyVector3 &t_position, MyVector3 &t_velocity, MyVector3 &t_positionTwo, MyVector3 &t_velocityTwo);
	void checkForBallTouchinWhiteYellow(MyVector3 &t_position, MyVector3 &t_velocity, MyVector3 &t_positionTwo, MyVector3 &t_velocityTwo);
	void checkForBallTouchinRedYellow(MyVector3 &t_position, MyVector3 &t_velocity, MyVector3 &t_positionTwo, MyVector3 &t_velocityTwo);
	void checkForBallTouchinYellowWhite(MyVector3 & t_position, MyVector3 & t_velocity, MyVector3 & t_positionTwo, MyVector3 & t_velocityTwo);
	//function that propels ball forward after aim and release of mouse
	void steadyAimFire(MyVector3 &t_position, MyVector3 &t_velocity); //will hopefully push ball given length of aim line

	//tidy code up and put in these functions
	void investigatePocketOccurences(); //pocket checking and outcome outputting function
	void lookoutForCannons(); //cannon detections moved out of update function
	void produceRoundOutcome(); //'when balls stop turning, decide what to do' function
	bool checkForPocketTouchin(MyVector3 &t_position); //this function checks for pocket touching
	MyVector3 pocketDistance; //stores distance between pockets and balls
	
	//no movement going on (as opposed to a whole lotta shakin goin on) 
	//saved it as vcariable to make it easier to say 'no movement'
	MyVector3 noMovement{ 0.0f , .0f , 0.0f };

	const double FRICTION =  0.99f; //slow the ball by this each time
	const double REDUCE_PUSH = 0.06f; //calm the initial velocity of shot - otherwise it goes mental

	//screen size
	const unsigned SCREEN_WIDTH = 800.0f;
	const unsigned SCREEN_HEIGHT = 500.0f;

	//table top variables
	const int BORDER = 20;
	const int PLAYINGFIELD_WIDTH = 760.0f;
	const int PLAYINGFIELD_HEIGHT = 460.0f;

	//radius of balls variable
	const double RADIUS = 15;

	//white ball
	bool inPlayWhite = true; //in play or not
	MyVector3 whiteBallPosition{400.0f, 250.0f, 0.0f}; //starting position of white ball
	MyVector3 whiteBallVelocity{0.0f, 0.0f , 0.0f}; //velocity of the white ball 
	void moveBall(MyVector3 &t_position , MyVector3 &t_velocity); //function that moves white ball

	//red ball
	bool inPlayRed = true; //in play or not
	MyVector3 redBallPosition{ 200.0f, 250.0f, 0.0f }; //starting position
	MyVector3 redBallVelocity{ 0.0f,0.0f,0.0f }; //velocity

	//yellow ball
	bool inPlayYellow = true; ///in play
	MyVector3 yellowBallPosition{ 600.0f, 250.0f, 0.0f }; //starting position
	MyVector3 yellowBallVelocity{ 0.0f,0.0f,0.0f }; //yellow ball velocity

	//distance between balls - collision lookin
	MyVector3 thereIsADistanceBetweenUs; //stores the distance between balls 
	MyVector3 centre{ 15.0f,15.0f,0.0f };//vector storing the center of the ball
	MyVector3 ballCentre = whiteBallPosition + centre; //centre of the ball determined

	//setting up basic stuff
	sf::RenderWindow m_window; // main SFML window
	sf::RectangleShape playingField; //the square in the middle - playing field
	sf::CircleShape ball; //the one sf circle we're allowed use

	//pockets
	const int POCKET_NUM = 6; //amount of pockets
	sf::CircleShape pockets[6]; //pocket array of circles

	//list of positions for pocket placement
	MyVector3 pocketOnePos{ 20.0f, 20.0f, 0.0f };
	MyVector3 pocketTwoPos{ 400.0f, 20.0f, 0.0f };
	MyVector3 pocketThreePos{ 780.0f, 20.0f, 0.0f };
	MyVector3 pocketFourPos{ 20.0f, 480.0f, 0.0f };
	MyVector3 pocketFivePos{ 400.0f, 480.0f, 0.0f };
	MyVector3 pocketSixPos{ 780.0f, 480.0f, 0.0f };

	//aiming stuff?
	MyVector3 ballPositionAim; //position of white ball
	MyVector3 cursorPositionAim; //mouse position
	sf::VertexArray aimline{ sf::Lines }; //thing for aiming - cue
	sf::Vertex lineStart{}; // start point of line
	sf::Vertex lineEnd{}; // end point of line
	MyVector3 aimDeterminedVelocity; //the velocity given by the aim

	//my gold bullion (boolean) storage area
	bool hasntShot = true; //has the player shot yet?
	bool m_exitGame; // control exiting game
	bool yellowsTurn = false; //is it yellow's turn
	bool WhitesTurn = true; //is it white's turn
	bool noMovementBool = false; //are all balls not moving
	bool hitRed = false; //has red been hit
	bool hitYellow = false; //has yellow been hit
	bool hitWhite = false; //has white been hit
	bool cannonScored = false; //did player get a cannon
	bool foul = false; //did player foul?
	bool pottedWhite = false; //is white potted
	bool pottedRed = false; //is red potted
	bool pottedYellow = false; //is yellow potted

	//the scores for each player
	int whiteScore = 0;
	int yellowScore = 0;

	//EXTRAS
	//text
	sf::Font font;
	sf::Text whiteScoreText;
	sf::Text yellowScoreText;
	const int textIndent = 300;
	const int yellowTextIndent = 20;

	//sounds
	sf::SoundBuffer fireBuffer;
	sf::Sound fireSound;

	sf::SoundBuffer dropBuffer;
	sf::Sound dropSound;

	sf::SoundBuffer cushionBuffer;
	sf::Sound cushionSound;
};

#endif // !GAME


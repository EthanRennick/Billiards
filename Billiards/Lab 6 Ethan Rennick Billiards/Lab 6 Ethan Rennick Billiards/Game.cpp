/// <summary>
/// Ethan Rennick
/// C00240102
/// started 23/1/19
/// time estimated: 30 hours
/// time actual: 50
/// ---
/// lab 6 billiards assignment from pete
/// started from scratch
/// Did all of this code on my own, with the exception of Pete's formulas, etc.
/// Very proud of this, went much better than Missile One. Extra functionality not really present, but
/// the base game is, in my opinion, completed which is more than missile one did.
/// Sounds, score display, collisions, cannons, hazards, cushions, ball touchin. It's been a fun project.
/// ---
/// known bugs?
/// Below, I've listed the bugs I am aware of in my code
/// ===
/// BORDER FOUL
/// If you move your mouse and hover over a tiny line between where Leftmost 
/// window border meets the graphics on screen,
/// a bug occurs where a foul instantaneously happens, complete with
/// enemy rewards of points and respawning balls in some cases. No idea
/// what caused this, and no idea how to fix. Only occurs on left side.
/// ===
/// NO MULTIPLE SCORES
/// Due to the way I wrote my detections of scores, it is possible in my version
/// to pot a ball, red for example, and if your cue should happen to be potted also,
/// it will cause a foul. I thought this was maybe justifiable, or perhaps should
/// have caused a hazard and a pot on the same turn? But my code wont allow that, not
/// in it's present state. So you'll get the points for potting the ball, but suffer a foul
/// should your own ball also be potted on that turn.
/// ===
/// WHITE POT YELLOW = FOUL
/// If the white cue pots the yellow ball he will be awarded points,
/// but no amount of rewriting or rearranging code stop it from declaring that
/// he gets a foul when his ball stops moving. Probably a simple fix, but I can't find it
/// Only occurs when white pots yellow, not the other way round.
///		Actually, after classmates attempted to fix, it now gives white a foul,
///		gives the returning yellow a foul, and put it back to white's turn. 
///		=> i attempted to fix and made it worse and am deciding to leave it.
/// 
/// </summary>

#include "Game.h"
#include "MyVector3.h"


/// <summary>
/// default constructor
/// </summary>
Game::Game() :
	m_window {sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32 }, "Billiards, by Ethan Rennick"},
	m_exitGame{ false } //when true, the game will exit
{
	setupStuff();
}

/// <summary>
/// default destructor for game? idk
/// </summary>
Game::~Game()
{
}


/// <summary>
/// game loop that makes the thing go to like 60fps
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event nextEvent;
	while (m_window.pollEvent(nextEvent))
	{
		if (sf::Event::Closed == nextEvent.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == nextEvent.type) //user key press
		{
			if (sf::Keyboard::Escape == nextEvent.key.code) //if i press escape
			{
				m_exitGame = true; //exiting is true
			}
		}

		if (noMovementBool == true) //if ball hasn't moved yet (ie still aiming) then AIM!
		{
			//if (sf::Event::MouseButtonPressed == nextEvent.type) //If mouse is clicked
			//{
			//	//idk
			//}

			if (sf::Event::MouseMoved == nextEvent.type)
			{
				aimline.clear(); //clear previous line
				//std::cout << "noticing mouse movement"; //former movement debug

				//if it is white ball's turn
				if (WhitesTurn == true)
				{
					//process all of this stuff that wouldn't work if it wasnt here
					cursorPositionAim = MyVector3{ static_cast<float>(nextEvent.mouseMove.x), static_cast<float>(nextEvent.mouseMove.y),0.0f }; //finds end point (mouse click)
					lineStart = (sf::Vertex{ whiteBallPosition, sf::Color::Blue });
					aimline.append(lineStart);
					lineEnd = (sf::Vertex{ cursorPositionAim, sf::Color::Red });
					aimline.append(lineEnd);
					processMouseEvents(nextEvent);
				}
				else //or if it isnt (it must be yellows)
				{
					//or process this stuff
					cursorPositionAim = MyVector3{ static_cast<float>(nextEvent.mouseMove.x), static_cast<float>(nextEvent.mouseMove.y),0.0f }; //finds end point (mouse click)
					lineStart = (sf::Vertex{ yellowBallPosition, sf::Color::Blue });
					aimline.append(lineStart);
					lineEnd = (sf::Vertex{ cursorPositionAim, sf::Color::Red });
					aimline.append(lineEnd);
					processMouseEvents(nextEvent);
				}
			}//if released
			if (sf::Event::MouseButtonReleased == nextEvent.type)
			{
				if (whiteBallVelocity == noMovement) //if white ball not moving
				{				
					//draw and process the aim
					aimline.clear();
					processMouseEvents(nextEvent);
				}
				else if (yellowBallVelocity == noMovement) //if yellow ball not moving
				{							
					//draw and process aiming
					aimline.clear();
					processMouseEvents(nextEvent);
				}

			}
		}
	}
	
}


/// <summary>
/// Update
/// Do this every 1/60 of a second
/// </summary>

void Game::update(sf::Time t_deltaTime) //should really only be function calls ;-;
{
	if (m_exitGame)//if i exit, close the window
	{
		m_window.close();
	}

	//ball movement functions, calls each one. 
	moveBall(whiteBallPosition, whiteBallVelocity); //move ball
	moveBall(redBallPosition, redBallVelocity); //move ball
	moveBall(yellowBallPosition, yellowBallVelocity); //move ball

	//check if any balls have touched the cushions
	if (inPlayWhite == true)
	{//if white is in play (not potted), check for cushion touching
		checkForCushionTouchin(whiteBallPosition, whiteBallVelocity); //has white touched a cushion
	}
	if (inPlayRed == true)
	{//if red is in play (not potted), check for cushion touching
		checkForCushionTouchin(redBallPosition, redBallVelocity); //has red touched a cushion
	}
	if (inPlayYellow == true)
	{//if yellow is in play (not potted), check for cushion touching
		checkForCushionTouchin(yellowBallPosition, yellowBallVelocity); //check for cushion touchin
	}

	//check for ball collisions between all balls
	if (inPlayWhite == true && inPlayRed == true)
	{//white - red collision
		checkForBallTouchinAgainstRed(whiteBallPosition, whiteBallVelocity, redBallPosition, redBallVelocity); //check for collision
	}
	if (inPlayYellow == true && inPlayRed == true)
	{//yellow - red collisions
		checkForBallTouchinAgainstRed(yellowBallPosition, yellowBallVelocity, redBallPosition, redBallVelocity); //check for collision
	}
	if (inPlayYellow == true && inPlayWhite == true)
	{//yellow- white collisions
		checkForBallTouchinWhiteYellow(whiteBallPosition, whiteBallVelocity, yellowBallPosition, yellowBallVelocity); //chgeck for collision
	}
	
	lookoutForCannons(); //functionthat detects cannons, if present
	investigatePocketOccurences(); //function that detects pocket things, like potting, hazard,fouls etc

	//if nothing is moving, bool = true, proceed to aim
	//also contains some minor checks for stuff needed to know at the end of a turn,
	//like if red is off table, put it back
	noBallMovement(whiteBallVelocity, redBallVelocity, yellowBallVelocity);

	//update text strings to display text each update in case score changes
	whiteScoreText.setString("White score: " + std::to_string(whiteScore));
	yellowScoreText.setString("Yellow score: " + std::to_string(yellowScore));
}



void Game::setupStuff()
{
	//setting up green background
	playingField.setSize(sf::Vector2f(760.0f, 460.0f));
	playingField.setPosition(20, 20); //border is 20
	playingField.setFillColor(sf::Color(static_cast<unsigned int>(16), static_cast<unsigned int>(165), static_cast<unsigned int>(21))); //custom green

	//setup base ball
	ball.setRadius(15.0f); //radius
	ball.setOrigin(15.0f, 15.0f); //origin of ball to center

	//setup pocket array
	for (int i = 0; i < POCKET_NUM; i++)
	{
		pockets[i].setRadius(15.0f);
		pockets[i].setOrigin(15.0f, 15.0f);
	}	

	//set up pocket positions
	pockets[0].setPosition(pocketOnePos);
	pockets[1].setPosition(pocketTwoPos);
	pockets[2].setPosition(pocketThreePos);
	pockets[3].setPosition(pocketFourPos);
	pockets[4].setPosition(pocketFivePos);
	pockets[5].setPosition(pocketSixPos);

	//load font - extra stuff
	if (!font.loadFromFile("arial.ttf"))
	{
		// error...
	}

	//set up texts for extra stuff
	//white team
	whiteScoreText.setFont(font);
	whiteScoreText.setPosition(SCREEN_WIDTH - textIndent * 2, SCREEN_HEIGHT - yellowTextIndent);
	whiteScoreText.setString("White score : " + std::to_string(whiteScore));
	whiteScoreText.setCharacterSize(20);
	whiteScoreText.setFillColor(sf::Color::White);
	//yellow team
	yellowScoreText.setFont(font);
	yellowScoreText.setPosition(SCREEN_WIDTH - textIndent, SCREEN_HEIGHT - yellowTextIndent);
	yellowScoreText.setString("Yellow score: " + std::to_string(yellowScore));
	yellowScoreText.setCharacterSize(20);
	yellowScoreText.setFillColor(sf::Color::Yellow);

	//sounds
	//ball touch
	fireBuffer.loadFromFile("fire.wav");
	fireSound.setBuffer(fireBuffer);
	//pocket touch
	dropBuffer.loadFromFile("drop.wav");
	dropSound.setBuffer(dropBuffer);
	//cushion touch
	cushionBuffer.loadFromFile("cushion.wav");
	cushionSound.setBuffer(cushionBuffer);

}


/// CHECKS FOR BALLS TOUCHING THE CUSHION
void Game::checkForCushionTouchin(MyVector3 &t_position, MyVector3 &t_velocity)
{
	//checks to see if balls have touched the cushion
	//essentially sets them in an angle perpendicular to the cushion,
	//accounting the direction they came from (90 degree angle)
	if (t_position.x >= SCREEN_WIDTH - RADIUS - BORDER) // right cushion
	{
		//reverse the x and play a cushion osund
		t_position.x = SCREEN_WIDTH - RADIUS - BORDER;
		t_velocity.reverseX();
		cushionSound.play();
	}
	if (t_position.x <= BORDER + RADIUS) //left cushion
	{
		//reverse x and play cushion noise
		t_position.x = BORDER + RADIUS;
		t_velocity.reverseX();
		cushionSound.play();

	}
	if (t_position.y >= SCREEN_HEIGHT-RADIUS-BORDER) //bottom cushion
	{ //reverse y and play sound
		t_position.y = SCREEN_HEIGHT - RADIUS - BORDER;
		t_velocity.reverseY();
		cushionSound.play();

	}
	if (t_position.y <= BORDER + RADIUS) //top cushion
	{//reverse y and make noise
		t_position.y = BORDER + RADIUS;
		t_velocity.reverseY();
		cushionSound.play();

	}
}


//function to check if balls have touched each other (collided)
void Game::checkForBallTouchin(MyVector3 &t_position, MyVector3 &t_velocity, MyVector3 &t_positionTwo, MyVector3 &t_velocityTwo)
{
	//function for looking at ball touchin stuff
	thereIsADistanceBetweenUs = t_position - t_positionTwo; //distance between the two balls
	if (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //if collided
	{		
		//stores the velocity of A so that it doesn't get mangled by the below calculations
		MyVector3 temporaryVector = t_velocity;

		while (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //bug sometimes occurs when they collide (dancing). this stops it
		{
			t_position -= (t_velocity.unit()) / 10; //gets a vector unit and pushes balls away from each other
			t_positionTwo -= (t_velocityTwo.unit()) / 10;
			thereIsADistanceBetweenUs = t_position - t_positionTwo; //new distance becomes this
		}

		//pete's formula
		t_velocity = t_velocity.rejection(thereIsADistanceBetweenUs) + t_velocityTwo.projection(thereIsADistanceBetweenUs);
		t_velocityTwo = t_velocityTwo.rejection(thereIsADistanceBetweenUs) + temporaryVector.projection(thereIsADistanceBetweenUs);

	}


}


//when balls stop moving, do this function
//also checks for end of turn/round occurences and outcomes
void Game::noBallMovement(MyVector3 & t_velocity, MyVector3 & t_velocityTwo, MyVector3 & t_velocityThree)
{
	if (t_velocity == noMovement && t_velocityTwo == noMovement && t_velocityThree == noMovement) //if no movement
	{
		noMovementBool = true; //no movement of balls here

		//these check to see if balls that are not on the table, SHOULD be on the table
		//ie, hazard or returning object ball
		if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true)) //return red
		{
			inPlayRed = true; //red is in play
			redBallPosition = { 200.0f, 250.0f, 0.0f }; //reset
		}
		if (inPlayYellow == false && yellowsTurn == true) //if yellow's turn, put the ball there if it isnt there
		{
			inPlayYellow = true; //in play
			yellowBallPosition = { 600.0f, 250.0f, 0.0f };//reset position
		}
		if (inPlayWhite == false && WhitesTurn == true) //if white's go, and no cue ball, put it there
		{
			inPlayWhite = true;//inplay
			whiteBallPosition = { 400.0f, 250.0f, 0.0f };//reset position

		}


		//player shot and balls done moving (find result of match)
		//does not include cannon detection or hazards or pots
		if (noMovementBool == true && hasntShot == false)
		{
			produceRoundOutcome(); //get the outcome of round function //keep code tidy
		}
	}
	else
	{
		noMovementBool = false; //balls still moving
	}
}

//bool returning function to see if a collision has occured
bool Game::checkForCollision(MyVector3 & t_position, MyVector3 & t_velocity, MyVector3 & t_positionTwo, MyVector3 & t_velocityTwo)
{
	//check for collision between balls
	thereIsADistanceBetweenUs = t_position - t_positionTwo; //distance between balls
	if (thereIsADistanceBetweenUs.length() <= (RADIUS * 2)) //if collided
	{
		return true; //return collision confirmed
	}
	return false; //return no collision
}

//Function to check if white and red collide
void Game::checkForBallTouchinAgainstRed(MyVector3 & t_position, MyVector3 & t_velocity, MyVector3 & t_positionTwo, MyVector3 & t_velocityTwo)
{
	thereIsADistanceBetweenUs = t_position - t_positionTwo; //distance between the two balls
	if (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //if collided
	{
		//if it's white's turn
		if (WhitesTurn && t_position != yellowBallPosition) //determines which ball hit red
		{
			hitRed = true; //red hit
			std::cout << "Red was hit by White. \n"; //output
			fireSound.play(); //balls touch sound

		}
		else if (WhitesTurn && t_position != whiteBallPosition ) //if white hit it
		{
			if (hitRed == true) //already has hit red
			{
				//abort - forget it
			}
			else
			{	
				hitRed = false;
			}
		}

		//if yellow's turn
		if (yellowsTurn && t_position != whiteBallPosition && t_position != redBallPosition) //confirmed yellow hit red
		{
			hitRed = true;
			std::cout << "Red was hit by Yellow. \n"; //output + change bool
			fireSound.play(); //balls touch sound

		}
		else if (yellowsTurn && t_position != whiteBallPosition) //yellow hit red
		{
			if (hitRed == true) //if already true
			{
				//abort
			}
			else //else make it false
			{
				hitRed = false;
			}
		}
		//stores the velocity of A so that it doesn't get mangled by the below calculations
		MyVector3 temporaryVector = t_velocity;

		while (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //bug sometimes occurs when they collide (dancing). this stops it
		{
			t_position -= (t_velocity.unit()) / 10; //moves the balls one unit vector away from each other each time they're in contact
			t_positionTwo -= (t_velocityTwo.unit()) / 10; //this avoids the square dancing balls bugs
			thereIsADistanceBetweenUs = t_position - t_positionTwo; //new distance
		}

		//pete's formula
		t_velocity = t_velocity.rejection(thereIsADistanceBetweenUs) + t_velocityTwo.projection(thereIsADistanceBetweenUs);
		t_velocityTwo = t_velocityTwo.rejection(thereIsADistanceBetweenUs) + temporaryVector.projection(thereIsADistanceBetweenUs);
	}
}

//more or less same as other described function above. checks for collion of yellow - white balls
void Game::checkForBallTouchinWhiteYellow(MyVector3 & t_position, MyVector3 & t_velocity, MyVector3 & t_positionTwo, MyVector3 & t_velocityTwo)
{
	thereIsADistanceBetweenUs = t_position - t_positionTwo; //distance between the two balls
	if (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //if collided
	{
		//if it's white's turn
		if (WhitesTurn == true)
		{
			hitYellow = true; //he hit yellow
		}
		else //yellow's go
		{
			hitWhite = true; //he hit white
		}
		
		std::cout << "White Ball and Yellow Ball have collided. \n"; //output
		fireSound.play();//sound play 
		//stores the velocity of A so that it doesn't get mangled by the below calculations
		MyVector3 temporaryVector = t_velocity;

		while (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //bug sometimes occurs when they collide (dancing). this stops it
		{
			t_position -= (t_velocity.unit()) / 10; //moves the balls one unit vector away from each other each time they're in contact
			t_positionTwo -= (t_velocityTwo.unit()) / 10; //this avoids the square dancing balls bugs
			thereIsADistanceBetweenUs = t_position - t_positionTwo; //new distance
		}

		//pete's formula
		t_velocity = t_velocity.rejection(thereIsADistanceBetweenUs) + t_velocityTwo.projection(thereIsADistanceBetweenUs);
		t_velocityTwo = t_velocityTwo.rejection(thereIsADistanceBetweenUs) + temporaryVector.projection(thereIsADistanceBetweenUs);

	}
}

//checks for collisions between red and yellow
void Game::checkForBallTouchinRedYellow(MyVector3 & t_position, MyVector3 & t_velocity, MyVector3 & t_positionTwo, MyVector3 & t_velocityTwo)
{
	thereIsADistanceBetweenUs = t_position - t_positionTwo; //distance between the two balls
	if (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //if collided
	{
		hitWhite = true; //innapropriately named, but oh well, is just because i needed a third 'hit'
		//coutput to user
		std::cout << "Yellow Ball and Red Ball have collided. \n"; //display to user
		fireSound.play();//play sound

		//stores the velocity of A so that it doesn't get mangled by the below calculations
		MyVector3 temporaryVector = t_velocity;

		while (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //bug sometimes occurs when they collide (dancing). this stops it
		{
			t_position -= (t_velocity.unit()) / 10; //moves the balls one unit vector away from each other each time they're in contact
			t_positionTwo -= (t_velocityTwo.unit()) / 10; //this avoids the square dancing balls bugs
			thereIsADistanceBetweenUs = t_position - t_positionTwo; //new distance
		}

		//pete's formula
		t_velocity = t_velocity.rejection(thereIsADistanceBetweenUs) + t_velocityTwo.projection(thereIsADistanceBetweenUs);
		t_velocityTwo = t_velocityTwo.rejection(thereIsADistanceBetweenUs) + temporaryVector.projection(thereIsADistanceBetweenUs);

	}
}

void Game::checkForBallTouchinYellowWhite(MyVector3 & t_position, MyVector3 & t_velocity, MyVector3 & t_positionTwo, MyVector3 & t_velocityTwo)
{
	thereIsADistanceBetweenUs = t_position - t_positionTwo; //distance between the two balls
	if (thereIsADistanceBetweenUs.length() < (RADIUS * 2))//if collided
	{
		hitWhite = true; //hit white is confirmed
		//collision occured debug
		std::cout << "Yellow Ball and White Ball have collided. \n"; //output to user
		fireSound.play(); //play sound

		//stores the velocity of A so that it doesn't get mangled by the below calculations
		MyVector3 temporaryVector = t_velocity;

		while (thereIsADistanceBetweenUs.length() < (RADIUS * 2)) //bug sometimes occurs when they collide (dancing). this stops it
		{
			t_position -= (t_velocity.unit()) / 10; //moves the balls one unit vector away from each other each time they're in contact
			t_positionTwo -= (t_velocityTwo.unit()) / 10; //this avoids the square dancing balls bugs
			thereIsADistanceBetweenUs = t_position - t_positionTwo; //new distance
		}

		//pete's formula
		t_velocity = t_velocity.rejection(thereIsADistanceBetweenUs) + t_velocityTwo.projection(thereIsADistanceBetweenUs);
		t_velocityTwo = t_velocityTwo.rejection(thereIsADistanceBetweenUs) + temporaryVector.projection(thereIsADistanceBetweenUs);

	}
}


//this function pushes the ball forward after aim line is drawn and released
//(releases the balls after line is drawn)
void Game::steadyAimFire(MyVector3 &t_position, MyVector3 &t_velocity)
{
	aimline.clear(); //clear line
	aimDeterminedVelocity = t_position - cursorPositionAim; //determine velocity
	t_velocity = aimDeterminedVelocity; //attach to ball being aimed
	t_velocity = t_velocity * REDUCE_PUSH;	 //lessen it so that it is less powerful
	hasntShot = false; //player has shot
}

//function that detects pocket collisions and determines outcomes
void Game::investigatePocketOccurences()
{
	if (checkForPocketTouchin(whiteBallPosition) == true)//has white been potted?
	{
		pottedWhite = true;  //white is potted
		dropSound.play(); //play pot noise
	}
	if (pottedWhite == true) //if white is potted find out why
	{	//white foul
		if (WhitesTurn == true && hitRed == false && hitYellow == false) //if white's turn and he hit nothing
		{ //bool changing
			whiteBallVelocity = noMovement; //reset white ball
			whiteBallPosition = { 400.0f, 250.0f, 0.0f };
			pottedWhite = false; //not potted
			WhitesTurn = false; //yellow's turn
			yellowsTurn = true;
			yellowScore += 2; //yellow score update
			std::cout << "White foul. +2 points to Yellow. Yellow Score = " << yellowScore << " \n";
			hasntShot = true; //next turn
			std::cout << " ======== Yellow's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn)//reset balls if need be
			{
				inPlayWhite = true; //is ball in play
				whiteBallPosition = { 400.0f, 250.0f, 0.0f }; //starting pos of white ball
			}
			if (inPlayYellow == false && yellowsTurn) //if yellow needs hi ball, put it on table
			{
				inPlayYellow = true; //is ball in play
				yellowBallPosition = { 600.0f, 250.0f, 0.0f }; //starting pos of yellow ball
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;//is ball in play
				redBallPosition = { 200.0f, 250.0f, 0.0f }; //starting pos of red ball
			}

		}//white got a red hazard
		else if (WhitesTurn == true && hitRed == true && hitYellow == false) //if white hit red AND pocketed itself
		{
			pottedWhite = false; //white no longer potted
			whiteScore += 3;
			std::cout << "White got a Red hazard. +3 points. White Score = " << whiteScore << " \n"; //score + display
			hasntShot = true; //has shot
			hitRed = false; //didnt get red
			whiteBallVelocity = noMovement; //white ball be still
			inPlayWhite = false; //white not inplay
			whiteBallPosition = { 400.0f, 250.0f, 0.0f }; //reset position
		}//white got a yellow hazard
		else if (WhitesTurn && hitRed == false && hitYellow == true) //white hit yellow and pocketed itself
		{	
			whiteBallPosition = { 400.0f, 250.0f, 0.0f }; //reset Pos
			hitYellow = false;
			pottedWhite = false; //white pocketed
			whiteScore += 2;
			std::cout << "White got a Yellow hazard. +2 points. White Score = " << whiteScore << " \n"; //stats + display
			hasntShot = true; //hasnt shot
			whiteBallVelocity = noMovement; //be still
			inPlayWhite = false; //not in play

		} //Yellow potted white
		else if (yellowsTurn == true && hitRed == false && hitWhite == true) //if white was potted by yellow
		{
			hitWhite = false; //hit white no longer
			pottedWhite = false; //white is not potted
			yellowScore += 2;
			std::cout << "Yellow potted White. +2 points. Yellow Score = " << yellowScore << " \n"; //score + display
			hasntShot = true; //waiting to shoot
			whiteBallPosition = { 400.0f, 250.0f, 0.0f }; //reset position
			whiteBallVelocity = noMovement; //not moving
			inPlayWhite = false; //not in play
		}
	}

	//has red been potted?
	if (checkForPocketTouchin(redBallPosition) == true) //is red ball p[otted?
	{
		pottedRed = true; //red ball is potted
		dropSound.play(); //play sound
	}
	if (pottedRed == true) //if red is potted, find out why
	{
		if (WhitesTurn) //If it's white's go
		{	//white potted red
			redBallPosition = { 200.0f, 250.0f, 0.0f }; //reset pos
			redBallVelocity = noMovement; //be still
			hitRed = false;
			hitYellow = false;
			pottedRed = false;
			whiteScore += 3;
			std::cout << "White potted Red. +3 points. White Score = " << whiteScore << " \n"; //stats + display
			hasntShot = true; //waiting to go again

			redBallVelocity = noMovement; //be still red
			inPlayRed = false;//not in play

		}
		else //if it's yellow's go
		{	//yellow potted red
			redBallPosition = { 200.0f, 250.0f, 0.0f }; //reset position
			redBallVelocity = noMovement; // be still
			hitRed = false;
			hitWhite = false;
			pottedRed = false;
			yellowScore += 3;
			std::cout << "Yellow potted Red. +3 points. Yellow Score = " << yellowScore << " \n"; //score + display
			hasntShot = true; //waiting to go again
			redBallVelocity = noMovement; //be still
			inPlayRed = false; //not in play
		}
	}

	//checkForPocketTouchin regards to yellow ball
	if (checkForPocketTouchin(yellowBallPosition) == true)  //is yellow in a pocket
	{
		pottedYellow = true; //he is? good
		dropSound.play(); //pla ysound
	}
	if (pottedYellow == true) //if yellow has been potted find out why
	{
		if (yellowsTurn == true && hitRed == false && hitWhite == false) //has yellow fouled itself?
		{
				hasntShot = true;//wait to go again
				yellowBallPosition = { 600.0f, 250.0f, 0.0f }; ///reset position
				yellowBallVelocity = noMovement; //be still
				pottedYellow = false; //not potted
				WhitesTurn = true; //switch turns
				yellowsTurn = false;
				whiteScore += 2;
				std::cout << "Yellow foul. +2 points to White. White Score = " << whiteScore << " \n";//score + display
				std::cout << " ======== White's Turn. ======== \n"; //change turns
				//return balls to play
				if (inPlayWhite == false && WhitesTurn) //reset balls if need be. commented all this before
				{
					inPlayWhite = true;
					whiteBallPosition = { 400.0f, 250.0f, 0.0f };
				}
				if (inPlayYellow == false && yellowsTurn)
				{
					inPlayYellow = true;
					yellowBallPosition = { 600.0f, 250.0f, 0.0f };
				}
				if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
				{
					inPlayRed = true;
					redBallPosition = { 200.0f, 250.0f, 0.0f };
				}
			
		}//Yellow score = White Hazard
		else if (yellowsTurn == true && hitRed == false && hitWhite == true) //has yellow hit white and gone into a pocket?
		{ 
			yellowBallPosition = { 600.0f, 250.0f, 0.0f }; //reset pos
			yellowBallVelocity = noMovement; //be still
			hitWhite = false;
			pottedYellow = false; //not potted 
			yellowScore += 2;
			std::cout << "Yellow got a white Hazard. +2 points. Yellow Score = " << yellowScore << " \n"; //stats and display
			hasntShot = true; //wait to go again
			inPlayYellow = false; //not in play
		}//yellow = red hazard
		else if (yellowsTurn == true && hitRed == true && hitWhite == false) //has yellow hit red and pocketed itself
		{
			yellowBallPosition = { 600.0f, 250.0f, 0.0f }; //reset pos
			yellowBallVelocity = noMovement; //be still
			hitRed = false;
			pottedYellow = false;
			yellowScore += 2;
			std::cout << "Yellow got a Red hazard. +3 points. Yellow Score = " << yellowScore << " \n"; //score + display
			hasntShot = true; //wait to shoot again
			yellowBallVelocity = noMovement; //be still
			inPlayYellow = false; //not in play
		} //yellow potted by white
		else if (WhitesTurn == true && hitRed == false && hitYellow == true) //has white ball potted yellow?
		{			
			hitYellow = false;
			pottedYellow = true;//potted on white's turn
			whiteScore += 2;
			std::cout << "White potted Yellow. +2 points. white Score = " << whiteScore << " \n"; //stats display
			hasntShot = true;
			yellowBallVelocity = noMovement;//be still
			yellowBallPosition = { 600.0f, 250.0f, 0.0f };//reset pos
			inPlayYellow = false;//not in play
		}
	}
}

//Function to detect cannons, called in update
void Game::lookoutForCannons()
{
	//Detect a cannon while turn is on-going
	if (WhitesTurn == true) //if it's white's go
	{	//check if he hit red
		if (checkForCollision(whiteBallPosition, whiteBallVelocity, redBallPosition, redBallVelocity) == true) //return bool
		{
			hitRed = true; //if yes, red confirmed hit
		} //check if he yellow
		if (checkForCollision(whiteBallPosition, whiteBallVelocity, yellowBallPosition, yellowBallVelocity) == true)
		{
			hitYellow = true; 
		} //if he has done both, he got a cannon
		if (hitYellow == true && hitRed == true) //cannon got
		{
			cannonScored = true; //cannon confirmed
			whiteScore += 2;
			std::cout << "Player One scored a cannon (white cue ball). Player has " << whiteScore << " points. \n"; //stats display
			hitYellow = false;//reset cannopns bools
			hitRed = false;
		}
	}
	if (yellowsTurn == true) //is it yellow's go?
	{
		if (checkForCollision(whiteBallPosition, whiteBallVelocity, redBallPosition, redBallVelocity) == true) // has he hit red
		{
			hitRed = true; //he hit red
		}
		if (checkForCollision(redBallPosition, redBallVelocity, yellowBallPosition, yellowBallVelocity) == true)//did he get white
		{
			hitWhite = true; // he sure did
		}
		if (hitWhite == true && hitRed == true)//if he did both he got a cannon
		{//same as before
			cannonScored = true;//cannon yes
			yellowScore += 2;
			std::cout << "Player Two scored a cannon (yellow cue ball). Player has " << yellowScore << " points. \n";//stats display
			hitWhite = false;//reset bools
			hitRed = false;
		}
	}
}

//when the balls stop rolling, and no other outcome produces, give one of these
//includes cannon + reset, hit yellow and nothing else, 
// hit red and nothing else, and a hit nothing foul (for both cue balls)
void Game::produceRoundOutcome()
{
	if (WhitesTurn == true)//Determine outcome on white's go
	{	
		//if you get a cannon
		if (cannonScored == true) //gonna stop commenting all these too, soon, ive done it dozens of times throughout the code
		{
			cannonScored = false; //got cannon
			hitRed = false;
			hitYellow = false;
			hasntShot = true;
		} //if you hit nothing (foul)
		else if (hitRed == false && hitYellow == false)
		{
			yellowScore += 2;
			std::cout << "White foul. +2 points to yellow. yellow Score = " << yellowScore << " \n"; //stats
			WhitesTurn = false;
			yellowsTurn = true; //switch turns
			hasntShot = true;
			std::cout << " ======== Yellow's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn) //reset balls if need be (pretty useless)
			{
				inPlayWhite = true;//played with
				whiteBallPosition = { 400.0f, 250.0f, 0.0f };//reset posit
			}
			if (inPlayYellow == false && yellowsTurn)
			{
				inPlayYellow = true;//in play
				yellowBallPosition = { 600.0f, 250.0f, 0.0f }; //reset pos
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;//inplay
				redBallPosition = { 200.0f, 250.0f, 0.0f };//reset pos
			}
		}//only hit red and nothin else
		else if (hitRed == true && hitYellow == false)
		{
			WhitesTurn = false;
			yellowsTurn = true; //switch turns
			hitRed = false;
			hasntShot = true;
			std::cout << " ======== Yellow's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn) //reset balls if need be
			{
				inPlayWhite = true;
				whiteBallPosition = { 400.0f, 250.0f, 0.0f };
			}
			if (inPlayYellow == false && yellowsTurn)
			{
				inPlayYellow = true;
				yellowBallPosition = { 600.0f, 250.0f, 0.0f };
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;
				redBallPosition = { 200.0f, 250.0f, 0.0f };
			}
		}//if hit yellow and nothing else
		else if (hitYellow == true && hitRed == false)
		{	//same as hit red/white and nothing else
			WhitesTurn = false;
			yellowsTurn = true; //switch turns
			hitYellow = false;
			hasntShot = true;
			std::cout << " ======== Yellow's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn) //reset balls if need be
			{
				inPlayWhite = true;
				whiteBallPosition = { 400.0f, 250.0f, 0.0f };
			}
			if (inPlayYellow == false && yellowsTurn)
			{
				inPlayYellow = true;
				yellowBallPosition = { 600.0f, 250.0f, 0.0f };
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;
				redBallPosition = { 200.0f, 250.0f, 0.0f };
			}

		}
		}
		else if (yellowsTurn == true) //do the same stuff again, but only if it's yellow's go
		{
		///if you get a cannon
		if (inPlayWhite == false) // if white ball is gone
		{
			inPlayWhite = true; //reset it
		}
		if (cannonScored == true)//did he get a cannon
		{
			cannonScored = false;
			hitRed = false;
			hitWhite = false;
			hasntShot = true;

		}//if you hit nothing - foul
		else if (hitRed == false && hitWhite == false)
		{
			whiteScore += 2;
			std::cout << "Yellow foul. +2 points to White. White Score = " << whiteScore << " \n";
			yellowsTurn = false;
			WhitesTurn = true;
			hasntShot = true;
			std::cout << "======== White's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn) //reset balls if need be
			{
				inPlayWhite = true;
				whiteBallPosition = { 400.0f, 250.0f, 0.0f };
			}
			if (inPlayYellow == false && yellowsTurn)
			{
				inPlayYellow = true;
				yellowBallPosition = { 600.0f, 250.0f, 0.0f };
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;
				redBallPosition = { 200.0f, 250.0f, 0.0f };
			}
		}//hit red
		else if (hitRed == true && hitWhite == false)
		{
			WhitesTurn = true;
			yellowsTurn = false;
			hitRed = false;
			hasntShot = true;
			std::cout << " ======== White's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn) //reset balls if need be
			{
				inPlayWhite = true;
				whiteBallPosition = { 400.0f, 250.0f, 0.0f };
			}
			if (inPlayYellow == false && yellowsTurn)
			{
				inPlayYellow = true;
				yellowBallPosition = { 600.0f, 250.0f, 0.0f };
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;
				redBallPosition = { 200.0f, 250.0f, 0.0f };
			}
		}//hit white
		else if (hitWhite == true && hitRed == false)
		{
			WhitesTurn = true;
			yellowsTurn = false;
			hitWhite = false;
			hasntShot = true;
			std::cout << " ======== White's Turn. ======== \n";
			if (inPlayWhite == false && WhitesTurn) //reset balls if need be
			{
				inPlayWhite = true;
				whiteBallPosition = { 400.0f, 250.0f, 0.0f };
			}
			if (inPlayYellow == false && yellowsTurn)
			{
				inPlayYellow = true;
				yellowBallPosition = { 600.0f, 250.0f, 0.0f };
			}
			if (inPlayRed == false && (yellowsTurn == true || WhitesTurn == true))
			{
				inPlayRed = true;
				redBallPosition = { 200.0f, 250.0f, 0.0f };
			}
		}

	}
}



//pocket collisions
bool Game::checkForPocketTouchin(MyVector3 & t_position)
{
	//distance between the two balls
	//This detects distance between all pockets and the ball in question
	//if any of them occur, it returns a bool to confirm collision
	pocketDistance = t_position - pocketOnePos; //top left
	if (pocketDistance.length() < (RADIUS * 2))
	{
		return true;//return yes
	}
	pocketDistance = t_position - pocketTwoPos; //top mid
	if (pocketDistance.length() < (RADIUS * 2))
	{
		return true;//return yes
	}
	pocketDistance = t_position - pocketThreePos; //top right
	if (pocketDistance.length() < (RADIUS * 2))
	{
		return true;//return yes
	}
	pocketDistance = t_position - pocketFourPos; //bottom left
	if (pocketDistance.length() < (RADIUS * 2))
	{
		return true;//return yes
	}
	pocketDistance = t_position - pocketFivePos;//bottom mid
	if (pocketDistance.length() < (RADIUS * 2))
	{
		return true;//return yes
	}
	pocketDistance = t_position - pocketSixPos; //bottom right
	if (pocketDistance.length() < (RADIUS * 2))
	{
		return true; //return yes
	}
		return false; //if no collisions, return false / null
}


//function that moves the balls
void Game::moveBall(MyVector3 &t_position, MyVector3 &t_velocity)
{
	t_position = t_position + t_velocity; //position plus velocity 

	if (t_velocity.length() < 0.10) //stop the ball if it's movement is pain stakingly slow
	{
		
		t_velocity = { 0.0f, 0.0f , 0.0f }; //no movement
	}
	t_velocity = t_velocity * FRICTION; //call friction method
}


//processes mouse events
void Game::processMouseEvents(sf::Event t_mouseEvent)
{
	//if you left click
	if (sf::Mouse::Left == t_mouseEvent.mouseButton.button)
	{	//only aim if not moving and no one has taken their go
		if (noMovementBool == true && hasntShot == true)
		{
			aimline.clear(); //clear anything 
			if (WhitesTurn == true) //draw to white ball
			{
				aimline.clear();//clear line
				cursorPositionAim = MyVector3{ static_cast<float>(t_mouseEvent.mouseButton.x), static_cast<float>(t_mouseEvent.mouseButton.y),0.0f }; //finds end point (mouse click)
				lineStart = (sf::Vertex{ whiteBallPosition, sf::Color::Blue }); //set line start to white 
				aimline.append(lineStart); //append
				lineEnd = (sf::Vertex{ cursorPositionAim, sf::Color::Red }); //set line end to the cursor
				aimline.append(lineEnd); //append it
				steadyAimFire(whiteBallPosition, whiteBallVelocity); //call the produce velocity function
			}
			else //or draw to yellow ball
			{
				aimline.clear(); //clear line
				cursorPositionAim = MyVector3{ static_cast<float>(t_mouseEvent.mouseButton.x), static_cast<float>(t_mouseEvent.mouseButton.y),0.0f }; //finds end point (mouse click)
				lineStart = (sf::Vertex{ yellowBallPosition, sf::Color::Cyan }); //line start to yellow
				aimline.append(lineStart); //append
				lineEnd = (sf::Vertex{ cursorPositionAim, sf::Color::Magenta }); //line end to cursor
				aimline.append(lineEnd); //append
				steadyAimFire(yellowBallPosition, yellowBallVelocity); //produce velocity with the aimline
			}
		}
	}
}

void Game::render()
{

	m_window.clear(sf::Color::Green); //clear background to green
	m_window.draw(playingField); //draw the playing field
	m_window.draw(aimline); //draw aim line

	//display texts for scores
	m_window.draw(whiteScoreText);
	m_window.draw(yellowScoreText);

	//draw white ball
	if (inPlayWhite == true)//only do this if white is on the table
	{
		ball.setPosition(whiteBallPosition); //set position
		ball.setFillColor(sf::Color::White); //white ball
		m_window.draw(ball); //drawball
	}
	//draw red ball
	if (inPlayRed == true)//only if red is on table
	{
		ball.setPosition(redBallPosition); //red ball pos
		ball.setFillColor(sf::Color::Red);//red ball
		m_window.draw(ball); //draw ball
	}

	//draw yellow ball
	if (inPlayYellow == true)//only if yellow is still in play
	{	ball.setPosition(yellowBallPosition); //yellow pos
		ball.setFillColor(sf::Color::Yellow); //yellow ball
		m_window.draw(ball); //draw ball
	}

	//draw pockets 
	//draws the array of pockets via for loop
	for (int i = 0; i < POCKET_NUM; i++)
	{
		pockets[i].setFillColor(sf::Color::Black); //set color
		m_window.draw(pockets[i]); //draw all pockets
	}
	m_window.display(); //display everything
}
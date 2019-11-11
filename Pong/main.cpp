//Author: RUTUL THAkKAR
//GAME 230 Pong Game Assignment 3
//Date: 10 Nov 2019
/*
Done Parts: A, B, C, D, E, F, G, H, I, J, K, L
extra Parts Done: M, S 
-> background image texture
-> max speed is also implemented to avoid tunnelling
-> speed increases once hits paddle and resets when point is scored
-> 2 user players or 2 AI players (switch using C and V keys) (left side player uses W for up and S for down, right side player uses up arrow and down arrow)
-> Background game music
-> sound ball hit paddle
-> sound point is scored (ball goes out of scene)
-> sound ball hits top or bottom edge
-> background music stops when game ends / a player wins.
-> sound when player wins game
-> toggle player 1 AI Mode with keyboard key C (default AI)
-> toggle player 2 AI Mode with keyboard key V (default player)
*/
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;

enum Player {PLAYER1 = 1, PLAYER2 = 2};
enum BallDirection {STOP = -1, LEFT, TOPLEFT, BOTTOMLEFT, RIGHT, TOPRIGHT, BOTTOMRIGHT};
float screenWidth = 512.0;
float screenHeight = 512.0;
bool isGameRunning = true;
//vectors representing directions (0 -> left, 1 -> top left, 2 -> bottom left, 3 -> right, 4 -> top right, 5 -> bottom right)
sf::Vector2f direction[6]= { sf::Vector2f(-1,0),sf::Vector2f(-0.5,-0.866),sf::Vector2f(-0.5,0.866),sf::Vector2f(1,0),sf::Vector2f(0.5,-0.866),sf::Vector2f(0.5,0.866) };
float deltaTime;

//for sounds
sf::SoundBuffer bufferHit;
sf::SoundBuffer bufferBackgroundMusic;
sf::SoundBuffer bufferPoint;
sf::SoundBuffer bufferWall;
sf::SoundBuffer bufferGameWon;

sf::Sound soundHit;
sf::Sound soundBackgroundMusic;
sf::Sound soundPoint;
sf::Sound soundWall;
sf::Sound soundGameWon;

//class for both paddles players
class Paddle {
	private:
		int playerNo;
		int score;
		float x;
		float y;
		float originalX;
		float originalY;
		float width;
		float height;
		int speed;
		Player controller;
		sf::RectangleShape paddleObj;
		bool AIOn;
	public:
		Paddle(int id, float x, float y, float width, float height, Player player, int speed) {
			this->playerNo = id;
			this->score = 0;
			this->x = x;
			this->y = y;
			this->originalX = x;
			this->originalY = y;
			this->width = width;
			this->height = height;
			this->controller = player; // tells if its paddle 1 or 2
			this->speed = speed;
			this->AIOn = true;
			paddleObj.setSize(sf::Vector2f(this->width, this->height));
			paddleObj.setOutlineThickness(1);
			paddleObj.setPosition(x, y);
			paddleObj.setOrigin(paddleObj.getSize().x / 2, paddleObj.getSize().y / 2); //origin is centre
		}
		inline int getSpeed() { return speed; }
		inline float getWidth() { return width; }
		inline float getheight() { return height; }
		inline Player getPlayerInfo() { return controller; }
		inline float getXPosition() { return x; }
		inline float getYPosition() { return y; }
		inline float getOriginalXPosition() { return originalX; }
		inline float getOriginalYPosition() { return originalY; }
		inline void reset() { x = originalX; y = originalY; paddleObj.setPosition(x, y); }
		inline int getScore() { return score; }
		void increaseScore() { 
			score++; 
			if (score >= 5) {
				isGameRunning = false;
			}
		}
		inline sf::Vector2f getOriginOfPaddle() { return paddleObj.getOrigin(); }
		inline void resetGame() { reset(); score = 0; }
		inline void setAIMode(bool b) { AIOn = b; }
		inline bool getAIMode() { return AIOn; }
		inline sf::RectangleShape getPaddleObj() { return paddleObj; }
		//void setTexture() {
		//	if (playerNo == 1) {
		//		//load player1 texture
		//		sf::Texture t;
		//		if (!(t.loadFromFile("paddle1.png"))) {
		//			cout << "ERROR LOADING TEXTURE PADDLE 1" << endl;
		//		}
		//		else {
		//			cout << "LOADING TEXTURE PADDLE 1" << endl;
		//		}
		//		paddleObj.setTexture(&t);
		//	}
		//	else {
		//		//load player2 texture
		//		sf::Texture t2;
		//		if (!(t2.loadFromFile("paddle2.png"))) {
		//			cout << "ERROR LOADING TEXTURE PADDLE 2" << endl;
		//		}
		//		else {
		//			cout << "LOADING TEXTURE PADDLE 2" << endl;
		//		}
		//		paddleObj.setTexture(&t2);
		//	}
		//}
		void draw(sf::RenderWindow *window) {
			window->draw(paddleObj);
		}

		void move(int amt, int direction){
			if (direction == 1) {
				// down
				y += amt;
				if ((y + height/2.0f + 10.0f) >= screenHeight) {
					y = screenHeight - height/2.0f - 10.0f;
				}
				paddleObj.setPosition(sf::Vector2f(x, y));
				
			}
			else {
				// up
				y -= amt;
				if(y < 70){
					y = 70;
				}
				paddleObj.setPosition(sf::Vector2f(x, y));
			}
		}
};

class Ball {
	private:
		sf::Vector2f position;
		sf::Vector2f originalPosition;
		float radius;
		sf::Vector2f directionOfBall;
		BallDirection dir;
		float speed;
		float originalSpeed;
		float maxSpeed;
		sf::CircleShape ballObj;

	public:
		Ball(sf::Vector2f position, float radius) {
			this->position = position;
			this->originalPosition = position;
			this->radius = radius;
			this->dir = RIGHT;
				//(BallDirection)((rand() + 4) % 6); 
			cout << "Direction : " << dir << endl;
			this->directionOfBall = direction[dir];
			this->speed = 0.3f;
			this->originalSpeed = 0.3f;
			this->maxSpeed = 1.2f;
			
			sf::CircleShape shape(radius);
			shape.setFillColor(sf::Color::White);
			shape.setPosition(position);
			shape.setOrigin(radius, radius); // centre
			ballObj = shape;
		}

		inline float getRadius() { return radius; }
		inline float getX() { return position.x; }
		inline float getY() { return position.y; }
		inline float getOriginalX() { return originalPosition.x; }
		inline float getOriginalY() { return originalPosition.y; }
		inline float getSpeed() { return speed; }
		inline float getOriginalSpeed() { return originalSpeed; }
		void reset() { 
			position.x = originalPosition.x; 
			position.y = originalPosition.y; 
			dir = RIGHT;
				//(BallDirection)((rand() + 4) % 6);
			directionOfBall = direction[dir];
			speed = originalSpeed;
			ballObj.setPosition(position);
		}
		void increaseSpeed() {
			if (speed <= maxSpeed) {
				//cout << "INCREASE SPEED : OLD : " << speed << endl;
				speed = speed * (110.0f/100.0f); //increase by 10 percent
				//cout << "INCREASE SPEED : NEW : " << speed << endl;
			}
		}

		void draw(sf::RenderWindow *window) {
			window->draw(ballObj);
		}

		void move(Paddle *player1, Paddle *player2, sf::Text *player1ScoreText, sf::Text *player2ScoreText) {
				/*cout << "DIRECTION : " << directionOfBall.x << "," << directionOfBall.y << endl;
				cout << "OLD " << position.x << "," << position.y << endl;*/
				position.x = position.x + directionOfBall.x * speed * deltaTime;
				position.y = position.y + directionOfBall.y * speed * deltaTime;
				/*cout << "NEW " << position.x << "," << position.y << endl;
				cout << "SPEED " << speed << endl;*/

				
				//Paddle Collision
				if (directionOfBall == direction[LEFT] || directionOfBall == direction[TOPLEFT] || directionOfBall == direction[BOTTOMLEFT]) {
					//ball is moving towards player 1 (AI)
					int heightPaddle1 = player1->getheight();
					int widthPaddle1 = player1->getWidth();
					int posXPaddle1 = player1->getXPosition();
					int posYPaddle1 = player1->getYPosition();
					int topPaddle1 = posYPaddle1 - heightPaddle1 / 2.0f;
					int bottomPaddle1 = posYPaddle1 + heightPaddle1 / 2.0f;
					int sectionHeightPaddle1 = heightPaddle1 / 3.0f; // dividing paddle into 3 equal parts into top middle and bottom
					int topEndPaddle1 = topPaddle1 + sectionHeightPaddle1;
					int bottomStartPaddle1 = bottomPaddle1 - sectionHeightPaddle1; // middle section between topEnd and BottomStart
					//top section to direct ball towards top screen edge
					//bottom section to direct ball towards bottom screen edge
					//middle section to keep it centre

					//ballObj.getPosition().y - radius is ball top edge
					//ballObj.getPosition().y + radius is ball bottom edge
					/*cout << "BALL to hit paddle 1" << endl;
					cout << "Ball pos : X " << position.x << " Y " << position.y << endl;

					cout << "PADDLE 1 Position " << posXPaddle1 << "," << posYPaddle1 << endl;
					cout << "PADDLE 1 top " << topPaddle1 << " Bottom Paddle1 " << bottomPaddle1 << endl;
					cout << "PADDLE 1 height " << heightPaddle1 << " width " << widthPaddle1 << endl;
					cout << "PADDLE 1 section height " << sectionHeightPaddle1 << " top end Paddle1 " << topEndPaddle1 << " Bottom start Paddle1 " << bottomStartPaddle1 << endl;*/
					if ((ballObj.getPosition().x - 2.0f * radius) <= posXPaddle1) {
						//cout << "INSIDE PADDLE 1 X CONDITION!" << endl;
						if (((ballObj.getPosition().y + radius) >= topPaddle1)
							&& ((ballObj.getPosition().y - radius) <= bottomPaddle1)) {
							// hits the paddle
							//cout << "INSIDE PADDLE 1 Y CONDITION!" << endl;
							position.x = position.x + 2.0f*radius;
							ballObj.setPosition(position);
							soundHit.play();
							increaseSpeed();
							if  (ballObj.getPosition().y >= topPaddle1 && ballObj.getPosition().y < topEndPaddle1) {
								//ball hit top section of paddle
								if (dir == BOTTOMLEFT) {
									dir = TOPRIGHT;
									//cout << "PADDLE 1: TOP SECTION Inside bottomleft" << endl;
								}
								else if (dir == TOPLEFT) {
									dir = TOPRIGHT;
									//cout << "PADDLE 1: TOP SECTION Inside topleft" << endl;
								}
								else {
									dir = TOPRIGHT;
									//cout << "PADDLE 1: TOP SECTION Inside else" << endl;
								}

								directionOfBall = direction[dir];
								//updating ball position
								position.x = position.x + directionOfBall.x * speed * deltaTime;
								position.y = position.y + directionOfBall.y * speed * deltaTime;

								//cout << "Hit top section of paddle 1" << endl;
							}
							else if (ballObj.getPosition().y >= topEndPaddle1 && ballObj.getPosition().y < bottomStartPaddle1) {
								//ball it middle section of paddle
								//cout << "Hit middle section of paddle 1" << endl;
								if (dir == LEFT) {
									dir = RIGHT;
									//cout << "PADDLE 1: MIDDLE SECTION Inside left" << endl;
								}
								else if (dir == TOPLEFT) {
									dir = BOTTOMRIGHT;
									//cout << "PADDLE 1: MIDDLE SECTION Inside topleft" << endl;
								}
								else if (dir == BOTTOMLEFT) {
									dir = TOPRIGHT;
									//cout << "PADDLE 1: MIDDLE SECTION Inside bottomleft" << endl;
								}
								else {
									dir = RIGHT;
									//cout << "PADDLE 1: MIDDLE SECTION Inside else" << endl;
								}

								directionOfBall = direction[dir];
								//updating ball position
								position.x = position.x + directionOfBall.x * speed * deltaTime;
								position.y = position.y + directionOfBall.y * speed * deltaTime;
							}
							else if (ballObj.getPosition().y >= bottomStartPaddle1 && ballObj.getPosition().y < bottomPaddle1) {
								//ball it bottom section of paddle
								//cout << "Hit bottom section of paddle 1" << endl;
								if (dir == BOTTOMLEFT) {
									dir = BOTTOMRIGHT;
									//cout << "PADDLE 1: BOTTOM SECTION Inside bottomleft" << endl;
								}
								else if (dir == TOPLEFT) {
									dir = BOTTOMRIGHT;
									//cout << "PADDLE 1: BOTTOM SECTION Inside topleft" << endl;
								}
								else {
									//cout << "PADDLE 1: BOTTOM SECTION Inside else" << endl;
									dir = BOTTOMRIGHT;
								}

								directionOfBall = direction[dir];
								//updating ball position
								position.x = position.x + directionOfBall.x * speed * deltaTime;
								position.y = position.y + directionOfBall.y * speed * deltaTime;
							}
							else {
								//cout << "Hit else of paddle 1" << endl;
							}
						}
					}
				}
				else {
					//ball is moving towards player 2 (player)
					int heightPaddle2 = player2->getheight();
					int widthPaddle2 = player2->getWidth();
					int posXPaddle2 = player2->getXPosition();
					int posYPaddle2 = player2->getYPosition();
					int topPaddle2 = posYPaddle2 - heightPaddle2 / 2.0f;
					int bottomPaddle2 = posYPaddle2 + heightPaddle2 / 2.0f;
					int sectionHeightPaddle2 = heightPaddle2 / 3.0f; // dividing paddle into 3 equal parts into top middle and bottom
					int topEndPaddle2 = topPaddle2 + sectionHeightPaddle2;
					int bottomStartPaddle2 = bottomPaddle2 - sectionHeightPaddle2; // middle section between topEnd and BottomStart
					//top section to direct ball towards top screen edge
					//bottom section to direct ball towards bottom screen edge
					//middle section to keep it centre

					//ballObj.getPosition().y - radius is ball top edge
					//ballObj.getPosition().y + radius is ball bottom edge

					if ((ballObj.getPosition().x + 2.0f * radius) >= posXPaddle2) {
						if (((ballObj.getPosition().y + radius) >= topPaddle2)
							&& ((ballObj.getPosition().y - radius) <= bottomPaddle2)) {
							// hits the paddle
							soundHit.play();
							
							increaseSpeed();
							if (ballObj.getPosition().y >= topPaddle2 && ballObj.getPosition().y < topEndPaddle2) {
								//ball it top section of paddle
								position.x = position.x - 2.0f*radius;
								ballObj.setPosition(position);
								if (dir == RIGHT) {
									dir = TOPLEFT;
								}
								else if (dir == BOTTOMRIGHT) {
									dir = TOPLEFT;
								}
								else if (dir == TOPRIGHT) {
									dir = TOPLEFT;
								}
								else {
									dir = TOPLEFT;
								}

								directionOfBall = direction[dir];
								//updating ball position
								position.x = position.x + directionOfBall.x * speed * deltaTime;
								position.y = position.y + directionOfBall.y * speed * deltaTime;

								//cout << "Hit top section of paddle 2" << endl;
							}
							else if (ballObj.getPosition().y >= topEndPaddle2 && ballObj.getPosition().y < bottomStartPaddle2) {
								//ball it middle section of paddle
								//cout << "Hit middle section of paddle 2" << " DIRECTION: " << dir << endl;
								position.x = position.x - 2.0f*radius;
								ballObj.setPosition(position);
								if (dir == RIGHT){
									dir = LEFT;
									//cout << "Inside right";
								}
								else if (dir == TOPRIGHT) {
									dir = BOTTOMLEFT;
									//cout << "Inside topright";
								}
								else if (dir == BOTTOMRIGHT) {
									dir = TOPLEFT;
									//cout << "Inside bottomright";
								}
								else {
									dir = LEFT;
									//cout << "Inside else";
								}
								//cout << "DIRECTION : " << dir << " (" << direction[dir].x << "," << direction[dir].y << ")" << endl;
								directionOfBall = direction[dir];
								//updating ball position
								//cout << "OLD POS PADDLE2 : " << position.x << "," << position.y;
								position.x = position.x + directionOfBall.x * speed * deltaTime;
								position.y = position.y + directionOfBall.y * speed * deltaTime;
								//cout << "NEW POS PADDLE2 : " << position.x << "," << position.y;
							}
							else if (ballObj.getPosition().y >= bottomStartPaddle2 && ballObj.getPosition().y < bottomPaddle2) {
								//ball it bottom section of paddle
								//cout << "Hit bottom section of paddle 2" << endl;
								position.x = position.x - 2.0f*radius;
								ballObj.setPosition(position);
								if (dir == BOTTOMRIGHT) {
									dir = BOTTOMLEFT;
								}
								else if (dir == TOPRIGHT) {
									dir = BOTTOMLEFT;
								}
								else {
									dir = BOTTOMLEFT;
								}

								directionOfBall = direction[dir];
								//updating ball position
								position.x = position.x + directionOfBall.x * speed * deltaTime;
								position.y = position.y + directionOfBall.y * speed * deltaTime;
							}
							else {
								//cout << "Hit else of paddle 2" << endl;
							}
							
							//cout << "Hit the paddle ball pos " << ballObj.getPosition().x << "," << ballObj.getPosition().y << " Paddle position " << posXPaddle2 << "," << posYPaddle2 << endl;
						}
					}
				}

				//Wall
				if (position.x <= radius) {
					//left window edge player 2 scores
					soundPoint.play();
					//cout << " ball speed " << speed << endl;
					player2->increaseScore();
					reset();
					player1ScoreText->setString(to_string(player1->getScore()) + "");
					player2ScoreText->setString(to_string(player2->getScore()) + "");
				} else if (position.x >= screenWidth - radius) {
					//right window edge
					//player 1 scores
					soundPoint.play();
					//cout << " ball speed " << speed << endl;
					player1->increaseScore();
					reset();
					player1ScoreText->setString(to_string(player1->getScore()) + "");
					player2ScoreText->setString(to_string(player2->getScore()) + "");
				}

				if (position.y < radius) {
					//top window edge
					soundWall.play();
					//changing direction
					directionOfBall.y *= -1.0;
					//cout << "DIRECTION : 3rd if" << directionOfBall.x << "," << directionOfBall.y << endl;
					position.y = 0 + radius + directionOfBall.y * speed * deltaTime;
				} 
				else if (position.y >= screenHeight - radius) {
					//bottom window edge

					soundWall.play();
					 // changes direction
					directionOfBall.y *= -1.0;
					position.y = screenHeight - radius + directionOfBall.y * speed * deltaTime;
				}
				ballObj.setPosition(position);
				/*cout << "DIRECTION : 4th if " << directionOfBall.x << "," << directionOfBall.y << endl;
				cout << "Ball pos " << ballObj.getPosition().x << endl;*/
		}
		
};

class Line {
	private:
		sf::Vector2f startPosition;
		sf::Vector2f endPosition;
		sf::Color color;
		sf::Vertex line[2];
	public:
		Line(sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Color color) {
			this->startPosition = startPosition;
			this->endPosition = endPosition;
			this->color = color;
		}

		inline float getStartPositionX() { return startPosition.x; };
		inline float getStartPositionY() { return startPosition.y; };
		inline float getEndPositionX() { return endPosition.x; };
		inline float getEndPositionY() { return endPosition.y; };
		inline sf::Color getColor() { return color; };

		void draw(sf::RenderWindow *window) {
			line[0].position = startPosition;
			line[0].color = color;
			line[1].position = endPosition;
			line[1].color = color;

			window->draw(line,2,sf::Lines);
		}
};

double distance(double x1, double y1, double x2, double y2) {
	double xDelta = x1 - x2;
	double yDelta = y1 - y2;
	return sqrt(xDelta * xDelta + yDelta * yDelta);
}

void gameRestart(Paddle *paddle1, Paddle *paddle2, Ball *ball, sf::Text *player1ScoreText, sf::Text *player2ScoreText) {
	paddle1->resetGame();
	paddle2->resetGame();

	player1ScoreText->setString(to_string(paddle1->getScore()) + "");
	player2ScoreText->setString(to_string(paddle2->getScore()) + "");

	ball->reset();
}

int main()
{
	bool gameWonSoundPlayed = false;
	bool pressedW = false;
	bool pressedS = false;
	bool pressedUp = false;
	bool pressedDown = false;
	Paddle player1(1,60.0,(screenHeight/2),30.0,80.0,PLAYER1,10);
	Paddle player2(2,440.0, (screenHeight / 2), 30.0, 80.0, PLAYER2,10);
	Ball ball(sf::Vector2f(screenWidth / 2, screenHeight / 2),10);
	Line centerLine(sf::Vector2f(screenWidth / 2, 0), sf::Vector2f(screenWidth / 2, screenHeight), sf::Color::White);

	sf::RenderWindow window(sf::VideoMode((int)screenWidth, (int)screenHeight), "Pong");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	
	if (!bufferBackgroundMusic.loadFromFile("sound.wav")) {
		cout << "ERROR LOADING SOUND !" << endl;
	}
	else {
		cout << "SUCCESS LOADING SOUND !" << endl;
	}

	soundBackgroundMusic.setBuffer(bufferBackgroundMusic);
	soundBackgroundMusic.setVolume(10.0f);
	soundBackgroundMusic.play();

	if (!bufferHit.loadFromFile("hit.wav")) {
		cout << "ERROR LOADING HIT SOUND !" << endl;
	}
	else {
		cout << "SUCCESS LOADING HIT SOUND !" << endl;
	}

	soundHit.setBuffer(bufferHit);
	soundHit.setVolume(100.0f);
	soundHit.setPitch(2.0f);

	if (!bufferPoint.loadFromFile("point.wav")) {
		cout << "ERROR LOADING POINT SOUND !" << endl;
	}
	else {
		cout << "SUCCESS LOADING POINT SOUND !" << endl;
	}

	soundPoint.setBuffer(bufferPoint);
	soundPoint.setVolume(100.0f);
	soundPoint.setPitch(2.0f);

	if (!bufferWall.loadFromFile("wall.wav")) {
		cout << "ERROR LOADING WALL SOUND !" << endl;
	}
	else {
		cout << "SUCCESS LOADING WALL SOUND !" << endl;
	}

	soundWall.setBuffer(bufferWall);
	soundWall.setVolume(100.0f);
	soundWall.setPitch(2.0f);

	if (!bufferGameWon.loadFromFile("tada.wav")) {
		cout << "ERROR LOADING GAME WON SOUND !" << endl;
	}
	else {
		cout << "SUCCESS LOADING GAME WON SOUND !" << endl;
	}

	soundGameWon.setBuffer(bufferGameWon);
	soundGameWon.setVolume(80.0f);


	//sf::Music music;	// Now the music routines
	//if (!music.openFromFile("sound.wav")) {
	//	cout << "ERROR LOADING MUSIC !" << endl;
	//}
	//else {
	//	cout << "SUCCESS LOADING MUSIC !" << endl;
	//}
	//music.play();

	/*player1.setTexture();
	player2.setTexture();*/

	/*sf::Texture t;
	if (!t.loadFromFile("paddle1.jpg")) {
		cout << "ERROR LOADING IMAGE PADDLE 1 in MAIN !" << endl;
	}
	else {
		cout << "SUCCESS LOADED TEXTURE PADDLE 1 MAIN" << endl;
	}
	player1.getPaddleObj().setTexture(&t);
	player2.getPaddleObj().setTexture(&t);*/

	player1.setAIMode(true);
	player2.setAIMode(false);

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("bg.png")) {
		cout << "ERROR LOADING BACKGROUND IMG " << endl;
	}
	else {
		cout << "SUCCESS LOADING BACKGROUND IMG " << endl;
	}
	sf::Sprite backgroundSprite(backgroundTexture);

	sf::Font font;
	if (!(font.loadFromFile("arial.ttf"))) {
		cout << "ERROR LOADING FONT!" << endl;
	}
	else {
		cout << "FONT LOADED!" << endl;
	}
	sf::Text player1ScoreText;
	sf::Text player2ScoreText;
	sf::Text gameOverText;
	sf::Text player1AIOnText;
	sf::Text player2AIOnText;
	sf::Text winnerText;

	gameOverText.setString("GAME OVER");
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(50);
	gameOverText.setOrigin(0, 0);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setStyle(sf::Text::Bold);
	gameOverText.setPosition(sf::Vector2f(100, 90));

	if (player1.getScore() >= 5) {
		winnerText.setString("Winner: Player 1!");
	}
	else if(player2.getScore() >= 5) {
		winnerText.setString("Winner: Player 2!");
	}
	else {
		winnerText.setString("");
	}
	
	winnerText.setFont(font);
	winnerText.setCharacterSize(40);
	winnerText.setOrigin(0, 0);
	winnerText.setFillColor(sf::Color::White);
	winnerText.setStyle(sf::Text::Bold);
	winnerText.setPosition(sf::Vector2f(100, 200));

	player1ScoreText.setString(to_string(player1.getScore()) + "");
	player2ScoreText.setString(to_string(player2.getScore()) + "");
	player1ScoreText.setFont(font);
	player2ScoreText.setFont(font);
	player1ScoreText.setCharacterSize(30);
	player2ScoreText.setCharacterSize(30);
	player1ScoreText.setOrigin(0,0);
	player2ScoreText.setOrigin(0,0);
	player1ScoreText.setFillColor(sf::Color::White);
	player2ScoreText.setFillColor(sf::Color::White);
	player1ScoreText.setStyle(sf::Text::Bold);
	player2ScoreText.setStyle(sf::Text::Bold);
	player1ScoreText.setPosition(sf::Vector2f(125, 50));
	player2ScoreText.setPosition(sf::Vector2f(375, 50));

	player1AIOnText.setString("AI");
	player1AIOnText.setFont(font);
	player1AIOnText.setCharacterSize(30);
	player1AIOnText.setOrigin(0, 0);
	player1AIOnText.setFillColor(sf::Color::White);
	player1AIOnText.setPosition(sf::Vector2f(125, 400));

	player2AIOnText.setString("USER");
	player2AIOnText.setFont(font);
	player2AIOnText.setCharacterSize(30);
	player2AIOnText.setOrigin(0, 0);
	player2AIOnText.setFillColor(sf::Color::White);
	player2AIOnText.setPosition(sf::Vector2f(300, 400));

	sf::Clock clock;
	clock.restart();
	while (window.isOpen())
	{
		deltaTime = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		if (player1.getAIMode()) {
			if (ball.getY() < player1.getYPosition()) {
				player1.move(player1.getSpeed(), -1);
			}
			if (ball.getY() > player1.getYPosition()) {
				player1.move(player1.getSpeed(), 1);
			}
		}


		if (player2.getAIMode()) {
			if (ball.getY() < player2.getYPosition()) {
				player2.move(player2.getSpeed(), -1);
			}
			if (ball.getY() > player2.getYPosition()) {
				player2.move(player2.getSpeed(), 1);
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (isGameRunning) {
				if (event.type == sf::Event::KeyReleased) {
					if (event.key.code == sf::Keyboard::Up) {
						pressedUp = false;
					}
					if (event.key.code == sf::Keyboard::Down) {
						pressedDown = false;
					}
					if (event.key.code == sf::Keyboard::W) {
						pressedW = false;
					}
					if (event.key.code == sf::Keyboard::S) {
						pressedS = false;
					}
				}

				if (event.type == sf::Event::KeyPressed) {
					if (!player1.getAIMode() && !player2.getAIMode()) {
						if (pressedW && pressedUp) {
							player2.move(player2.getSpeed(), -1);
							player1.move(player1.getSpeed(), -1);
						} 
						if (pressedS && pressedDown) {
							player2.move(player2.getSpeed(), 1);
							player1.move(player1.getSpeed(), 1);
						}
					}
					//AI MODE BOTH PLAYERS
					if (player1.getAIMode() && player2.getAIMode()) {
						if (ball.getY() < player1.getYPosition()) {
							player1.move(player1.getSpeed(), -1);
						}
						if (ball.getY() > player1.getYPosition()) {
							player1.move(player1.getSpeed(), 1);
						}
						if (ball.getY() < player2.getYPosition()) {
							player2.move(player2.getSpeed(), -1);
						}
						if (ball.getY() > player2.getYPosition()) {
							player2.move(player2.getSpeed(), 1);
						}
					}

					if (!player2.getAIMode()) {
						if (event.key.code == sf::Keyboard::Up) {
							pressedUp = true;
							player2.move(player2.getSpeed(), -1);
						}
						if (event.key.code == sf::Keyboard::Down) {
							pressedDown = true;
							player2.move(player2.getSpeed(), 1);
						}
					}
					else {
						//AI CODE
						if (ball.getY() < player2.getYPosition()) {
							player2.move(player2.getSpeed(), -1);
						}
						if (ball.getY() > player2.getYPosition()) {
							player2.move(player2.getSpeed(), 1);
						}
					}

					if (event.key.code == sf::Keyboard::C) {
						if (player1.getAIMode()) {
							player1AIOnText.setString("USER");
							player1.setAIMode(false);
						}
						else {
							player1AIOnText.setString("AI");
							player1.setAIMode(true);
						}
					}
					if (event.key.code == sf::Keyboard::V) {
						if (player2.getAIMode()) {
							player2AIOnText.setString("USER");
							player2.setAIMode(false);
						}
						else {
							player2AIOnText.setString("AI");
							player2.setAIMode(true);
						}
					}

					if (!player1.getAIMode()) {
						if (event.key.code == sf::Keyboard::W) {
							pressedW = true;
							player1.move(player2.getSpeed(), -1);
						}
						if (event.key.code == sf::Keyboard::S) {
							pressedS = true;
							player1.move(player2.getSpeed(), 1);
						}
					}
					else {
						//AI CODE
						//move up
						if (ball.getY() < player1.getYPosition()) {
							player1.move(player1.getSpeed(), -1);
						}
						if(ball.getY() > player1.getYPosition()) {
							player1.move(player1.getSpeed(), 1);
						}
					}
				}
			}
			else {
				if (player1.getScore() >= 5 || player2.getScore() >= 5) {
					isGameRunning = false;

					

					if (event.type == sf::Event::KeyPressed) {
						if (event.key.code == sf::Keyboard::Space) {
							//resetGame
							isGameRunning = true;
							window.clear(sf::Color::Black);
							window.draw(backgroundSprite);
							gameWonSoundPlayed = false;
							gameRestart(&player1, &player2, &ball, &player1ScoreText, &player2ScoreText);
							player1.draw(&window);
							player2.draw(&window);
							ball.draw(&window);
							centerLine.draw(&window);
							ball.move(&player1, &player2, &player1ScoreText, &player2ScoreText);
							window.draw(player1ScoreText);
							window.draw(player2ScoreText);
							window.display();
						}
					}
				}
				else {
					isGameRunning = true;
					if (event.type == sf::Event::KeyReleased) {
						if (event.key.code == sf::Keyboard::Up) {
							pressedUp = false;
						}
						if (event.key.code == sf::Keyboard::Down) {
							pressedDown = false;
						}
						if (event.key.code == sf::Keyboard::W) {
							pressedW = false;
						}
						if (event.key.code == sf::Keyboard::S) {
							pressedS = false;
						}
					}
					if (event.type == sf::Event::KeyPressed) {
						if (!player1.getAIMode() && !player2.getAIMode()) {
							if (pressedW && pressedUp) {
								player2.move(player2.getSpeed(), -1);
								player1.move(player1.getSpeed(), -1);
							}
							if (pressedS && pressedDown) {
								player2.move(player2.getSpeed(), 1);
								player1.move(player1.getSpeed(), 1);
							}
						}

						if (player1.getAIMode() && player2.getAIMode()) {
							if (ball.getY() < player1.getYPosition()) {
								player1.move(player1.getSpeed(), -1);
							}
							if (ball.getY() > player1.getYPosition()) {
								player1.move(player1.getSpeed(), 1);
							}
							if (ball.getY() < player2.getYPosition()) {
								player2.move(player2.getSpeed(), -1);
							}
							if (ball.getY() > player2.getYPosition()) {
								player2.move(player2.getSpeed(), 1);
							}
						}
						if (!player2.getAIMode()) {
							if (event.key.code == sf::Keyboard::Up) {
								pressedUp = true;
								player2.move(player2.getSpeed(), -1);
							}
							if (event.key.code == sf::Keyboard::Down) {
								pressedDown = true;
								player2.move(player2.getSpeed(), 1);
							}
						}
						else {
							//AI CODE
							if (ball.getY() < player2.getYPosition()) {
								player2.move(player2.getSpeed(), -1);
							}
							if (ball.getY() > player2.getYPosition()) {
								player2.move(player2.getSpeed(), 1);
							}
						}
						
						if (event.key.code == sf::Keyboard::C) {
							if (player1.getAIMode()) {
								player1.setAIMode(false);
								player1AIOnText.setString("USER");
							}
							else {
								player1.setAIMode(true);
								player1AIOnText.setString("AI");
							}
						}
						if (event.key.code == sf::Keyboard::V) {
							if (player2.getAIMode()) {
								player2.setAIMode(false);
								player2AIOnText.setString("USER");
							}
							else {
								player2.setAIMode(true);
								player2AIOnText.setString("AI");
							}
						}

						if (!player1.getAIMode()) {
							if (event.key.code == sf::Keyboard::W) {
								pressedW = true;
								player1.move(player2.getSpeed(), -1);
							}
							if (event.key.code == sf::Keyboard::S) {
								pressedS = true;
								player1.move(player2.getSpeed(), 1);
							}
						}
						else {
							//AI CODE
							if (ball.getY() < player1.getYPosition()) {
								player1.move(player1.getSpeed(), -1);
							}
							if (ball.getY() > player1.getYPosition()) {
								player1.move(player1.getSpeed(), 1);
							}
						}
					}
				}
			}
			
		}

		
		window.clear(sf::Color::Black);

		window.draw(backgroundSprite);

		player1.draw(&window);
		player2.draw(&window);
		ball.draw(&window);
		centerLine.draw(&window);
		if (!isGameRunning) {
			soundBackgroundMusic.stop();
			window.draw(gameOverText);

			if (player1.getScore() >= 5) {
				winnerText.setString("Winner: Player 1!");
			}
			else if (player2.getScore() >= 5) {
				winnerText.setString("Winner: Player 2!");
			}
			else {
				winnerText.setString("");
			}

			window.draw(winnerText);
			if (soundGameWon.getStatus() != sf::Sound::Playing && !gameWonSoundPlayed) {
				gameWonSoundPlayed = true;
				soundGameWon.play();
			}	
		}
		else {
			gameWonSoundPlayed = false;
			if (soundBackgroundMusic.getStatus() != sf::Sound::Playing)
				soundBackgroundMusic.play();
			ball.move(&player1, &player2, &player1ScoreText, &player2ScoreText);
		}
		
		window.draw(player1ScoreText);
		window.draw(player2ScoreText);
		window.draw(player1AIOnText);
		window.draw(player2AIOnText);
		window.display();

		if (soundBackgroundMusic.getStatus() != sf::Sound::Playing)
			soundBackgroundMusic.play();
	}

	return 0;
}
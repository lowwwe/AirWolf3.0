/// <summary>
/// author Pete Lowe Jan 26
/// you need to change the above line or lose marks
/// Also don't have any member properties called Delete...
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )
// ignore this warning
// C:\SFML - 3.0.0\include\SFML\System\Exception.hpp(41, 47) : 
// warning C4275 : non dll - interface class 'std::runtime_error' used as base for dll - interface class 'sf::Exception'

/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the #endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour

enum class Direction
{
	None,
	Left,
	Right
};

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void processMousePress(const std::optional<sf::Event> t_event);
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();

	void animateHelo();
	void move();
	
	
	void setupSprites();
	void setupAudio();

	
	sf::RenderWindow m_window; // main SFML window

	

	

	sf::Texture m_heloTexture; // texture for the heloicopter
	sf::Sprite m_heloSprite{m_heloTexture };// spite for the helopcopter
	


	sf::SoundBuffer m_heloSoundBuffer; // sound buffer
	sf::Sound m_heloSound{ m_heloSoundBuffer }; // used to play sound

	bool m_DELETEexitGame; // control exiting game

	int m_currentFrame = 0;// current frame for animation
	float m_currentFrameCounter = 0.0f;// frame counter
	float m_frameIncrement = 0.234;// frame increment =14/60 == 2.33

	Direction m_facing = Direction::None;// facing of helo
	sf::Vector2f m_location{ 200.0f,200.0f };// location of Helo
	sf::Vector2f m_velocity{ 0.0f,0.0f };// velocity of helo
	sf::Vector2f m_target{ 0.0f,0.0f };// target location for helo

	float m_speed = 5.0f; // helo speed 

};

#pragma warning( pop ) 
#endif // !GAME_HPP


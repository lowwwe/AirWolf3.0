/// <summary>
/// author Pete Lowe jan 26
/// you need to change the above line or lose marks
/// 
/// 
/// Estimate 60 min probally correct
/// 14:35 - 
/// </summary>


#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the texts
/// load and setup the images
/// load and setup the sounds
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{800U, 600U}, 32U }, "AirWolf" },
	m_DELETEexitGame{false} //when true game will exit
{
	
	setupSprites(); // load texture
	setupAudio(); // load sounds
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
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
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_DELETEexitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (newEvent->is<sf::Event::MouseButtonPressed>())
		{
			processMousePress(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_DELETEexitGame = true; 
	}
}

void Game::processMousePress(const std::optional<sf::Event> t_event)
{
	const sf::Event::MouseButtonPressed* newMousePress = t_event->getIf<sf::Event::MouseButtonPressed>();
	sf::Vector2f path;// path of helicopter flight


	if (sf::Mouse::Button::Left == newMousePress->button)
	{
		m_target = static_cast<sf::Vector2f>(newMousePress->position);
		path = m_target - m_location;
		path = path.normalized();
		m_heloSound.setPitch(1.0f);
		m_frameIncrement = 0.5f;
		m_velocity = path * m_speed;
		if (newMousePress->position.x < m_location.x)
		{
			m_facing = Direction::Left;
			m_heloSprite.setScale(sf::Vector2f{ -1.0f, 1.0f });
		}
		else
		{
			m_facing = Direction::Right;
			m_heloSprite.setScale(sf::Vector2f{ 1.0f,1.0f });
		}
	}
}

/// <summary>
/// Check if any keys are currently pressed
/// </summary>
void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_DELETEexitGame = true; 
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (m_DELETEexitGame)
	{
		m_window.close();
	}
	animateHelo();
	move();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(ULTRAMARINE);

	m_window.draw(m_heloSprite);

	
	
	m_window.display();
}


/// <summary>
/// aninate the helocoptor
/// 4 frames number 0,1,2,3
/// 
/// </summary>
void Game::animateHelo()
{
	int newFramewNumber = 0; // new frame number if different to current then change
	m_currentFrameCounter += m_frameIncrement;
	newFramewNumber = static_cast<int>(m_currentFrameCounter) % 4;


	if (newFramewNumber != m_currentFrame)
	{
		m_currentFrame = newFramewNumber;
		m_heloSprite.setTextureRect(sf::IntRect{sf::Vector2i{0,newFramewNumber * 64}, sf::Vector2i{180,64}}); // {0,0} - {0,64} - {0,128} - {0,196}

	}
}


/// <summary>
/// move helicopter using existing velocity
/// and stop when we get to target
/// </summary>
void Game::move()
{

	m_location += m_velocity;
	m_heloSprite.setPosition(m_location);
	if (m_facing == Direction::Left)
	{
		if (m_location.x < m_target.x)
		{
			m_velocity = sf::Vector2f{ 0.0f,0.0f };
			m_facing = Direction::None;
			m_heloSound.setPitch(0.5f);
			m_frameIncrement = 0.234f;
		}
	}
	if (Direction::Right == m_facing)
	{
		if (m_location.x > m_target.x)
		{
			m_facing = Direction::None;
			m_velocity = sf::Vector2f{ 0.0f,0.0f };
			m_heloSound.setPitch(0.5f);
			m_frameIncrement = 0.234f;
		}
	}
}


/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprites()
{
	if(!m_heloTexture.loadFromFile("ASSETS\\IMAGES\\helicopter.png"))
	{
		std::cout << "problem wiuth helo image" << std::endl;
	}
	m_heloSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,196}, sf::Vector2i{180,64} });
	m_heloSprite.setOrigin(sf::Vector2f{ 90.0f,0.0f });

	
}

/// <summary>
/// load sound file and assign buffers
/// </summary>
void Game::setupAudio()
{
	if (!m_heloSoundBuffer.loadFromFile("Assets/audio/helicopter.wav"))
	{
		std::cout << "error with helo sound file" << std::endl;
	}
	m_heloSound.setLooping(true);
	m_heloSound.setPitch(0.5f);
	m_heloSound.play();
	
	
}

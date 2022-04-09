#include <SFML/Graphics.hpp>
#include <iostream>
#include <direct.h>
#include <limits.h>

using namespace std;

string current_working_dir()
{
    char buff[260];
    _getcwd(buff, 260);
    string current_working_dir(buff);
    
    return current_working_dir;
}


int main()
{   
    int fps = 60;
    // CREATE WINDOW
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");   
    window.setFramerateLimit(fps);
    

// IMPORT FONT
sf::Font arial;
if (!arial.loadFromFile("arial.ttf"))
{
    return EXIT_FAILURE;
};

// SHOW LOCATION OF POINTER (FOR DEBUGGING)
sf::Text text;
text.setFont(arial);
text.setString("Hello World!");
text.setCharacterSize(24);
text.setFillColor(sf::Color::Green);
text.setStyle(sf::Text::Bold);


// CREATE walls
sf::VertexArray wallLeft(sf::LineStrip, 2);
wallLeft[0].position = sf::Vector2f(0, 0);
wallLeft[1].position = sf::Vector2f(0, 480);

sf::VertexArray wallTop(sf::LineStrip, 2);
wallTop[0].position = sf::Vector2f(0, 0);
wallTop[1].position = sf::Vector2f(640, 0);

sf::VertexArray wallRight(sf::LineStrip, 2);
wallRight[0].position = sf::Vector2f(640, 0);
wallRight[1].position = sf::Vector2f(640, 480);

sf::VertexArray wallBottom(sf::LineStrip, 2);
wallBottom[0].position = sf::Vector2f(0, 480);
wallBottom[1].position = sf::Vector2f(640, 480);


//CREATE CIRCLE
sf::CircleShape circle(20.f, 50);
circle.setFillColor(sf::Color::Green);



// DECLARE INITIAL VARIABLES FOR SIMULATION
double deltaTime = 1.0 / 60.0;
sf::Vector2f pos = sf::Vector2f(200.0, 200.0);
sf::Vector2f accel = sf::Vector2f(0.0, 300.0);
sf::Vector2f velocity = sf::Vector2f(200.0, 0.0);
double dampening = 0.8;



int frameDelay = 15;
unsigned short frameCounter = 0;
int circleCount = 0;
vector<sf::CircleShape> vCircles;
// RENDER LOOP
while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }

    // CALCULATING CIRCLE POSITION
    velocity.x = velocity.x + (accel.x * deltaTime);
    velocity.y = velocity.y + (accel.y * deltaTime);

    pos.x = pos.x + (velocity.x * deltaTime);
    pos.y = pos.y + (velocity.y * deltaTime);


    if (frameCounter < frameDelay)
    {
        frameCounter += 1;
    }
    else if (frameCounter >= frameDelay)
    {
        sf::CircleShape circle1(5.0, 10);
        circle1.setPosition((pos.x + circle.getRadius()) - circle1.getRadius(), (pos.y + circle.getRadius()) - circle1.getRadius());
        vCircles.push_back(circle1);

        frameCounter = 0;
        circleCount += 1;
    };



    circle.setPosition(pos);


	// COLLISION DETECTION
    if (circle.getPosition().x <= wallLeft.getBounds().left)
    {
        velocity.x = -velocity.x * dampening;
        pos.x = wallLeft.getBounds().left;
        cout << velocity.x << endl;
    }
    else if ((circle.getPosition().x + circle.getRadius() * 2) >= wallRight.getBounds().left)
    {
        velocity.x = -velocity.x * dampening;
        pos.x = (wallRight.getBounds().left) - circle.getRadius() * 2;
        cout << velocity.x << endl;
    };

	if (circle.getPosition().y <= wallTop.getBounds().top)
	{
		velocity.y = -velocity.y * dampening;
        pos.y = wallTop.getBounds().top;
		cout << velocity.y << endl;
	}
    else if ((circle.getPosition().y + circle.getRadius() * 2) >= wallBottom.getBounds().top)
    {
        velocity.y = -velocity.y * dampening;
        // pos.y + circle.getRadius()*2 = wallBottom.getBounds().top
        pos.y = (wallBottom.getBounds().top) - circle.getRadius() * 2;
        cout << velocity.y << endl;
    }



	text.setString(to_string(circle.getPosition().x) + ", " + to_string(circle.getPosition().y));

	window.clear();
	window.draw(circle);
	for (sf::CircleShape c : vCircles)
	{
		window.draw(c);
	}
	window.draw(wallLeft);
	window.draw(wallTop);
	window.draw(wallBottom);
	window.draw(wallRight);
	window.draw(text);
	window.display();
}

    return 0;
}
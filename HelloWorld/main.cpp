#include <SFML/Graphics.hpp>
#include <iostream>
#include <direct.h>
#include <limits.h>
#include <Windows.h>


using namespace std;

string currentWorkingDir()
{
    char buff[260];
    _getcwd(buff, 260);
    string current_working_dir(buff);
    
    return current_working_dir;
}

sf::Vector2f midpoint(sf::Vector2f vector1, sf::Vector2f vector2)
{
    return sf::Vector2f::Vector2((float)(vector1.x + vector2.x) / 2, (float)(vector1.y + vector2.y) / 2);
};



int main()
{   
    int fps = 60;
    // CREATE WINDOW
    sf::RenderWindow window(sf::VideoMode(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)), "2d Physics Sim", sf::Style::Fullscreen);
    cout << GetSystemMetrics(SM_CXVIRTUALSCREEN) << endl;
    cout << GetSystemMetrics(SM_CYVIRTUALSCREEN) << endl;
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
    wallLeft[1].position = sf::Vector2f(0, window.getSize().y);

    sf::VertexArray wallTop(sf::LineStrip, 2);
    wallTop[0].position = sf::Vector2f(0, 0);
    wallTop[1].position = sf::Vector2f(window.getSize().x, 0);

    sf::VertexArray wallRight(sf::LineStrip, 2);
    wallRight[0].position = sf::Vector2f(window.getSize().x, 0);
    wallRight[1].position = sf::Vector2f(window.getSize().x, window.getSize().y);

    sf::VertexArray wallBottom(sf::LineStrip, 2);
    wallBottom[0].position = sf::Vector2f(0, window.getSize().y);
    wallBottom[1].position = sf::Vector2f(window.getSize().x, window.getSize().y);


    //CREATE CIRCLE
    sf::CircleShape circle(20.f, 50);
    circle.setFillColor(sf::Color::Green);



    // DECLARE INITIAL VARIABLES FOR SIMULATION
    double deltaTime = 1.0 / fps;
    sf::Vector2f pos = sf::Vector2f(200.0, 00.0);
    // sf::Vector2f accel = sf::Vector2f(0.0, 576.0);
    sf::Vector2f accel = sf::Vector2f(0.0, 576.0);
    sf::Vector2f velocity = sf::Vector2f(0.0, 200.0);
    double dampening = 0.8;
    int throwModifier = 1500;

    bool trailOn = true;
    unsigned short lineLength = 100;
    bool isDragging = false;
    int circleCount = 0;
    vector <sf::CircleShape> vCircles;
    sf::Vector2f lastFramePos = sf::Vector2f(NULL, NULL);
    sf::Vector2f distanceMoved;
    sf::Vector2f velocityWhileDragged;
    // RENDER LOOP
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                vCircles.clear();
            }
        }

        // MOUSE CURSOR TO PICK BALL UP
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true && sf::Mouse::getPosition(window).x >= circle.getPosition().x && sf::Mouse::getPosition(window).x <= (circle.getPosition().x + circle.getRadius() * 2)
            && sf::Mouse::getPosition(window).y <= (circle.getPosition().y + circle.getRadius() * 2) && sf::Mouse::getPosition(window).y >= circle.getRadius())
        {
            isDragging = true;
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            isDragging = false;

        }

        if (isDragging)
        {
            pos.x = sf::Mouse::getPosition(window).x - circle.getRadius();
            pos.y = sf::Mouse::getPosition(window).y - circle.getRadius();
            velocity = velocityWhileDragged;
        }

        if (!isDragging)
        {
            // CALCULATING CIRCLE POSITION
            velocity.x = velocity.x + (accel.x * deltaTime);
            velocity.y = velocity.y + (accel.y * deltaTime);

            pos.x = pos.x + (velocity.x * deltaTime);
            pos.y = pos.y + (velocity.y * deltaTime);

        };


        if (lastFramePos.x != NULL && lastFramePos.y != NULL)
        {
            distanceMoved.x = pos.x - lastFramePos.x;
            distanceMoved.y = pos.y - lastFramePos.y;

            velocityWhileDragged.x = distanceMoved.x * (deltaTime * throwModifier);
            velocityWhileDragged.y = distanceMoved.y * (deltaTime * throwModifier);

        };

        if (trailOn && lastFramePos != sf::Vector2f::Vector2(NULL,NULL)) {
            sf::CircleShape circle2(5.0, 10);
            circle2.setPosition(midpoint(sf::Vector2f::Vector2((lastFramePos.x + circle.getRadius()) - circle2.getRadius(), (pos.y + circle.getRadius()) - circle2.getRadius()),
                sf::Vector2f::Vector2((pos.x + circle.getRadius()) - circle2.getRadius(), (pos.y + circle.getRadius()) - circle2.getRadius())));
            vCircles.push_back(circle2);
            circleCount += 1;

            sf::CircleShape circle1(5.0, 10);
            circle1.setPosition((pos.x + circle.getRadius()) - circle1.getRadius(), (pos.y + circle.getRadius()) - circle1.getRadius());
            vCircles.push_back(circle1);
            circleCount += 1;
        };





        circle.setPosition(pos);


	    // COLLISION DETECTION
        if (circle.getPosition().x <= wallLeft.getBounds().left)
        {
            velocity.x = -velocity.x * dampening;
            pos.x = wallLeft.getBounds().left;
            // cout << velocity.x << endl;
        }
        else if ((circle.getPosition().x + circle.getRadius() * 2) >= wallRight.getBounds().left)
        {
            velocity.x = -velocity.x * dampening;
            pos.x = (wallRight.getBounds().left) - circle.getRadius() * 2;
            // cout << velocity.x << endl;
        };

	    if (circle.getPosition().y <= wallTop.getBounds().top)
	    {
		    velocity.y = -velocity.y * dampening;
            pos.y = wallTop.getBounds().top;
		    // cout << velocity.y << endl;
	    }
        else if ((circle.getPosition().y + circle.getRadius() * 2) >= wallBottom.getBounds().top)
        {
            velocity.y = -velocity.y * dampening;
            if (dampening != 1)
            {
                velocity.x = velocity.x * 0.99;
            }
            pos.y = (wallBottom.getBounds().top) - circle.getRadius() * 2;
            // cout << velocity.y << endl;
        }



	    text.setString(to_string(velocityWhileDragged.x) + ", " + to_string(velocityWhileDragged.y));

        lastFramePos = pos;

	    window.clear();
	    window.draw(circle);
	    for (sf::CircleShape c : vCircles)
	    {
            if (size(vCircles) >= lineLength)
            {
                vCircles.erase(vCircles.begin());
            }
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
};
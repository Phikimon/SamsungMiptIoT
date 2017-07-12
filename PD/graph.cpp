#include <cstdio>
#include <SFML/Graphics.hpp>
#include "graph.hpp"

Plot::Plot (Point max):
    max_  (max),
    data_ {},
    name_ {}
{}

Plot::~Plot () {}

void Plot::PlotMaker ()
{
    sf::RenderWindow window (sf::VideoMode (600, 400), "Plot");

    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                window.close ();
        }

    	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close ();
        }

        window.clear (sf::Color::Black);

        for (int i = 0; i < MAX_VAL; i++)
        {
            sf::Vertex line [] =
            {
                sf::Vertex (sf::Vector2f (data_[i].x   * 10, data_[i].y   * 10)),
                sf::Vertex (sf::Vector2f (data_[i+1].x * 10, data_[i+1].y * 10))
            };

            window.draw (line, 2, sf::Lines);
        }
        window.display();
    }
}

int main ()
{
    //Point max = {MAX_X, MAX_Y};
    Plot test (max);

    for (int i = 0; i < 10; i++) //test data
    {
        test.data_[i].x = 10 + i;
        test.data_[i].y = 10 + i;
    }

    for (int i = 10; i <= MAX_VAL; i++) //test data
    {
        test.data_[i].x = 20 + i;
        test.data_[i].y = 10 + i;
    }

    test.PlotMaker ();

    /*for (int i = 0; i < 30; i++)
    {
        printf ("data_[%d].x = %lg || data_[%d].y = %lg\n",
             i, test.data_[i].x, i, test.data_[i].y);
    }*/

    return 0;
}

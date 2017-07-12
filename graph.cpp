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
    sf::RenderWindow window (sf::VideoMode (800, 600), "Plot");

    bool indicator = true;

    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
                window.close ();
        }

        if (indicator)
        {
            window.clear (sf::Color::White);

            for (int i = 0; i < MAX_VAL; i++)
            {
                sf::Vertex line [] =
                {
                    sf::Vertex (sf::Vector2f
                        (data_[i].x,   data_[i].y)),
                    sf::Vertex (sf::Vector2f
                        (data_[i+1].x, data_[i+1].y))
                };

                window.draw (line, 2, sf::Lines);
            }
            indicator = false;
            window.display();
        }
    }
}

int main ()
{
    Point max = {MAX_X, MAX_Y};
    Plot test (max);
    test.PlotMaker ();

    for (int i = 0; i < 10; i++)
    {
        printf ("data_[%d].x = %lg || data_[%d].y = %lg\n",
             i, test.data_[i].x, i, test.data_[i].y);
    }

    return 0;
}

#ifndef GRAPH_HPP
#define GRAPH_HPP

enum
{
    SIZE_OF_DATA = 1024,
    SIZE_OF_NAME = 16,
    MAX_VAL      = 32,
    MAX_X        = 16,
    MAX_Y        = 16
};

typedef struct Point
{
    double x, y;
} Point;

class Plot
{
    public:

        Point max_;
        Point data_ [SIZE_OF_DATA];
        char  name_ [SIZE_OF_NAME];

        Plot (Point max);
        ~Plot ();
        void PlotMaker ();
};

#endif

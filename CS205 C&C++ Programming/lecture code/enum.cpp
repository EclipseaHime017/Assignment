#include <iostream>

using namespace std;

int main(){
    enum color {WHITE, BLACK, RED, GREEN, BLUE, YELLOW, NUM_COLORS};
    enum color pen_color = RED;
    pen_color = color(3);

    cout << "We have " << NUM_COLORS << " pens." << endl;

    int color_index = pen_color;
    color_index += 1;
    cout << "color_index = " << color_index << endl;
}
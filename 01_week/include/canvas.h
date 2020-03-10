# pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum State {
    None, ForegroundChoice, BackgroundChoice, DrawLine, DrawRectangle,
    DrawRectangleFull, DrawCircle, WriteFile, ReadFile
};

class Canvas : public sf::Drawable {
    private:
        State state; // Currently pressed keyboard key choice
        bool mouse_pressed; // Flag that allows to draw
        sf::Vector2f starting_point; // The place where the mouse was pressed

        sf::RectangleShape* rectangle; // The shape that will be drawn
        sf::CircleShape* circle; // The shape that will be drawn
        sf::Vertex line[2];

        sf::Color selected_color;

        // Variable in which the current screen is saved when drawing a new shape
        sf::View current_view; void set_shape_size(sf::Vector2f cursor_position);

    public:
        Canvas() : state(State::None) { selected_color = sf::Color(255, 255, 255);}
        inline void set_state(State new_state) { state = new_state; }
        inline bool is_mouse_pressed() { return mouse_pressed; }
        void mouse_press(sf::RenderWindow, sf::Vector2f);
        void draw_shape(sf::RenderWindow, sf::Event::MouseMoveEvent);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

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
        sf::Vertex* line;

        sf::Color selected_color;

        // Variable in which the current screen is saved when drawing a new shape
        sf::View current_view;
        void set_shape_size(sf::Vector2f cursor_position);

    public:
        Canvas();
        inline void set_state(State new_state) { state = new_state; }
        inline void set_view(sf::View view) { current_view = view; }
        inline void stop_drawing() { mouse_pressed = false; }
        inline sf::View get_view() { return current_view; }
        inline bool is_mouse_pressed() { return mouse_pressed; }
        void mouse_press(sf::Event::MouseButtonEvent);
        void draw_shape(sf::Event::MouseMoveEvent);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};

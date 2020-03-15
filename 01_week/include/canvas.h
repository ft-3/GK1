# pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <deque>

enum State {
    ForegroundChoice, BackgroundChoice, DrawLine, DrawRectangle,
    DrawRectangleFull, DrawCircle, WriteFile, ReadFile
};

class Canvas : public sf::Drawable {
    private:
        std::deque<sf::Shape*> shapes;
        std::deque<std::array<sf::Vertex, 2>> lines;
        State state; // Currently pressed keyboard key choice
        sf::Vector2f start_position; // The place where the mouse was pressed

        sf::Color selected_color_fill;
        sf::Color selected_color_outline;

        void set_shape_size(sf::Vector2f cursor_position);

    public:
        bool draw_flag; // Flag that allows to draw
        bool choose_color_flag;
        Canvas();
        inline void set_state(State new_state) {
            state = new_state;
        }
        inline void stop_drawing() {
            draw_flag = false;
        }
        inline bool is_draw_flag() {
            return draw_flag;
        }
        inline bool is_choose_color_flag() {
            return choose_color_flag;
        }
        void start_draw(sf::Event::MouseButtonEvent);
        void draw_shape(sf::Event::MouseMoveEvent);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
        void choose_color(sf::Event::MouseButtonEvent);
};

inline float calculate_distance(sf::Vector2f starting_position, sf::Vector2f cursor_position);

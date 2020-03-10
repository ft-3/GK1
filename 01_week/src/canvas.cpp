#include "include/canvas.h"
#include <cmath>

inline float calculate_distance(sf::Vector2f starting_position, sf::Vector2f cursor_position) {
    return std::sqrt(std::pow(std::abs(starting_position.x - cursor_position.x), 2) +
                     std::pow(std::abs(starting_position.y - cursor_position.y), 2));
}

void Canvas::set_shape_size(sf::Vector2f cursor_position) {
    //TODO: line
    switch (state) {
        case DrawRectangle:
        case DrawRectangleFull:
            rectangle->setSize(cursor_position - starting_point);
            break;
        case DrawCircle:
            circle->setRadius(calculate_distance(starting_point, cursor_position));
            break;
        default:
            break;
    }

}


void Canvas::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(10.f, 10.f)),
        sf::Vertex(sf::Vector2f(150.f, 150.f))
    };
    circle->setFillColor(sf::Color::Transparent);
    circle->setOutlineThickness(5.f);
    circle->setOutlineColor(sf::Color(155, 144, 133));

    target.draw(line, 2, sf::Lines);
}

// TODO: Save the current window in a buffer MAYBE DONE
void Canvas::mouse_press(sf::RenderWindow window, sf::Vector2f starting_position) {
    mouse_pressed = true;
    starting_point = starting_position;
    current_view = window.getView();
    switch (state) {
        case DrawLine:
            break;
        // TODO: Handle drawing with the currently selected color
        case DrawRectangle:
            rectangle = new sf::RectangleShape;
            rectangle->setPosition(starting_point);
            rectangle->setFillColor(sf::Color::Transparent);
            rectangle->setSize(sf::Vector2f(0.f, 0.f));
            rectangle->setOutlineThickness(5.f);
            rectangle->setOutlineColor(selected_color);
            break;
        case DrawRectangleFull:
            rectangle = new sf::RectangleShape;
            rectangle->setPosition(starting_point);
            rectangle->setSize(sf::Vector2f(0.f, 0.f));
            rectangle->setFillColor(selected_color);
            rectangle->setOutlineThickness(0.f);
            break;
        case DrawCircle:
            circle = new sf::CircleShape(0.f);
            circle->setPosition(starting_point);
            circle->setFillColor(selected_color);
            circle->setOutlineThickness(0.f);
            break;
        default:
            break;
    }
}

void Canvas::draw_shape(sf::RenderWindow window, sf::Event::MouseMoveEvent event) {
   window.setView(current_view); 
   sf::Vector2f pos(event.mouseMove.x, event.MouseMove.y);
   set_shape_size(pos);
   window.draw(shape);

}

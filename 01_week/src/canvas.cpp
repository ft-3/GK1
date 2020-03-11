#include "canvas.h"
#include <cmath>


Canvas::Canvas() : state(State::DrawRectangleFull), mouse_pressed(false),
                    rectangle(nullptr), circle(nullptr), line(nullptr) {
    selected_color = sf::Color(255, 255, 255);
}

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
            circle = nullptr;
            line = nullptr;
            break;
        case DrawCircle:
            circle->setRadius(calculate_distance(starting_point, cursor_position));
            rectangle = nullptr;
            line = nullptr;
            break;
        default:
            break;
    }
}


void Canvas::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (circle)
        target.draw(*circle);
    if (rectangle)
        target.draw(*rectangle);
}

// TODO: Save the current window in a buffer MAYBE DONE
void Canvas::mouse_press(sf::Event::MouseButtonEvent event) {
    mouse_pressed = true;
    //mouse_pressed = !mouse_pressed;
    starting_point = sf::Vector2f(event.x, event.y);
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

void Canvas::draw_shape(sf::Event::MouseMoveEvent event) {
   sf::Vector2f pos(event.x, event.y);
   set_shape_size(pos);
}

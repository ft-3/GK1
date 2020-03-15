#include "canvas.h"
#include <cmath>

#include <iostream>

Canvas::Canvas() : state(State::DrawRectangleFull), draw_flag(false), choose_color_flag(false) {
    selected_color_fill = sf::Color(255, 255, 255);
    selected_color_outline = sf::Color(255, 255, 255);
    shapes = std::deque<sf::Shape*>(0);
    lines = std::deque<std::array<sf::Vertex, 2>>(0);
}

void Canvas::set_shape_size(sf::Vector2f cursor_position) {
    switch (state) {
        case DrawRectangle:
        case DrawRectangleFull:
            if (!shapes.empty()) {
                sf::RectangleShape* last = (sf::RectangleShape*) shapes.front();
                last->setSize(cursor_position - start_position);
            }
            break;
        case DrawCircle:
            if (!shapes.empty()) {
                sf::CircleShape* last = (sf::CircleShape*) shapes.front();
                last->setRadius(calculate_distance(start_position, cursor_position));
            }
            break;
        case DrawLine:
            if (!lines.empty()) {
                lines.front()[1].position = cursor_position;
                break;
            }
        default:
            break;
    }
}
void Canvas::choose_color(sf::Event::MouseButtonEvent event) {
    auto x = event.x;
    auto y = event.y;
    sf::Color choice;
    if (y <= 30) {
        if (x <= 255) choice = sf::Color(x, 255, 0);
        else if (x <= 510) choice = sf::Color(255, 255-(x % 256), 0);
        else if (x <= 255*3) choice = sf::Color(255, 0, (x % 256));
    } else if (y <=60) {
        if (x <= 255) choice = sf::Color(0, 255, 255-(254 - x));
        else if (x <= 510) choice = sf::Color(0, ((254 - x) % 256), 255);
        else if (x <= 255*3) choice = sf::Color(255-((254 - x) % 256), 0, 255);
    }
    if (state == State::ForegroundChoice) selected_color_outline = choice;
    else if (state == State::BackgroundChoice) selected_color_fill = choice;
}


void Canvas::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& shape : shapes) {
        target.draw(*shape);
    }
    for (auto& line : lines) {
        sf::Vertex final_line[2] = {line[0], line[1]};
        target.draw(final_line, 2, sf::Lines);
    }
}

void Canvas::start_draw(sf::Event::MouseButtonEvent event) {
    draw_flag = true;
    choose_color_flag = false;
    start_position = sf::Vector2f(event.x, event.y);
    sf::Shape* shape;
    std::array<sf::Vertex, 2> line = { sf::Vertex(start_position, selected_color_fill), sf::Vertex(start_position, selected_color_fill) };
    switch (state) {
        case DrawLine:
            lines.push_front(line);
            break;
        case DrawRectangle:
            shape = new sf::RectangleShape(sf::Vector2f(0.f, 0.f));
            shape->setPosition(start_position);
            shape->setFillColor(sf::Color::Transparent);
            shape->setOutlineThickness(1.f);
            shape->setOutlineColor(selected_color_outline);
            shapes.push_front(shape);
            break;
        case DrawRectangleFull:
            shape = new sf::RectangleShape(sf::Vector2f(0.f, 0.f));
            shape->setPosition(start_position);
            shape->setFillColor(selected_color_fill);
            shape->setOutlineThickness(0.f);
            shapes.push_front(shape);
            break;
        case DrawCircle:
            shape = new sf::CircleShape(0.f);
            shape->setPosition(start_position);
            shape->setFillColor(selected_color_fill);
            shape->setOutlineThickness(0.f);
            shapes.push_front(shape);
            break;
        default:
            break;
    }
}

void Canvas::draw_shape(sf::Event::MouseMoveEvent event) {

    if (event.y > 597) event.y = 597;
    else if (event.y < 63) event.y = 63;
    sf::Vector2f pos(event.x, event.y);
    set_shape_size(pos);
}

inline float calculate_distance(sf::Vector2f starting_position, sf::Vector2f cursor_position) {
    return std::sqrt(std::pow(std::abs(starting_position.x - cursor_position.x), 2) +
                     std::pow(std::abs(starting_position.y - cursor_position.y), 2));
}


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "menu.h"
#include "canvas.h"
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 01",
                            sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    Menu menu;
    Canvas canvas;

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        while (window.pollEvent(event))
        {
            // Handling inputs
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::F:
                        canvas.set_state(State::ForegroundChoice);
                        canvas.draw_flag = false;
                        canvas.choose_color_flag = true;
                        break;
                    case sf::Keyboard::B:
                        canvas.set_state(State::BackgroundChoice);
                        canvas.draw_flag = false;
                        canvas.choose_color_flag = true;
                        break;
                    case sf::Keyboard::L:
                        canvas.set_state(State::DrawLine);
                        canvas.draw_flag = false;
                        canvas.choose_color_flag = false;
                        break;
                    case sf::Keyboard::R:
                        canvas.set_state(State::DrawRectangle);
                        canvas.draw_flag = false;
                        canvas.choose_color_flag = false;
                        break;
                    case sf::Keyboard::A:
                        canvas.set_state(State::DrawRectangleFull);
                        canvas.draw_flag = false;
                        canvas.choose_color_flag = false;
                        break;
                    case sf::Keyboard::C:
                        canvas.set_state(State::DrawCircle);
                        canvas.draw_flag = false;
                        canvas.choose_color_flag = false;
                        break;
                    case sf::Keyboard::W:
                        break;
                    case sf::Keyboard::O:
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    default:
                        break;
                    }
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!canvas.draw_flag && !canvas.choose_color_flag) {
                        canvas.start_draw(event.mouseButton);
                    }
                    else if (canvas.choose_color_flag) {
                        canvas.choose_color(event.mouseButton);
                    }
                }
                break;
            case sf::Event::MouseMoved:
                if (canvas.is_draw_flag()) {
                    canvas.draw_shape(event.mouseMove);
                }
                break;
            case sf::Event::MouseButtonReleased:
                canvas.stop_drawing();
                break;

            default:
                break;
        }

    }
   //Draw BEGIN
   window.draw(menu);
   window.draw(canvas);
   window.display();
   //Draw END
  }
 return 0;
}

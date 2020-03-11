#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "menu.h"
#include "canvas.h"

//#include "include/menu.h"
//#include "include/canvas.h"

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
                        menu.outtextxy(window,600, 600, L"F");
                        canvas.set_state(State::ForegroundChoice);
                        break;
                    case sf::Keyboard::B:
                        menu.outtextxy(window,600, 600, L"B");
                        canvas.set_state(State::BackgroundChoice);
                        break;
                    case sf::Keyboard::L:
                        menu.outtextxy(window,600, 600, L"L");
                        canvas.set_state(State::DrawLine);
                        break;
                    case sf::Keyboard::R:
                        menu.outtextxy(window,600, 600, L"R");
                        canvas.set_state(State::DrawRectangle);
                        break;
                    case sf::Keyboard::A:
                        menu.outtextxy(window,600, 600, L"A");
                        canvas.set_state(State::DrawRectangleFull);
                        break;
                    case sf::Keyboard::C:
                        menu.outtextxy(window,600, 600, L"C");
                        canvas.set_state(State::DrawCircle);
                        break;
                    case sf::Keyboard::W:
                        menu.outtextxy(window,600, 600, L"W");
                        break;
                    case sf::Keyboard::O:
                        menu.outtextxy(window,600, 600, L"O");
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
                    if (!canvas.is_mouse_pressed()) {
                        canvas.set_view(window.getView());
                        canvas.mouse_press(event.mouseButton);
                    }
                }
                break;
            case sf::Event::MouseMoved:
                if (canvas.is_mouse_pressed()) {
                    window.setView(canvas.get_view());
                    canvas.draw_shape(event.mouseMove);
                }
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

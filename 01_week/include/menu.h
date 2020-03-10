# pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Menu : public sf::Drawable
{
private:
    sf::Font font;
    sf::Text *text;
    sf::RectangleShape *rectangle;

    sf::Texture *colors_texture;
    sf::Sprite *colors_sprite;
    sf::Uint8 *colors_pixels;
    const unsigned int colors_size_x = 765;
    const unsigned int colors_size_y = 60;
    inline void draw_to_color_pixels(unsigned int x, unsigned int y,
                                    unsigned char r, unsigned char g,
                                    unsigned char b) {
    colors_pixels[4 * (y * colors_size_x + x) + 0] = r;
    colors_pixels[4 * (y * colors_size_x + x) + 1] = g;
    colors_pixels[4 * (y * colors_size_x + x) + 2] = b;
    colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
    }

public:
    Menu();

    void outtextxy(sf::RenderTarget& target, float x, float y, const wchar_t *str) const {
        text->setPosition(x, y); 
        text->setString(str); 
        target.draw(*text);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

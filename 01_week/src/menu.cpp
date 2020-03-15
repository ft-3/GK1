#include "menu.h"
#include "font.h"

Menu::Menu() {
    font.loadFromMemory(font_data, font_data_size);
    text = new sf::Text;
    text->setFont(font);
    text->setCharacterSize(12);
    text->setFillColor(sf::Color::White);

    rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
    rectangle->setFillColor(sf::Color::Transparent);
    rectangle->setOutlineColor(sf::Color::White);
    rectangle->setOutlineThickness(1.0f);
    rectangle->setPosition(0, 62);

    unsigned int x, y;
    colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
    for (x = 0; x<255; x++)
    for (y = 0; y < 30; y++) {
        draw_to_color_pixels(x, y, x, 255, 0);
        draw_to_color_pixels(x+255, y, 255, 255-x, 0);
        draw_to_color_pixels(x + 510, y, 255, 0, x);
        draw_to_color_pixels(254 - x, y+30, 0, 255, 255-x);
        draw_to_color_pixels(509 - x, y + 30, 0, x, 255 );
        draw_to_color_pixels(764 - x, y + 30, 255-x, 0, 255);
        }

    colors_texture = new sf::Texture();
    colors_texture->create(colors_size_x, colors_size_y);
    colors_texture->update(colors_pixels);

    colors_sprite = new sf::Sprite();
    colors_sprite->setTexture(*colors_texture);
    colors_sprite->setPosition(0, 0);
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    outtextxy(target, 5, 600, L"f - wybor koloru rysowania");
    outtextxy(target, 5, 615, L"b - wybor koloru wypelniania");
    outtextxy(target, 5, 630, L"l - rysowanie linii");

    outtextxy(target, 200, 600, L"r - rysowanie prostokata");
    outtextxy(target, 200, 615, L"a - rysowanie wypelnionego prostokata");
    outtextxy(target, 200, 630, L"c - rysowanie okregu");

    outtextxy(target, 470, 600, L"w - zapis do pliku");
    outtextxy(target, 470, 615, L"o - odczyt z pliku");
    outtextxy(target, 470, 630, L"esc - wyjscie");

    outtextxy(target, 650, 615, L"Aktualne:");

    target.draw(*rectangle);
    target.draw(*colors_sprite);
}

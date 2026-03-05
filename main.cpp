// Sample program main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Image.h"
#include "Button.h"
using namespace std;
using namespace sf;

void loadImages(vector<MyImage*> &images);

const int imgW = 640, imgH = 450;
int main()
{

    int selectedImage = 0;
    vector<MyImage*> images;
    loadImages(images);
    RenderWindow window(VideoMode(Vector2u(1000, 800)), "SFML works!");
    CircleShape shape(100.f);
    MyImage *img = new MyImage("../Images/boxes_1.ppm");



    Font font("../ARIAL.TTF");

   vector<Button*> buttons;

// ---- Layout settings ----
int buttonX = 700;          // move column a bit right
int buttonY = 15;           // top start
int buttonW = 260;          // wider looks much better
int buttonH = 35;           // slightly taller
int gap  = 10;           // spacing between buttons

auto addButton= [&](const string& text, Handler h)
{
    buttons.push_back(new Button(font, {buttonX, buttonY}, {(float)buttonW, (float)buttonH}, text, h));
    buttonY += buttonH + gap;
};

// Buttons in a vertical column
addButton("Reload",        [&images, &selectedImage](){ images[selectedImage]->reload(); });
addButton("Only Red",      [&images, &selectedImage](){ images[selectedImage]->filterRed(); });
addButton("Only Green",    [&images, &selectedImage](){ images[selectedImage]->filterGreen(); });
addButton("Only Blue",     [&images, &selectedImage](){ images[selectedImage]->filterBlue(); });

addButton("Grey Scale",    [&images, &selectedImage](){ images[selectedImage]->greyScale(); });
addButton("Flip Horizontal",[&images, &selectedImage](){ images[selectedImage]->flipHorizontal(); });
addButton("Flip Vertical", [&images, &selectedImage](){ images[selectedImage]->flipVertical(); });

addButton("Rotate 90",     [&images, &selectedImage](){ images[selectedImage]->advancedFeature1(); });
addButton("Gaussian Blur", [&images, &selectedImage](){ images[selectedImage]->advancedFeature2(); });
addButton("Sobel Edge Detection",[&images, &selectedImage](){ images[selectedImage]->advancedFeature3(); });
addButton("Cartoon Filter",[&images, &selectedImage](){ images[selectedImage]->advancedFeatureExtra(); });

addButton("Save",          [&images, &selectedImage](){ images[selectedImage]->save(); });

// Navigation buttons (keep under image)
buttons.push_back(new Button(font, {250, imgH + 40}, {60, 35}, "<",
    [&images, &selectedImage](){ if (selectedImage > 0) selectedImage--; }));

buttons.push_back(new Button(font, {330, imgH + 40}, {60, 35}, ">",
    [&images, &selectedImage](){ if (selectedImage < (int)images.size() - 1) selectedImage++; }));


    while (window.isOpen())
    {
        optional<Event> event;
        while ((event = window.pollEvent()))
        {
            if (event->is<Event::Closed>()) // Check if the event is a window close event
            {
                window.close();
                return 0;
            }
            if (auto evt = event->getIf<Event::MouseButtonPressed>()) {
                Vector2i pos = evt->position;
                for (Button *btn: buttons) {
                    if (btn->contains(pos)) {
                        btn->click();
                        break;
                    }
                }

            }

        }
        window.clear();

        window.draw(*(images[selectedImage]));
        for (Button *btn: buttons) {
            window.draw(*btn);
        }
        window.display();
    }

    return 0;
}

void loadImages(vector<MyImage*> &images) {
    namespace stdfs = std::filesystem ;


    // http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
    const stdfs::directory_iterator end{} ;

    for( stdfs::directory_iterator iter{"../Images"} ; iter != end ; ++iter )
    {
        // http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file
        if( stdfs::is_regular_file(*iter) ) // comment out if all names (names of directories tc.) are required
        {
            MyImage *img = new MyImage(iter->path().string());
            if (img->getSize().x > 0 && img->getSize().y >0) {
                img->setTargetSize({imgW, imgH});
                img->setPosition({3,3});
                images.push_back(img);

            }
        }
    }


}

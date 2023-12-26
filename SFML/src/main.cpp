#include <iostream>
#include <memory>
#include <fstream>
#include <cstdint>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
// #include "imgui_stdlib.h"

namespace A1
{
    // Holds an object that will be displayed to the window alongside additional
    // properties used to act on the objects display parameters
    constexpr float COLOR_RANGE{ 255 };
    typedef struct RenderObj
    {
        sf::Shape* shape_;
        std::string name_;
        float color_[3];
        float xSpeed_;
        float ySpeed_;
        float scale_;
        bool draw_;

        // Circle Constructor
        RenderObj( float& radius, const uint32_t& segments, std::string& name,
                   float& xSpeed, float& ySpeed, float& xPos, float& yPos, 
                   uint8_t& red, uint8_t& green, uint8_t& blue )
                   {
                        name_ = name ;
                        color_[0] = ( red / COLOR_RANGE );
                        color_[1] = ( green / COLOR_RANGE );
                        color_[2] = ( blue / COLOR_RANGE );
                        xSpeed_ = xSpeed;
                        ySpeed_ = ySpeed;
                        scale_ = 1.0f;
                        draw_ = true;
                        shape_ = new sf::CircleShape( radius, segments );
                        shape_->setPosition( xPos, yPos );
                        shape_->setFillColor( sf::Color(color_[0] * 255, color_[1] * 255, color_[2] * 255) );
                   };

        // Rectangle constructor
        RenderObj( float& width, float& height, std::string& name,
                   float& xSpeed, float& ySpeed, float& xPos, float& yPos, 
                   uint8_t& red, uint8_t& green, uint8_t& blue )
                   {
                        name_ = name ;
                        color_[0] = ( red / COLOR_RANGE );
                        color_[1] = ( green / COLOR_RANGE );
                        color_[2] = ( blue / COLOR_RANGE );
                        xSpeed_ = xSpeed;
                        ySpeed_ = ySpeed;
                        scale_ = 1.0f;
                        draw_ = true;
                        shape_ = new sf::RectangleShape( sf::Vector2f(width, height) );
                        shape_->setPosition( xPos, yPos );
                        shape_->setFillColor( sf::Color(red, green, blue) );
                   };

        // Destructor
        ~RenderObj()
        {
            delete shape_;
        };

    } RenderObj;

    constexpr int32_t OK{ 1 };
    // Window parameter have to have initial size if we want the window to open even if config doesn't specify anything
    uint32_t windowWidth{ 1920 };
    uint32_t windowHeight{ 1080 };
    std::unordered_map<std::string, RenderObj> objMap;
}

int32_t setup( void )
{
    std::ifstream fin( "./config.txt" );
    if ( !fin.is_open() )
    {
        std::cerr << "Error: Config file not found!\n";
        return -1;
    }

    std::string id;
    std::string name;
    float width;
    float height;
    float radius;
    float xPos;
    float yPos;
    float xSpeed;
    float ySpeed;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    constexpr uint32_t segments{ 32u };

    while( fin >> id )
    {
        if( "Window" == id )
        {
            fin >> A1::windowWidth;
            fin >> A1::windowHeight;
        }
        else if( "Font" == id )
        {
            // Don't care about font file just going to hardcode the one font I added
            fin >> id; // File
            fin >> id; // Size
            fin >> id; // Red
            fin >> id; // Green
            fin >> id; // Blue
        }
        else if( "Circle" == id )
        {
            fin >> name;
            fin >> xPos;
            fin >> yPos;
            fin >> xSpeed;
            fin >> ySpeed;
            fin >> red;
            fin >> green;
            fin >> blue;
            fin >> radius;

            A1::objMap.insert( std::map< std::string, A1::RenderObj >::value_type (
                   name, 
                   A1::RenderObj(radius, segments, name, xSpeed, ySpeed, xPos, yPos,  red, green, blue)) );
        }
        else if( "Rectangle" == id )
        {
            fin >> name;
            fin >> xPos;
            fin >> yPos;
            fin >> xSpeed;
            fin >> ySpeed;
            fin >> red;
            fin >> green;
            fin >> blue;
            fin >> width;
            fin >> height;

            A1::objMap.insert( std::map< std::string, A1::RenderObj >::value_type (
                   name, 
                   A1::RenderObj(width, height, name, xSpeed, ySpeed, xPos, yPos, red, green, blue)) );
        }
        else
        {
            // Could safely deal with undesired lines here but going to simple return error;
            std::cerr << "Error: Unknown id found in config!\n";
            return -1;
        }
    }

    return A1::OK;
}

int32_t main( int argc, char* argv[] )
{
    if ( A1::OK != setup() )
    {
        exit(-1);
    }
    std::cout << "post setup\n";

    sf::RenderWindow window( sf::VideoMode(A1::windowWidth, A1::windowHeight), "SFML works!" );
    window.setFramerateLimit( 60u );

    // initialize IMGUI and create a clock used for its internal timing
    ImGui::SFML::Init( window );
    sf::Clock deltaClock;

    // scale the imgui ui by a given factor, does not affect text size
    ImGui::GetStyle().ScaleAllSizes( 1.0f );

    // the imgui color {r,g,b} wheel requires floats from 0-1 instead of ints from 0-255
    float c[3] = { 0.0f, 1.0f, 1.0f };

    // let's make a shape that we will draw to the screen
    float circleRadius = 50;    // radius to draw the circle
    int circleSegments = 32;    // number of segments to draw the circle with
    float circleSpeedX = 1.0f;    // we will use this to move the circle later
    float circleSpeedY = 0.5f;    // you will read these values from the file
    bool drawCircle = true;    // whether or not to drwa the circle
    bool drawText = true;    // whether or not to draw the text

    // create the sfml circle shape based on our parameters
    sf::CircleShape circle( circleRadius, circleSegments );   // create a circle shape with radius 50
    circle.setPosition( 10.0f, 10.0f );   // set the top-left position of the circle

    // let's load a font so we can display some text
    sf::Font myFont;
    std::cout << "pre font\n";

    // attempt to load the font from a file
    if( !myFont.loadFromFile("FreeMono.ttf") )
    {
        // if we can't load the font, exit with error.
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
    std::cout << "post font\n";

    // set up a character array to set the text
    char displayString[255] = "Sample Text"; // TODO: This needs to be removed

    // main loop - continues for each frame while window is open
    while( window.isOpen() )
    {
        // event handling
        sf::Event event;
        while( window.pollEvent(event) )
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent( window, event );

            // this event triggers when the window is closed
            if( sf::Event::Closed == event.type )
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if( sf::Event::KeyPressed == event.type )
            {
                // print the key that was pressed to the console
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                // example, what happens when x is pressed
                if( sf::Keyboard::X == event.key.code )
                {
                    // reverse the x direction of the circle on the screen
                    circleSpeedX *= -1.0f;
                }
            }
        }

        // update imgui for this frame with the time that the last frame took
        ImGui::SFML::Update( window, deltaClock.restart() );

        // draw the UI
        ImGui::Begin( "Window title" );
        ImGui::Text( "Window text!" );
        ImGui::Checkbox( "Draw Circle", &drawCircle );
        ImGui::SameLine();
        ImGui::Checkbox( "Draw Text", &drawText );
        ImGui::SliderFloat( "Radius", &circleRadius, 0.0f, 300.0f );
        ImGui::SliderInt( "Sides", &circleSegments, 3, 64 );
        ImGui::ColorEdit3( "Color Circle", c );
        ImGui::InputText( "Shape Name", displayString, 255 );
        if ( ImGui::Button("Reset Circle") )
        {
            circle.setPosition( 0, 0 );
        }
        ImGui::End();

        // set the circle properties because the may have been updated with the ui
        circle.setFillColor( sf::Color(c[0]*255, c[1]*255, c[2]*255) );
        circle.setPointCount( circleSegments );
        circle.setRadius( circleRadius );
        // basic animation - move the each frame if it's still in teh frame
        circle.setPosition( circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY );

        // basic rendering function calls
        window.clear(); // clear the window of anything previously drawn
        if( drawCircle ) // draw the circle if the boolean is true
        {
            window.draw( circle );
        }

        ImGui::SFML::Render( window );    // draw the ui last so it's on top

        window.display();   // call the window display function
    }

    return 0;
}
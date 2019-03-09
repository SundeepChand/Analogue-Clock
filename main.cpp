#include <SFML/Graphics.hpp>
#include <ctime>

//Define screen dimensions.
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

//Define some colors.
#define DARKGRAY sf::Color(94, 97, 111)
#define CHROMEYELLOW sf::Color(249, 230, 98)
#define DARKCHROMEYELLOW sf::Color(243, 169, 16, 100)
#define BROWN sf::Color(128, 70, 19)

class Time
{
public:
    int hour, minute, second;
    void setTime(tm time);
};

void Time::setTime(tm time)
{
    hour = time.tm_hour;
    hour = hour>12 ? hour-12 : hour;
    minute = time.tm_min;
    second = time.tm_sec;
}

class Stopwatch
{
private:
    int clockRadius;
    float secondAngle, minuteAngle, hourAngle;
    Time initial_time;
    sf::CircleShape watchFace, cap;
    sf::RectangleShape secondHand, minuteHand, hourHand;
    sf::Texture watchFaceTexture, secondHandTexture, minuteHandTexture, hourHandTexture;
    sf::RenderWindow *window;
private:
    void renderNumbers();
public:
    Stopwatch(sf::RenderWindow *window);
    void update();
};

Stopwatch::Stopwatch(sf::RenderWindow *window)
{
    this->window = window;
    //Create the watch face
    clockRadius = 150;
    watchFace.setRadius(clockRadius);
    watchFace.setFillColor(sf::Color::White);
    watchFace.setOrigin(watchFace.getLocalBounds().width/2, watchFace.getLocalBounds().height/2);
    watchFace.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    watchFace.setOutlineColor(BROWN);
    watchFace.setOutlineThickness(10);

    //Set the cap
    cap.setRadius(10);
    cap.setOrigin(cap.getLocalBounds().width/2, cap.getLocalBounds().height/2);
    cap.setPosition(sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
    cap.setFillColor(DARKGRAY);
    cap.setOutlineThickness(4);
    cap.setOutlineColor(sf::Color::Black);

    //Render the letters
    renderNumbers();

    //Set the clock hands.
    if(!secondHandTexture.loadFromFile("img/second_hand.png") || !minuteHandTexture.loadFromFile("img/minute_hand.png") || !hourHandTexture.loadFromFile("img/hour_hand.png"))
    {
        exit(0);
    }

    //Get the current time.
    time_t t = time(NULL);
    tm localTime = *localtime(&t);
    initial_time.setTime(localTime);

    //Set second hand
    secondHandTexture.setSmooth(true);
    secondAngle = -90 + 6*initial_time.second;
    secondHand.setSize(sf::Vector2f(clockRadius-50, 8));
    secondHand.setTexture(&secondHandTexture);
    secondHand.setOrigin(sf::Vector2f(0, secondHand.getLocalBounds().height/2));
    secondHand.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    secondHand.setRotation(secondAngle);

    //Set minute hand
    minuteHandTexture.setSmooth(true);
    minuteAngle = -90 + 6*initial_time.minute;
    minuteHand.setSize(sf::Vector2f(clockRadius-30, 12));
    minuteHand.setTexture(&minuteHandTexture);
    minuteHand.setOrigin(sf::Vector2f(0, minuteHand.getLocalBounds().height/2));
    minuteHand.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    minuteHand.setRotation(minuteAngle);

    //Set hour hand
    hourHandTexture.setSmooth(true);
    hourAngle = -90 + 30*initial_time.hour + (minuteAngle+90)/12;
    hourHand.setSize(sf::Vector2f(clockRadius-60, 15));
    hourHand.setTexture(&hourHandTexture);
    hourHand.setOrigin(sf::Vector2f(0, hourHand.getLocalBounds().height/2));
    hourHand.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    hourHand.setRotation(hourAngle);
}

void Stopwatch::renderNumbers()
{
    if(watchFaceTexture.loadFromFile("img/clock_face.png"))
    {
        watchFaceTexture.setSmooth(true);
        watchFace.setTexture(&watchFaceTexture);
    }
    else
    {
        exit(0);
    }
}

void Stopwatch::update()
{
    //Update the time.
    time_t t = time(NULL);
    tm localTime = *localtime(&t);
    initial_time.setTime(localTime);
    secondAngle = -90 + 6*initial_time.second;
    minuteAngle = -90 + 6*initial_time.minute;
    hourAngle = -90 + 30*initial_time.hour + (minuteAngle+90)/12;
    secondHand.setRotation(secondAngle);
    minuteHand.setRotation(minuteAngle);
    hourHand.setRotation(hourAngle);

    //Draw the objects.
    window->draw(watchFace);
    window->draw(hourHand);
    window->draw(minuteHand);
    window->draw(secondHand);
    window->draw(cap);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Analogue Clock", sf::Style::Close);
    sf::Image img;
    if(!img.loadFromFile("img/icon1.png"))
    {
        exit(0);
    }
    window.setIcon(img.getSize().x, img.getSize().y, img.getPixelsPtr());

    //Background
    sf::Vertex backGround[5] =
    {
        sf::Vertex(sf::Vector2f(0, 0), CHROMEYELLOW),
        sf::Vertex(sf::Vector2f(SCREEN_WIDTH, 0), DARKCHROMEYELLOW),
        sf::Vertex(sf::Vector2f(0, SCREEN_HEIGHT), CHROMEYELLOW),
        sf::Vertex(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT), DARKCHROMEYELLOW)
    };

    //Create the stopwatch object.
    Stopwatch watch(&window);

    //Main game loop.
    while (window.isOpen())
    {
        sf::Event event;
        //Main event loop.
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        window.clear(CHROMEYELLOW);
        window.draw(backGround, 4, sf::Quads);
        watch.update();
        window.display();
        window.setFramerateLimit(60);
    }

    return EXIT_SUCCESS;
}

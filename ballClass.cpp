#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

const int WINDOW_WIDTH = 1232;
const int WINDOW_HEIGHT = 663;
const int BALL_RADIUS = 15;
const float BALL_SPEED = 5.0f;
const float MAX_POWER = 100.0f;
const float FRICTION = 0.98f;
const float CUE_STICK_LENGTH = 100.0f;

const sf::FloatRect TABLE_BOUNDS(50.0f, 50.0f, WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT - 100.0f);
const sf::Vector2f TABLE_SIZE(TABLE_BOUNDS.width, TABLE_BOUNDS.height);

class Ball : public sf::CircleShape
{
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;

public:
    Ball(float radius, const sf::Vector2f& ballPosition, float ballMass)
        : sf::CircleShape(radius), position(ballPosition), mass(ballMass)
    {
        setPosition(position);
    }

    void update()
    {
        position += velocity;
        setPosition(position);

        velocity *= FRICTION; // Apply friction

        if (std::abs(velocity.x) < 0.01f && std::abs(velocity.y) < 0.01f) // Stop ball if velocity is small enough
            velocity = sf::Vector2f(0.0f, 0.0f);
    }

    void set_Velocity(const sf::Vector2f& newVelocity)
    {
        velocity = newVelocity;
    }

    sf::Vector2f get_Velocity() const
    {
        return velocity;
    }

    float get_InverseMass() const
    {
        if (mass != 0.0f)
            return 1.0f / mass;
        else
            return 0.0f;
    }
};

class CueBall : public Ball
{
public:
    CueBall(const sf::Vector2f& ballPosition, float ballMass)
        : Ball(BALL_RADIUS, ballPosition, ballMass)
    {
        setFillColor(sf::Color::White);
    }
};

class PlainBall : public Ball
{
public:
    PlainBall(float radius, const sf::Vector2f& ballPosition, float ballMass)
    : Ball(radius, ballPosition, ballMass)
    {
    setFillColor(sf::Color::Blue);
    }
};


class StripedBall : public Ball
{
public:
    StripedBall(float radius, const sf::Vector2f& ballPosition, float ballMass)
    : Ball(radius, ballPosition, ballMass)
    {
    setFillColor(sf::Color::Yellow);
    }
};


class BlackBall : public Ball
{
public:
    BlackBall(float radius, const sf::Vector2f& ballPosition, const sf::Vector2f& ballVelocity, float ballMass)
    : Ball(radius, ballPosition, ballMass)
{
    setFillColor(sf::Color::Black);
    set_Velocity(ballVelocity);
}
};


float dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

bool checkBallCollision(const Ball& ball1, const Ball& ball2)
{
    sf::Vector2f distance = ball2.getPosition() - ball1.getPosition();
    float magnitudeSquared = distance.x * distance.x + distance.y * distance.y;
    float combinedRadius = 2 * BALL_RADIUS;

    return magnitudeSquared <= combinedRadius * combinedRadius;
}

void resolveBallCollision(Ball& ball1, Ball& ball2)
{
    sf::Vector2f normal = ball2.getPosition() - ball1.getPosition();
    float distance = std::sqrt(normal.x * normal.x + normal.y * normal.y);
    float overlap = 2 * BALL_RADIUS - distance;

    if (overlap > 0)
    {
        sf::Vector2f unitNormal = normal / distance;
        sf::Vector2f relativeVelocity = ball2.get_Velocity() - ball1.get_Velocity();
        float velocityAlongNormal = dot(relativeVelocity, unitNormal);

        if (velocityAlongNormal > 0)
            return;

        float impulseScalar = (-(1 + FRICTION) * velocityAlongNormal) / (ball1.get_InverseMass() + ball2.get_InverseMass());

        sf::Vector2f impulse = impulseScalar * unitNormal;

        ball1.set_Velocity(ball1.get_Velocity() - ball1.get_InverseMass() * impulse);
        ball2.set_Velocity(ball2.get_Velocity() + ball2.get_InverseMass() * impulse);

        sf::Vector2f correction = (overlap / (ball1.get_InverseMass() + ball2.get_InverseMass())) * 0.5f * unitNormal;

        ball1.setPosition(ball1.getPosition() - ball1.get_InverseMass() * correction);
        ball2.setPosition(ball2.getPosition() + ball2.get_InverseMass() * correction);
    }
}

void updateBalls(std::vector<Ball*>& balls)
{
    for (Ball* ball : balls)
        ball->update();

    for (std::size_t i = 0; i < balls.size(); ++i)
    {
        for (std::size_t j = i + 1; j < balls.size(); ++j)
        {
            if (checkBallCollision(*balls[i], *balls[j]))
                resolveBallCollision(*balls[i], *balls[j]);
        }
    }
}

void drawBalls(sf::RenderWindow& window, const std::vector<Ball*>& balls)
{
    for (const Ball* ball : balls)
        window.draw(*ball);
}

void handleCollisionsWithTable(Ball& ball)
{
    sf::Vector2f position = ball.getPosition();
    sf::Vector2f velocity = ball.get_Velocity();

    if (position.x - BALL_RADIUS < TABLE_BOUNDS.left) // Left
    {
        position.x = TABLE_BOUNDS.left + BALL_RADIUS;
        velocity.x = -velocity.x;
    }
    else if (position.x + BALL_RADIUS > TABLE_BOUNDS.left + TABLE_BOUNDS.width) // Right
    {
        position.x = TABLE_BOUNDS.left + TABLE_BOUNDS.width - BALL_RADIUS;
        velocity.x = -velocity.x;
    }

    if (position.y - BALL_RADIUS < TABLE_BOUNDS.top) // Top
    {
        position.y = TABLE_BOUNDS.top + BALL_RADIUS;
        velocity.y = -velocity.y;
    }
    else if (position.y + BALL_RADIUS > TABLE_BOUNDS.top + TABLE_BOUNDS.height) // Bottom
    {
        position.y = TABLE_BOUNDS.top + TABLE_BOUNDS.height - BALL_RADIUS;
        velocity.y = -velocity.y;
    }

    ball.setPosition(position);
    ball.set_Velocity(velocity);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pool Game");

    CueBall cueBall(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f), 1.0f);

    std::vector<Ball*> balls;

balls.push_back(&cueBall);

// Generate the isosceles triangle pattern
// Create stripe balls and plain balls
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(428.f,331.5f), 1.0f)); // First row: Plain Ball
    
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(398.f,316.5f), 1.0f)); // Second row: Plain Ball
    balls.push_back(new StripedBall(BALL_RADIUS, sf::Vector2f(398.f,346.5f), 1.0f)); // Second row: Stripe Ball
    
    balls.push_back(new StripedBall(BALL_RADIUS, sf::Vector2f(368.f,301.5), 1.0f)); // Third row: Stripe Ball
    balls.push_back(new BlackBall(BALL_RADIUS, sf::Vector2f(368.f,331.5),sf::Vector2f(0.f, 0.f),1.0f)); // Third row: Black Ball
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(368.f,361.5f), 1.0f)); // Third row: Plain Ball
    
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(338.f,286.5f), 1.0f)); // Fourth row: Plain Ball
    balls.push_back(new StripedBall(BALL_RADIUS, sf::Vector2f(338.f,316.5f), 1.0f)); // Fourth row: Striped Ball
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(338.f,346.5), 1.0f)); // Fourth row: Plain Ball
    balls.push_back(new StripedBall(BALL_RADIUS, sf::Vector2f(338.f,376.5f), 1.0f)); // Fourth row: Striped Ball
   
    balls.push_back(new StripedBall(BALL_RADIUS, sf::Vector2f(308.f, 271.5f), 1.0f)); // Fifth row: Striped Ball
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(308.f, 301.5f), 1.0f)); // Fifth row: Plain Ball
    balls.push_back(new StripedBall(BALL_RADIUS, sf::Vector2f(308.f, 331.5f), 1.0f)); // Fifth row: Striped Ball
    balls.push_back(new PlainBall(BALL_RADIUS, sf::Vector2f(308.f, 361.5f), 1.0f)); // Fifth row: Plain Ball
    balls.push_back(new StripedBall(BALL_RADIUS , sf::Vector2f(308.f, 391.5f), 1.0f));// Fifth row: Striped Ball

    
    bool isCueBallShot = false;
    sf::Vector2f cueStickPosition;
    sf::Vector2f cueStickDirection;
    
   while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && !isCueBallShot)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    cueStickPosition = cueBall.getPosition();
                    cueStickDirection = sf::Vector2f(event.mouseButton.x, event.mouseButton.y) - cueStickPosition;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && !isCueBallShot)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isCueBallShot = true;

                    float power = std::min(MAX_POWER, std::sqrt(cueStickDirection.x * cueStickDirection.x + cueStickDirection.y * cueStickDirection.y));
                    sf::Vector2f cueBallVelocity = -power * cueStickDirection / std::sqrt(cueStickDirection.x * cueStickDirection.x + cueStickDirection.y * cueStickDirection.y);

                    cueBall.set_Velocity(cueBallVelocity);
                }
            }
        }

        if (!isCueBallShot)
        {
            cueStickPosition = cueBall.getPosition();
            cueStickDirection = sf::Vector2f(sf::Mouse::getPosition(window)) - cueStickPosition;

            float cueStickLength = std::min(CUE_STICK_LENGTH, std::sqrt(cueStickDirection.x * cueStickDirection.x + cueStickDirection.y * cueStickDirection.y));
            cueStickDirection = cueStickLength * cueStickDirection / std::sqrt(cueStickDirection.x * cueStickDirection.x + cueStickDirection.y * cueStickDirection.y);
        }

        cueBall.setPosition(cueBall.getPosition() + cueBall.get_Velocity());
        handleCollisionsWithTable(cueBall);

        if (isCueBallShot)
        {

       for (Ball* ball : balls)
            handleCollisionsWithTable(*ball);
            
            updateBalls(balls);
        
        }
        
        window.clear(sf::Color::Green);
        drawBalls(window, balls);

        if (!isCueBallShot)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(cueStickPosition),
                sf::Vertex(cueStickPosition + cueStickDirection)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }

    for (Ball* ball : balls)
        delete ball;

    return 0;

}
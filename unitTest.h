#include <iostream>
#include <cassert>
#include <cmath>
#include <ball.h>

const int BALL_RADIUS = 20;
const float FRICTION = 0.98f;

float dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

bool isAlmostEqual(float a, float b, float epsilon = 0.001f) {
    return std::abs(a - b) < epsilon;
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
        sf::Vector2f relativeVelocity = ball2.getVelocity() - ball1.getVelocity();
        float velocityAlongNormal = dot(relativeVelocity, unitNormal);

        if (velocityAlongNormal > 0)
            return;

        float impulseScalar = (-(1 + FRICTION) * velocityAlongNormal) / (ball1.getInverseMass() + ball2.getInverseMass());

        sf::Vector2f impulse = impulseScalar * unitNormal;

        ball1.setVelocity(ball1.getVelocity() - ball1.getInverseMass() * impulse);
        ball2.setVelocity(ball2.getVelocity() + ball2.getInverseMass() * impulse);

        sf::Vector2f correction = (overlap / (ball1.getInverseMass() + ball2.getInverseMass())) * 0.5f * unitNormal;

        ball1.setPosition(ball1.getPosition() - ball1.getInverseMass() * correction);
        ball2.setPosition(ball2.getPosition() + ball2.getInverseMass() * correction);
    }
}
// Test the Ball class
void testBall() {
    // Create a ball
    sf::Vector2f position(100.0f, 200.0f);
    float radius = 15.0f;
    float mass = 1.0f;
    Ball ball(radius, position, mass);

    // Test initial position and velocity
    assert(ball.getPosition() == position);
    assert(ball.getVelocity() == sf::Vector2f(0.0f, 0.0f));

    // Test setting and getting velocity
    sf::Vector2f velocity(5.0f, -3.0f);
    ball.setVelocity(velocity);
    assert(ball.getVelocity() == velocity);

    // Test inverse mass
    assert(isAlmostEqual(ball.getInverseMass(), 1.0f / mass));

    // Test ball update
    sf::Vector2f expectedPosition = position + velocity;
    ball.update();
    assert(ball.getPosition() == expectedPosition);
}

// Test the collision functions
void testCollision() {
    // Test ball collision detection
    sf::Vector2f position1(100.0f, 200.0f);
    sf::Vector2f position2(150.0f, 200.0f);
    float radius = 15.0f;
    float mass = 1.0f;
    Ball ball1(radius, position1, mass);
    Ball ball2(radius, position2, mass);

    assert(checkBallCollision(ball1, ball2));

    // Test ball collision resolution
    sf::Vector2f initialVelocity1(2.0f, 3.0f);
    sf::Vector2f initialVelocity2(-1.0f, -2.0f);
    ball1.setVelocity(initialVelocity1);
    ball2.setVelocity(initialVelocity2);

    resolveBallCollision(ball1, ball2);

    // After collision, velocities should be swapped
    assert(ball1.getVelocity() == sf::Vector2f(-initialVelocity1.x, initialVelocity1.y));
    assert(ball2.getVelocity() == sf::Vector2f(-initialVelocity2.x, initialVelocity2.y));
}


#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>

sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Game", sf::Style::Close);

// Player
sf::RectangleShape player(sf::Vector2f(100.0f, 100.0f));
sf::RectangleShape playerAttack(sf::Vector2f(100.0f, 10.0f));
int attackCounter = 0;

// Opponent
sf::RectangleShape opponent(sf::Vector2f(100.0f, 100.0f));
sf::RectangleShape opponentAttack(sf::Vector2f(100.0f, 10.0f));
int attackCounterOpponent = 0;

double accumulator = 0.0;

// Game loop related

void endGame() {
    window.close();
}

void renderScene() {
    window.clear();
    window.draw(playerAttack);
    window.draw(opponentAttack);
    window.draw(player);
    window.draw(opponent);
    window.display();
}

// Online 

// {left, right, attack} might add more if needed
std::array<int,3> recieveInputs() {
    std::array<int, 3> recieved = { 0,0,1 };
    return recieved;
}

void sendInputs() {

}

// Collision Logic

bool AABBCollision(const sf::RectangleShape& a, const sf::RectangleShape& b) {
    sf::FloatRect aBounds = a.getGlobalBounds();
    sf::FloatRect bBounds = b.getGlobalBounds();

    return aBounds.position.x < bBounds.position.x + bBounds.size.x &&
        aBounds.position.x + aBounds.size.x > bBounds.position.x &&
        aBounds.position.y < bBounds.position.y + bBounds.size.y &&
        aBounds.position.y + aBounds.size.y > bBounds.position.y;
}

void checkCollisions() {
    if (AABBCollision(player, opponent)) {
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2f opponentPos = opponent.getPosition();
        
        if (playerPos.x >= 50.0f) {
            player.setPosition(sf::Vector2f(playerPos.x - 10.0f, playerPos.y));
        }
        if (opponentPos.x <= 1230.0f) {
            opponent.setPosition(sf::Vector2f(opponentPos.x + 10.0f, opponentPos.y));
        }
    }

    if (attackCounter > 40 && AABBCollision(playerAttack, opponent)) {
        //printf("Player Attack colides with Opponent\n");
        endGame();
    }
    if (attackCounter > 40 && attackCounterOpponent > 40 && AABBCollision(playerAttack, opponentAttack)) {
        //printf("Player Attack colides with Opponent Attack\n");
        endGame();
    }
    if (attackCounterOpponent > 40 && AABBCollision(opponentAttack, player)) {
        //printf("Opponent Attack colides with Player\n");
        endGame();
    }
    if (attackCounterOpponent > 40 && attackCounter > 40 && AABBCollision(opponentAttack, playerAttack)) {
        //printf("Opponent Attack colides with Player Attack\n");
        endGame();
    }
}

// Updating Players

void updatePlayer() {
    sf::Vector2f playerPosition = player.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && playerPosition.x >= 50.0f) {
        player.move(sf::Vector2f(-10.0f, 0.0f));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && playerPosition.x >= 50.0f) {
        player.move(sf::Vector2f(10.0f, 0.0f));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && playerPosition.x <= 1230.0f) {
        playerAttack.setPosition(sf::Vector2f(playerPosition.x + 50.0f, 450.f));
        if (attackCounter <= 0) {
            attackCounter = 60;
        }
    }

    if (attackCounter <= 40) {
        playerAttack.setPosition(sf::Vector2f(playerPosition.x - 50.0f, 450.f));
        attackCounter--;
    }
    else {
        playerAttack.setPosition(sf::Vector2f(playerPosition.x + 50.0f, 450.f));
        attackCounter--;
    }
}

void updateOpponent(std::array<int, 3> inputs) {
    sf::Vector2f opponentPosition = opponent.getPosition();

    //if (inputs[0] == 1 && opponentPosition.x >= 50.0f) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && opponentPosition.x >= 50.0f) {
        opponent.move(sf::Vector2f(-10.0f, 0.0f));
    }

    //if (inputs[1] == 1  && opponentPosition.x <= 1230.0f) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && opponentPosition.x >= 50.0f) {
        opponent.move(sf::Vector2f(10.0f, 0.0f));
    }

    //if (inputs[2] == 1 && opponentPosition.x <= 1230.0f) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && opponentPosition.x >= 50.0f) {
        opponentAttack.setPosition(sf::Vector2f(opponentPosition.x + 50.0f, 450.f));
        if (attackCounterOpponent <= 0) {
            attackCounterOpponent = 60;
        }
    }

    if (attackCounterOpponent <= 40) {
        opponentAttack.setPosition(sf::Vector2f(opponentPosition.x + 50.0f, 450.f));
        attackCounterOpponent--;
    }
    else {
        opponentAttack.setPosition(sf::Vector2f(opponentPosition.x - 50.0f, 450.f));
        attackCounterOpponent--;
    }
}

// Game Loop

int main()
{
    sf::Clock clock;
    opponent.setFillColor(sf::Color::Red);
    opponentAttack.setFillColor(sf::Color::Red);

    player.setOrigin(sf::Vector2f(50.0f, 50.0f));
    playerAttack.setOrigin(sf::Vector2f(0.0f, 50.0f));
    player.move(sf::Vector2f(80.0f, 410.0f));
    playerAttack.move(sf::Vector2f(30.0f, 450.0f));

    opponent.setOrigin(sf::Vector2f(50.0f, 50.0f));
    opponentAttack.setOrigin(sf::Vector2f(100.0f, 50.0f));
    opponent.move(sf::Vector2f(900.0f, 410.0f));
    opponentAttack.move(sf::Vector2f(1250.0f, 450.0f));
    
    while (window.isOpen())
    {
        double deltaTime = clock.restart().asSeconds();
        accumulator += deltaTime;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        
        while (accumulator > 1.0 / 60.0) {
            updatePlayer();
            updateOpponent(recieveInputs());

            checkCollisions();

            accumulator -= 1.0 / 60.0;
        }

        renderScene();
    }

    return 0;
}
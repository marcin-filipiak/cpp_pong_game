#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  // Dodanie obsługi dźwięku
#include <iostream>

int main() {
    // Stworzenie okna gry
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Game");

    // Załadowanie tła
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/background.png")) {
        std::cerr << "Nie można załadować tła!" << std::endl;
        return -1;
    }

    sf::Sprite background;
    background.setTexture(backgroundTexture);

    // Ustawienia dla paletki (poruszającej się lewo-prawo na dole)
    sf::RectangleShape paddle(sf::Vector2f(100, 10));  // Paletka: szerokość 100, wysokość 10
    paddle.setPosition(350, 560);  // Początkowa pozycja paletki (na dole)
    paddle.setFillColor(sf::Color::White);

    // Załadowanie tekstury piłki
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/images/ball.png")) {
        std::cerr << "Nie można załadować tekstury piłki!" << std::endl;
        return -1;
    }

    // Ustawienia dla piłki (z obrazkiem)
    sf::Sprite ball;
    ball.setTexture(ballTexture);
    ball.setPosition(400, 300);  // Początkowa pozycja piłki

    // Ustawienia dla tekstu (licznik uderzeń)
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Nie można załadować czcionki!" << std::endl;
        return -1;
    }

    sf::Text hitCountText;
    hitCountText.setFont(font);
    hitCountText.setCharacterSize(24);  // Rozmiar tekstu
    hitCountText.setFillColor(sf::Color::White);  // Kolor tekstu
    hitCountText.setPosition(700, 10);  // Pozycja tekstu na ekranie

    // Licznik uderzeń piłki w paletkę
    int hitCount = 0;

    // Prędkość ruchu piłki i paletki
    float paddleSpeed = 1.0f;
    sf::Vector2f ballSpeed(0.5f, 0.5f);  // Początkowa prędkość piłki

    // Załaduj dźwięk z pliku .wav dla trafienia w paletkę
    sf::SoundBuffer hitBuffer;
    if (!hitBuffer.loadFromFile("assets/sounds/winning-a-coin.wav")) {
        std::cerr << "Nie można załadować pliku dźwiękowego winning-a-coin.wav!" << std::endl;
        return -1;
    }

    sf::Sound hitSound;
    hitSound.setBuffer(hitBuffer);

    // Załaduj dźwięk z pliku .wav dla trafienia w dolną ścianę (reset punktów)
    sf::SoundBuffer resetBuffer;
    if (!resetBuffer.loadFromFile("assets/sounds/funny-cartoon-fast-splat.wav")) {
        std::cerr << "Nie można załadować pliku dźwiękowego funny-cartoon-fast-splat.wav!" << std::endl;
        return -1;
    }

    sf::Sound resetSound;
    resetSound.setBuffer(resetBuffer);

    // Główna pętla gry
    while (window.isOpen()) {
        // Sprawdzanie zdarzeń (np. zamknięcie okna)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Ruch paletki (prawo/lewo)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 0) {
            paddle.move(-paddleSpeed, 0);  // Ruch w lewo
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x + paddle.getSize().x < window.getSize().x) {
            paddle.move(paddleSpeed, 0);  // Ruch w prawo
        }

        // Ruch piłki
        ball.move(ballSpeed);

        // Odbicie piłki od górnej i bocznych ścianek
        if (ball.getPosition().y <= 0) {
            ballSpeed.y = -ballSpeed.y;  // Zmiana kierunku piłki po osi Y (góra)
        }
        if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getGlobalBounds().width >= window.getSize().x) {
            ballSpeed.x = -ballSpeed.x;  // Zmiana kierunku piłki po osi X (boczne ścianki)
        }

        // Sprawdzenie, czy piłka uderza w dolną ścianę
        if (ball.getPosition().y + ball.getGlobalBounds().height >= window.getSize().y) {
            ballSpeed.y = -ballSpeed.y;  // Odbicie piłki
            hitCount = 0;  // Resetowanie licznika punktów
            resetSound.play();  // Odtworzenie dźwięku resetowania
        }

        // Odbicie piłki od paletki i naliczanie punktów
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballSpeed.y = -ballSpeed.y;  // Zmiana kierunku piłki po osi Y
            hitCount++;  // Zwiększamy licznik uderzeń po każdym uderzeniu w paletkę

            // Odtwarzanie dźwięku po uderzeniu
            hitSound.play();
        }

        // Aktualizacja tekstu licznika
        hitCountText.setString("Hits: " + std::to_string(hitCount));

        // Czyszczenie okna i rysowanie tła
        window.clear();
        window.draw(background);

        // Rysowanie paletki, piłki i tekstu
        window.draw(paddle);
        window.draw(ball);
        window.draw(hitCountText);

        // Wyświetlanie zmian
        window.display();
    }

    return 0;
}


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  // Dodanie obsługi dźwięku
#include <iostream>

int main() {
    // Stworzenie okna gry
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Game");

    // Ustawienia dla paletki
    sf::RectangleShape paddle(sf::Vector2f(10, 100));  // Paletka: szerokość 10, wysokość 100
    paddle.setPosition(50, 250);  // Początkowa pozycja paletki
    paddle.setFillColor(sf::Color::White);

    // Ustawienia dla piłki
    sf::CircleShape ball(10.f);  // Piłka o promieniu 10 pikseli
    ball.setPosition(400, 300);  // Początkowa pozycja piłki
    ball.setFillColor(sf::Color::White);

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

    // Załaduj dźwięk z pliku .wav dla trafienia w lewą ścianę
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

        // Ruch paletki (góra/dół)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle.getPosition().y > 0) {
            paddle.move(0, -paddleSpeed);  // Ruch w górę
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddle.getPosition().y + paddle.getSize().y < window.getSize().y) {
            paddle.move(0, paddleSpeed);  // Ruch w dół
        }

        // Ruch piłki
        ball.move(ballSpeed);

        // Odbicie piłki od górnej i dolnej ściany
        if (ball.getPosition().y <= 0 || ball.getPosition().y + ball.getRadius() * 2 >= window.getSize().y) {
            ballSpeed.y = -ballSpeed.y;  // Zmiana kierunku piłki po osi Y
        }

        // Sprawdzenie, czy piłka uderza w lewą ścianę
        if (ball.getPosition().x <= 0) {
            ballSpeed.x = -ballSpeed.x;  // Zmiana kierunku piłki po osi X
            hitCount = 0;  // Resetowanie licznika punktów
            resetSound.play();  // Odtworzenie dźwięku resetowania
        }
        // Odbicie piłki od prawej ściany
        else if (ball.getPosition().x + ball.getRadius() * 2 >= window.getSize().x) {
            ballSpeed.x = -ballSpeed.x;  // Zmiana kierunku piłki po osi X
        }

        // Odbicie piłki od paletki
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            // Sprawdzanie, czy piłka uderzyła w paletkę od prawej strony
            if (ball.getPosition().x <= paddle.getPosition().x + paddle.getSize().x && ballSpeed.x < 0) {
                ballSpeed.x = -ballSpeed.x;  // Zmiana kierunku piłki po osi X
                hitCount++;  // Zwiększamy licznik uderzeń, tylko jeśli uderzenie było z prawej strony

                // Odtwarzanie dźwięku po uderzeniu
                hitSound.play();
            } else {
                // Nadal odbijamy piłkę, jeśli uderzyła w paletkę, ale nie liczymy punktu
                ballSpeed.x = -ballSpeed.x;
            }
        }

        // Aktualizacja tekstu licznika
        hitCountText.setString("Hits: " + std::to_string(hitCount));

        // Czyszczenie okna
        window.clear();

        // Rysowanie paletki, piłki i tekstu
        window.draw(paddle);
        window.draw(ball);
        window.draw(hitCountText);

        // Wyświetlanie zmian
        window.display();
    }

    return 0;
}


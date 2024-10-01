#!/bin/sh

# apt install libsfml-dev

# Użyj pkg-config, aby uzyskać flagi do kompilacji z SFML
params="`pkg-config --cflags --libs sfml-all`"

# package name from control file
package_name=$(grep 'Package:' control | cut -d' ' -f2)

rm bin/$package_name
clear
echo "-------------Compile for SFML---------------"
echo "--------------------------------------------"
echo "                COMPILATION"
echo "--------------------------------------------"
g++ -o bin/$package_name src/$package_name.cpp $params 2> error_log.txt
# Sprawdzanie, czy plik error_log.txt jest pusty
if [ -s error_log.txt ]; then
  echo "---Errors during compilation---"
  # Wyświetlanie błędów na ekranie z paginacją
  more error_log.txt
  rm error_log.txt
fi  
echo "--------------------------------------------"
echo "             END OF COMPILATION"
echo "--------------------------------------------"

# Sprawdzanie, czy plik skompilowany istnieje, zanim uruchomimy program
if [ -f "bin/$package_name" ]; then
	echo "--------------------------------------------"
	echo "                RUN"
	echo "--------------------------------------------"
	chmod +x bin/$package_name
	cd bin  # Zmiana katalogu na /bin
	./$package_name  # Uruchomienie programu
	cd ..  # Powrót do głównego katalogu po zakończeniu
	echo "--------------------------------------------"
	echo "             END OF RUN"
	echo "--------------------------------------------"
fi


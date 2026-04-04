#include "Car.h"
#include <Windows.h>

int main() {
    SetConsoleOutputCP(65001);

    SportsCar sc;
    sc.setTurbo(true);
    sc.changeGear(3);
    sc.speedUp();
    sc.display();

    return 0;
}
#pragma once
#include "Car.h"

class SportsCar : public Car {
public:
    bool bTurbo;

    SportsCar(int s, const char* n, int g, bool tur) : Car(s, n, g) {
        bTurbo = tur;
    }

    void setTurbo(bool bTur) { bTurbo = bTur; }

    void speedUp() {
        if (bTurbo) speed += 20;
        else Car::speedUp();
    }

    void display() {
        Car::display();
        if (bTurbo) printf("Turbo : ON\n");
        else printf("Turbo : OFF\n");
    }
};
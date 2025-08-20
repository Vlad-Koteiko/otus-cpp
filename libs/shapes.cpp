#include "shapes.hpp"

Circle::Circle(Point center, double radius) : center(center), radius(radius) {}

std::string Circle::getName() const { return "Circle"; }

void Circle::draw() const { std::cout << "draw circle" << std::endl; }

Rectangle::Rectangle(Point topLeft, Point bottomRight)
    : topLeft(topLeft), bottomRight(bottomRight) {}

std::string Rectangle::getName() const { return "Rectangle"; }

void Rectangle::draw() const { std::cout << "draw rectangle" << std::endl; }

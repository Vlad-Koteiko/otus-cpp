#pragma once
#include <string>
#include <iostream>

struct Point {
  double x;
  double y;
};

class Shape {
public:
  virtual std::string getName() const = 0;
  virtual void draw() const = 0;
};

struct Circle final : Shape {

  explicit Circle(Point center, double radius);

  std::string getName() const final;
  void draw() const final;

private:
  Point center;
  double radius;
};

struct Rectangle final : Shape {

  explicit Rectangle(Point topLeft, Point bottomRight);

  std::string getName() const final;
  void draw() const final;

private:
  Point topLeft;
  Point bottomRight;
};

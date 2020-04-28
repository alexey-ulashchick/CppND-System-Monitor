#include "ncurses_display.h"
#include "system.h"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
  System system;
  system.Update();

  NCursesDisplay::Display(system);
}
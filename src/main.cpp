#ifndef MAIN_CPP
#define MAIN_CPP
#include "main.hpp"
#include "include/app.hpp"
#include <chrono>
namespace Origin {
auto main(int argc, char *argv[]) -> int {
  App app = App(argc, argv);
  return (app.loop(seconds(100)));
}
} // namespace Origin
int main(int argc, char *argv[]) { return Origin::main(argc, argv); }
#endif
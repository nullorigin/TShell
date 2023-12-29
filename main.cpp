#ifndef MAIN_CPP
#define MAIN_CPP
#include "main.hpp"
#include "include/rc.hpp"
#include <chrono>
namespace origin {
auto main(int argc, char **argv) -> int {
  App rc = App(argc, argv);
  return (rc.loop(seconds(10)));
}
} // namespace origin
int main(int argc, char *argv[]) { return origin::main(argc, argv); }
#endif
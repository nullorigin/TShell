#ifndef CON_HPP
#define CON_HPP
#include <cstdio>
#include <fcntl.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <urlmon.h>
#include <winbase.h>
#include <winsock.h>

struct Console {
  int Height;
  int Width;
  int BgColor;
  int FgColor;
  /* Console color identifiers*/
  static const int BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4,
                   MAGENTA = 5, BROWN = 6, LIGHTGRAY = 7, DARKGRAY = 8,
                   LIGHTBLUE = 9, LIGHTGREEN = 10, LIGHTCYAN = 11,
                   LIGHTRED = 12, LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15,
                   BLINK = 128;
  Console() {
    Height = 25;
    Width = 80;
    BgColor = BLACK;
    FgColor = WHITE;
  }
  inline auto ClearEOL() -> void { printf("\033[2K"); }
  inline auto InsertLine() -> void { printf("\033[%dA", 1); }
  inline auto GotoXY(int x, int y) { printf("\033[%d;%df", y, x); }

  inline auto ClearScreen() -> void {
    printf("\033[%dm\033[2J\033[1;1f", BgColor);
  }

  inline auto SetBgColor(int color) -> void {
    switch (color % 16) {
    case BLACK:
      BgColor = 40;
      break;
    case BLUE:
      BgColor = 44;
      break;
    case GREEN:
      BgColor = 42;
      break;
    case CYAN:
      BgColor = 46;
      break;
    case RED:
      BgColor = 41;
      break;
    case MAGENTA:
      BgColor = 45;
      break;
    case BROWN:
      BgColor = 43;
      break;
    case LIGHTGRAY:
      BgColor = 47;
      break;
    case DARKGRAY:
      BgColor = 40;
      break;
    case LIGHTBLUE:
      BgColor = 44;
      break;
    case LIGHTGREEN:
      BgColor = 42;
      break;
    case LIGHTCYAN:
      BgColor = 46;
      break;
    case LIGHTRED:
      BgColor = 41;
      break;
    case LIGHTMAGENTA:
      BgColor = 45;
      break;
    case YELLOW:
      BgColor = 43;
      break;
    case WHITE:
      BgColor = 47;
      break;
    }
  }
  inline auto SetFgColor(int color) -> void {
    switch (color % 16) {
    case BLACK:
      FgColor = 30;
      break;
    case BLUE:
      FgColor = 34;
      break;
    case GREEN:
      FgColor = 32;
      break;
    case CYAN:
      FgColor = 36;
      break;
    case RED:
      FgColor = 31;
      break;
    case MAGENTA:
      FgColor = 35;
      break;
    case BROWN:
      FgColor = 33;
      break;
    case LIGHTGRAY:
      FgColor = 37;
      break;
    case DARKGRAY:
      FgColor = 30;
      break;
    case LIGHTBLUE:
      FgColor = 34;
      break;
    case LIGHTGREEN:
      FgColor = 32;
      break;
    case LIGHTCYAN:
      FgColor = 36;
      break;
    case LIGHTRED:
      FgColor = 31;
      break;
    case LIGHTMAGENTA:
      FgColor = 35;
      break;
    case YELLOW:
      FgColor = 33;
      break;
    case WHITE:
      FgColor = 37;
    default:
      break;
    }
  }
  inline auto PrintBgColor(int color) -> void {
    switch (color % 16) {
    case BLACK:
      printf("\033[0;%d;%dm", 30, BgColor);
      break;
    case BLUE:
      printf("\033[0;%d;%dm", 34, BgColor);
      break;
    case GREEN:
      printf("\033[0;%d;%dm", 32, BgColor);
      break;
    case CYAN:
      printf("\033[0;%d;%dm", 36, BgColor);
      break;
    case RED:
      printf("\033[0;%d;%dm", 31, BgColor);
      break;
    case MAGENTA:
      printf("\033[0;%d;%dm", 35, BgColor);
      break;
    case BROWN:
      printf("\033[0;%d;%dm", 33, BgColor);
      break;
    case LIGHTGRAY:
      printf("\033[0;%d;%dm", 37, BgColor);
      break;
    case DARKGRAY:
      printf("\033[1;%d;%dm", 30, BgColor);
      break;
    case LIGHTBLUE:
      printf("\033[1;%d;%dm", 34, BgColor);
      break;
    case LIGHTGREEN:
      printf("\033[1;%d;%dm", 32, BgColor);
      break;
    case LIGHTCYAN:
      printf("\033[1;%d;%dm", 36, BgColor);
      break;
    case LIGHTRED:
      printf("\033[1;%d;%dm", 31, BgColor);
      break;
    case LIGHTMAGENTA:
      printf("\033[1;%d;%dm", 35, BgColor);
      break;
    case YELLOW:
      printf("\033[1;%d;%dm", 33, BgColor);
      break;
    case WHITE:
      printf("\033[1;%d;%dm", 37, BgColor);
      break;
    default:
      break;
    }
  }

  inline auto PrintFgColor(int color) -> void {
    switch (color % 16) {
    case BLACK:
      printf("\033[0;%d;%dm", 30, FgColor);
      break;
    case BLUE:
      printf("\033[0;%d;%dm", 34, FgColor);
      break;
    case GREEN:
      printf("\033[0;%d;%dm", 32, FgColor);
      break;
    case CYAN:
      printf("\033[0;%d;%dm", 36, FgColor);
      break;
    case RED:
      printf("\033[0;%d;%dm", 31, FgColor);
      break;
    case MAGENTA:
      printf("\033[0;%d;%dm", 35, FgColor);
      break;
    case BROWN:
      printf("\033[0;%d;%dm", 33, FgColor);
      break;
    case LIGHTGRAY:
      printf("\033[0;%d;%dm", 37, FgColor);
      break;
    case DARKGRAY:
      printf("\033[1;%d;%dm", 30, FgColor);
      break;
    case LIGHTBLUE:
      printf("\033[1;%d;%dm", 34, FgColor);
      break;
    case LIGHTGREEN:
      printf("\033[1;%d;%dm", 32, FgColor);
      break;
    case LIGHTCYAN:
      printf("\033[1;%d;%dm", 36, FgColor);
      break;
    case LIGHTRED:
      printf("\033[1;%d;%dm", 31, FgColor);
      break;
    case LIGHTMAGENTA:
      printf("\033[1;%d;%dm", 35, FgColor);
      break;
    case YELLOW:
      printf("\033[1;%d;%dm", 33, FgColor);
      break;
    case WHITE:
      printf("\033[1;%d;%dm", 37, FgColor);
      break;
    default:
      break;
    }
  }
  inline auto UngetChar(char ch) -> int { return ungetc(ch, stdin); }
  inline auto GetChar(bool echo = false) -> int {
      int ch = _getch();
	  if (echo)
		  printf("%c", ch);
	  return ch;
  }

  inline auto GetXY(int &x, int &y) -> int {
    printf("\033[6n");
    if (GetChar() != '\x1B') {
      return 0;
    }
    if (GetChar() != '\x5B') {
      return 0;
    }
    int in = 0;
    int ly = 0;
    while ((in = GetChar()) != ';') {
      ly = ly * 10 + in - '0';
    }
    int lx = 0;
    while ((in = GetChar()) != 'R') {
      lx = lx * 10 + in - '0';
    }
    x = lx;
    y = ly;
    return x * y;
  }

  inline auto GetX() -> int {
    int x = 0;
    int y = 0;
    GetXY(x, y);
    return x;
  }

  inline auto GetY() -> int {
    int x = 0;
    int y = 0;
    GetXY(x, y);
    return y;
  }

  inline auto KeyHit() -> int {
	return _kbhit();
  }

  inline auto PrintChar(const char c) -> char {
    printf("%c", c);
    return c;
  }
  inline auto PrintStr(std::string str) -> int {
    printf("%s", str.c_str());
    return 0;
  }
  inline auto PrintStrColor(const std::string &str, int fg, int bg) -> int {
    SetFgColor(fg);
    SetBgColor(bg);
    PrintStr(str);
    SetFgColor(WHITE);
    SetBgColor(BLACK);
    return 0;
  }
  inline auto ChangeColor(int fg, int bg) -> int {
    printf("\033[%d;%dm", fg + 30, bg + 40);
    return 0;
  }
  inline auto ChangeColor(int color) -> int {
    return ChangeColor(color % 16, color / 16);
  }
  inline auto PrintCStr(const char *str) -> int {
    printf("%s", str);
    return 0;
  }
  auto GetHome() -> std::string { return GetEnv("HOME"); }
  auto GetCwd() -> std::string {
      LPSTR buf = LPSTR();
      GetCurrentDirectoryA(1024, buf);
	  return std::string(buf);
  }
  inline auto GetUser() -> std::string {
      LPSTR user = nullptr;
	  LPDWORD size = (LPDWORD)user;
      GetUserNameA(user, size);
	  return std::string(user).substr(0, *size - 1);
  }
  static auto GetHostname() -> std::string {
    LPSTR hostname = LPSTR();
    GetComputerNameA(hostname, LPDWORD(1024));
    return hostname;
  }
  inline auto GetPass(const char *prompt) -> char * {
    char *pass = nullptr;
    printf("%c", *prompt);
    return fgets(pass, 100, stdin);
  }
  inline auto GetEnv(const std::string &name) -> std::string {
    return getenv(name.c_str());
  }
  inline auto ReadText(int l, int t, int r, int b, void *destination)
      -> size_t {
    printf("\033[%d;%d;%d;%dH", b, t, r, l);
    fflush(stdout);
    return fread(destination, 1, l * t * r, stdin);
  }
  inline auto WriteText(int l, int t, int r, int b, void *source) -> size_t {
    printf("\033[%d;%d;%d;%dH", b, t, r, l);
    fflush(stdout);
    return fwrite(source, 1, l * t * r, stdout);
  }
};
#endif

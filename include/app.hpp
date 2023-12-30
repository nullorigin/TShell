#ifndef RC_HPP
#define RC_HPP
#include "console.hpp"
#include "timer.hpp"
#include "util.hpp"
#include <chrono>
#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <thread>
#include <mutex>
namespace origin {
using namespace std;
class App {
private:
  int *p{nullptr};
  mutex *Mutex{nullptr};
  std::string Output{};
  std::string Input{};
  int RunState{};
  long Cycles{};
  long MaxCycles{};
  bool Status{};
  Console *Con{nullptr};
  Timer *TimerArr[8];
  std::string TimerText{};
  std::string ExecText{};
  /* An array of run states in string form, sorted to form a doubly linked list
with their matching int values, and used to retrieve text by index. */
  const std::string RunStates[17] = {
      "Uninitialized", "Initializing", "Initialized", "Starting", "Started",
      "Pausing",       "Paused",       "Resuming",    "Resumed",  "Stopping",
      "Stopped",       "Restarting",   "Restarted",   "Exiting",  "Exited",
      "Killing",       "Killed"};
  /* An list of run states in integer form, intended to form a doubly linked
list with their matching std::string values, and used to retrieve text by index.
*/
  static const int Uninitialized = 0, Initializing = 1, Initialized = 2,
                   Starting = 3, Started = 4, Pausing = 5, Paused = 6,
                   Resuming = 7, Resumed = 8, Stopping = 9, Stopped = 10,
                   Restarting = 11, Restarted = 12, Exiting = 13, Exited = 14,
                   Killing = 15, Killed = 16;
  /* A dependency map of applicable run states to enter into after a state
switch. Uses the constants above as specifiers for the equivalent index. */
  const int RunMap[17][8] = {
      {Uninitialized, Initializing, -1, -1, -1, -1, -1, -1},
      {Initializing, Initialized, Exiting, Killing, -1, -1, -1, -1},
      {Initialized, Starting, Exiting, Killing, -1, -1, -1, -1},
      {Starting, Started, Restarting, Exiting, Killing, -1, -1, -1},
      {Started, Pausing, Stopping, Restarting, Exiting, Killing, -1, -1},
      {Pausing, Paused, Resuming, Restarting, Exiting, Killing, -1, -1},
      {Paused, Resuming, Stopping, Restarting, Exiting, Killing, -1, -1},
      {Resuming, Resumed, Stopping, Restarting, Exiting, Killing - 1, -1, -1},
      {Resumed, Pausing, Stopping, Restarting, Exiting, Killing, -1, -1},
      {Stopping, Stopped, Restarting, Exiting, Killing, -1, -1, -1},
      {Stopped, Starting, Restarting, Exiting, Killing, -1, -1, -1},
      {Restarting, Restarted, Exiting, Killing, -1, -1, -1, -1},
      {Restarted, Restarting, Starting, Exiting, Killing, -1, -1, -1},
      {Exiting, Exited, Killing, -1, -1, -1, -1, -1},
      {Exited, Killing, -1, -1, -1, -1, -1, -1},
      {Killing, Killed, -1, -1, -1, -1, -1, -1},
      {Killed, Uninitialized, -1, -1, -1, -1, -1, -1}};

  const std::string Cmd[16] = {"init",   "1", "start", "2", "pause",   "3",
                               "resume", "4", "stop",  "5", "restart", "6",
                               "exit",   "7", "kill",  "8"};

  const int CmdMap[16] = {1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15};
  const std::string CmdArg[16] = {"", "", "", "", "", "", "", "",
                                  "", "", "", "", "", "", "", ""};
  const int AllTxt = -1, PromptTxt = 0, StateTxt = 1, CycleTxt = 2,
            TimerTxt = 3, ExecTxt = 4;
  inline void NewVar() {
    for (int i = 0; i < 8; i++) {
      TimerArr[i] = new Timer;
    }
    Con = new struct Console;
    Mutex = new mutex;
    p = new int;
  }
  inline void DeleteVar() {
    for (int i = 0; i < 8; i++) {
      delete TimerArr[i];
    }
    delete Con;
    delete Mutex;
    delete p;
  }

public:
  App(int argc, char *argv[]) {
    NewVar();
    *Con = Console();
    *p = 0;
    SetState(Uninitialized);
    Cycles = 0;
    MaxCycles = 1000000000;
  }
  ~App() { DeleteVar(); };

  // The main loop of the application. Splits the output into a separate thread
  // and continues updating the input status until the application is exited or
  // a time limit is reached.
  auto loop(nanoseconds runtime) -> int {
    if (runtime > nanoseconds::zero()) {
      TimerArr[0]->SetLimit(runtime);
    }
    std::thread thread{[this]() { this->ProcessOutput(); }};
    thread.detach();
    while ((GetState() < Exited) &&
           (TimerArr[0]->GetRemaining() > nanoseconds::zero())) {
      ProcessInput();
      ProcessCycles();
    }
    return 0;
  }
  // Sets the run state of the application to 'Initializing' and Upon success,
  // sets the run state to 'Initialized'.
  auto DoInit() -> bool {
    if (RunState == Uninitialized) {
      if (SetState(Initializing)) {
        return SetState(Initialized);
      }
    }
    return false;
  }
  // Sets the run state of the application to 'Starting' and executes specified
  // commands. Upon success,the run state is set to 'Started'.
  auto DoStart() -> bool {
    if (SetState(Starting)) {
      TimerArr[0]->Start();
      return SetState(Started);
    }
    return false;
  }
  // Sets the run state of the application to 'Pausing' and executes specified
  // commands. Upon success, the run state is set to 'Paused'. In addition, the
  // output is frozen and saved until the 'resume' command is executed, at which
  // point it continues from where it left off.
  auto DoPause() -> bool {
    if (SetState(Pausing)) {
      TimerArr[0]->Pause();
      return SetState(Paused);
    }
    return false;
  }
  // Sets the run state of the application to 'Resuming' and executes specified
  // commands. Upon success, the run state is set to 'Resumed'. This command is
  // only valid if the application is in the 'Paused' state. In addition, the
  // output is frozen until this command is called at which point it
  // continues from where it left off.
  auto DoResume() -> bool {
    if (SetState(Resuming)) {
      TimerArr[0]->Resume();
      return SetState(Resumed);
    }
    return false;
  }
  // Sets the run state of the application to 'Stopping' and executes specified
  // commands. Upon success, the run state is set to 'Stopped'. In this state
  // The only applicable commands are 'start', 'restart' and 'exit'.
  auto DoStop() -> bool {
    if (SetState(Stopping)) {
      TimerArr[0]->Stop();
      ResetCycles();
      return SetState(Stopped);
    }

    return false;
  }
  // Sets the run state of the application to 'Restarting' and executes
  // specified commands. Upon success, the run state is set to 'Restarted'. All
  // Variables are reset to their initial values.
  auto DoRestart() -> bool {
    if (SetState(Restarting)) {
      TimerArr[0]->Restart();
      Output.clear();
      Input.clear();
      ResetCycles();
      return SetState(Restarted);
    }
    return false;
  }
  // Sets the run state of the application to 'Exiting' and executes specified
  // commands. Upon failure, the kill command is executed and forces the
  // application to close. Upon success, the run state is set to 'Exited' and
  // exits normally.
  auto DoExit() -> bool {
    if (SetState(Exiting)) {
      if (SetState(Exited)) {
        DoKill();
      }
    }
    return false;
  }
  // Sets the run state of the application to 'Killing' and then 'Killed'. The
  // application is forced to close when this command is executed.
  auto DoKill() -> bool {
    if (SetState(Killing)) {
      SetState(Killed);
      ::exit(-1);
    }
    return false;
  }
  // Returns the entire map of valid run states to enter into during a state
  // switch.
  auto GetRunMap() -> const int (*)[8] { return RunMap; }
  // Returns an array of applicable run states in which to switch to
  // from a given previous state.
  auto GetStateNext(int state) -> const int (*)[8] { return &RunMap[state]; }
  // Returns the string representation of a given run state.
  auto GetStateString(int state) -> std::string { return RunStates[state]; }
  auto GetArg(const std::string &command) -> std::string {
    for (int i = 0; i < 16; i++) {
      if (Cmd[i] == command) {
        return CmdArg[i];
      }
    }
    return "";
  }
  // Finds the index of a given command from its string representation.
  auto GetCmd(const std::string &command) -> int {
    for (int i = 0; i < 16; i++) {
      if (Cmd[i] == command) {
        return CmdMap[i];
      }
    }
    return -1;
  }
  // Returns the string representation of a given command from its index.
  auto GetCmd(int index) -> int { return CmdMap[index]; }
  // Determines if the application is in a state in which it is actually
  // running.
  auto IsRunning() -> bool {
    int const state = GetState();
    return ((state < Paused) && (state > Uninitialized)) ||
           (state < Exited && state >= Resuming && state != Stopped &&
            state != Restarted);
  }
  // Returns true if the application is in the specified state.
  auto Is(int state) const -> bool { return (RunState == state); }
  auto GetState() const -> int { return RunState; }
  auto GetOutput() -> std::string { return Output; }
  auto GetInput() -> std::string { return Input; }
  auto GetCycles() const -> long { return Cycles; }
  auto SetCycles(long cycles) -> void { Cycles = cycles; }
  auto incrementCycles() -> void {
    if (IsRunning()) {
      Cycles++;
    }
    if (Cycles > MaxCycles) {
      DoExit();
    }
  }
  auto GetMaxCycles() const -> long { return MaxCycles; }
  auto SetMaxCycles(long cycles) -> int {
    if (cycles <= 0) {
      MaxCycles = 1;
      return -1;
    }
    MaxCycles = cycles;
    return 0;
  }
  auto ResetCycles() -> void { Cycles = 0; }
  static auto GetThreadId() -> std::thread::id {
    return std::this_thread::get_id();
  }
  auto SetStatus(bool status) -> bool {
    Status = status;
    return Status;
  }
  auto GetText(int name, const std::string &dlim = "\n") -> std::string {
    std::string txt[] = {
        (">-(" + Con->GetUser() + "@" + Console::GetHostname() + ")-$ [" +
         GetInput() + "]" + dlim),
        (" (State)=[" + GetStateString(GetState()) + "]" + dlim),
        (" (Cycles)=[" + ToString(GetCycles()) + "]" + dlim),
        (" (Timer)=[" + ToString(TimerArr[0]->GetElapsed()) + "s]" + dlim),
        (dlim + ExecText + dlim)};
    if (IsRunning()) {
      TimerText = txt[TimerTxt];
    } else if (TimerArr[0]->IsRunning()) {
      txt[TimerTxt] = TimerText;
    }
    std::string txt_out;
    if (name >= 0 && name < 5) {
      txt_out = txt[name];
    } else if (name == AllTxt) {
      for (int i = 0; i < 5; i++) {
        txt_out += txt[i];
      }
    }
    return txt_out;
  }
  auto GetStatus() const -> bool { return Status; }
  auto SetOutput(const std::string output, bool format = false) -> int {
    Output.clear();
    if (format) {
      for (char o : output) {
        if (o != '\0' && o != '\r') {
          Output.push_back(o);
        }
      }
    } else {
      Output = output;
    }
    return 0;
  }
  auto SetInput(const std::string &input, bool format) -> int {
    Input.clear();
    if (format) {
      for (char i : input) {
        if (i != '\0' && i != '\r' && i != (char)8) {
          Input += i;
        }
      }
    } else {
      Input = input;
    }
    return 0;
  }
  auto SetState(int target) -> bool {
    bool status = false;
    int state = GetState();
    int i = 0;
    if (state == target) {
      return SetStatus(status);
    }
    if (target >= 1 && target < 17) {
      while ((RunMap[state][i] != -1) && (i < 8) && (!status)) {
        if (RunMap[state][i] == target) {
          state = target;
          status = true;
        }
        i++;
      }
    }
    RunState = state;
    return SetStatus(status);
  }

private:
  auto ProcessInput() -> int {
    int state = GetState();
    if ((state >= Uninitialized) && (state <= Exited)) {
      std::string in = GetInput();
      if (Con->KeyHit() != 0) {
        int const i = Con->GetChar();
        char ch = static_cast<char>(i);
        if ((ch != '\n') && (ch != '\r') && (ch != '\0')) {
          if (ch == 8 || ch == 127 || ch == 27) {
            in = in.substr(0, in.size() - 1);
          } else {
            in += ch;
          }
          in.shrink_to_fit();
          SetInput(in, true);
        } else {
          state = GetCmd(in);
          if (state != -1) {
            ProcessState(state);
          } else if (!in.empty() || ch == '\n') {
            ExecText = exec(in.c_str());
          }
          SetInput("", false);
        }
      }
      return 0;
    }
    return -1;
  }
  auto ProcessOutput() -> int {
    nanoseconds TimeMax;
    nanoseconds TimeEnd;
    while (true) {
      Mutex->lock();
      TimeMax = (TimerArr[0]->GetNow() + nanoseconds(8333333));
      Con->ClearScreen();
      flush(std::cout);
      flush(std::cerr);
      std::string const out = GetText(AllTxt);
      SetOutput(out, true);
      std::cout << GetOutput() << std::flush;
      TimeEnd = (TimerArr[0]->GetNow());
      Mutex->unlock();
      if (TimeEnd < TimeMax) {
        std::this_thread::sleep_for(TimeMax - TimeEnd);
      }
    }
    return 0;
  }
  auto ProcessCycles() -> int {
    incrementCycles();
    if (GetCycles() >= GetMaxCycles() && GetMaxCycles() != 0) {
      DoExit();
    }
    return 0;
  }
  auto ProcessState(int state) -> int {
    switch (state) {
    case Initializing:
      if (SetStatus(DoInit())) {
        return Initialized;
      }
      break;
    case Starting:
      if (SetStatus(DoStart())) {
        return Started;
      }
      break;
    case Pausing:
      if (SetStatus(DoPause())) {
        return Paused;
      }
      break;
    case Resuming:
      if (SetStatus(DoResume())) {
        return Resumed;
      }
      break;
    case Stopping:
      if (SetStatus(DoStop())) {
        return Stopped;
      }
      break;
    case Restarting:
      if (SetStatus(DoRestart())) {
        return Restarted;
      }
      break;
    case Exiting:
      if (SetStatus(DoExit())) {
        return Exited;
      }
      break;
    case Killing:
      if (SetStatus(DoKill())) {
        return Killed;
      }
      break;
    default:
      break;
    }
    return -1;
  }
  static auto exec(const char *cmd) -> std::string {
    char buffer[128];
    std::string result = "";
    FILE *pipe =  _popen(cmd, "r");
    if (pipe == nullptr) {
      throw std::runtime_error("popen() failed!");
    }
    try {
      while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
        result += buffer;
      }
    } catch (...) {
      _pclose(pipe);
      throw;
    }
    _pclose(pipe);
    return result;
  }
}; // namespace run
} // namespace origin
#endif // RC_HPP
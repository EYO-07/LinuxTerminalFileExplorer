/// BEGIN CODEX_termios.h 
// {TextMarker|red:|blue:}
// {TextMarker|magenta:utf8_to_wstring,wstring_to_utf8,wstring,wcout|cyan:string,cout}

// -- preprocessor directives
#pragma once 
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include <array>
#include <filesystem>
#include <unordered_map>
#include <iconv.h>
#include <stdexcept>
#include <codecvt>
#include <locale>
#include <set>
#include <chrono>
#include <format> // C++20
#include <fstream>
#include <cerrno>

// -- function declarations
bool disableRawMode(termios& settings); // Disable echo and set raw mode
void restoreMode(const termios& settings); // Restore terminal settings
void saveCursorPosition(); // Necessary for clearOutput to know where to start clearing 
void clearOutput();
bool commandOutput(std::string command, std::vector<std::string>& lines);
bool commandOutput(std::string command, std::string& output);
bool changeDirectory(const std::string& path);
//std::vector<std::wstring> split(const std::wstring& input, wchar_t delimiter);
std::vector<std::string> split(const std::string& input, char delimiter);
void openTerminal(const std::filesystem::path& path, const std::string& term);
std::string wstring_to_utf8(const std::wstring& wstr);
std::wstring utf8_to_wstring(const std::string& str);
//std::wstring getTimeStamp(); // ?
std::string str_getTimeStamp(); 
//bool saveFile(std::filesystem::path dest, std::wstring filename, std::wstring content); // ?
bool saveFile(std::filesystem::path dest, std::string filename, std::string content); // ?
std::string createCopyToBashScript(std::set<std::filesystem::path> list, std::filesystem::path dest); // ?
std::string createMoveToBashScript(std::set<std::filesystem::path> list, std::filesystem::path dest); // ?
std::string createDeleteBashScript(std::set<std::filesystem::path> list); // ?

// 
class TerminalExplorer {
public:
    TerminalExplorer();
    std::vector<std::string>& getLines() { return this->lines; }
    void update();
    void up(int step = 1);
    void down(int step = 1);
    void left();
    void right();
    int getIndex(const std::filesystem::path&);
    void setTerminal(std::string term) { this->terminal = term; }
    std::string getTerminal() { return this->terminal; }
    void openTerminal();
    void setGrepArgs(std::string arg) { this->grep_arg = arg; } // UNUSED 
    void selectFile();
    void clearSelectedFiles() { this->selected_files.clear(); }
    void deselectFile();
    bool generateCopyFilesScript(); // ?
    bool generateMoveFilesScript(); // ?
    bool generateDeleteFilesScript(); // ?
    void setUpdateList(bool value) { this->b_update_ls_list = value; }
    void setDropDir(std::filesystem::path dirPath) { this->dropDir = std::filesystem::absolute(dirPath); }
    bool isLineSelectedItem(int index);
    void outChangePath();
    void outKeepPath();
    bool coutCurrentHead();
protected:
    // -- variables 
    std::vector<std::string> lines;
    std::vector<std::string> raw_lines;
    std::filesystem::path currentDir;
    std::filesystem::path dropDir; 
    std::unordered_map<std::filesystem::path, int> path2index;
    int range = 15;
    std::string terminal = "xterm";
    std::string grep_arg = ""; // UNUSED 
    std::vector<std::string> filters; 
    std::set<std::filesystem::path> selected_files; // UNUSED
    bool b_update_ls_list = true;
    // -- methods
    bool updateLsCommand();
    std::filesystem::path getPathFromLine(int index); 
    bool isDir(int index); // !
    void coutSelectedFiles();
};

/// END CODEX_termios.h 





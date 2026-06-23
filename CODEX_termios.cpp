/// BEGIN CODEX_termios.h 

// -- preprocessor directives
#include "CODEX_termios.h"

// -- local functions
// -- implementations 
bool disableRawMode(termios& settings) {
    if (tcgetattr(STDIN_FILENO, &settings) != 0) {
        return false;
    }
    termios raw = settings;
    // Disable canonical mode (line buffering) and echo
    raw.c_lflag &= ~(ICANON | ECHO);
    // Disable signals like Ctrl+C for custom handling if needed
    // raw.c_lflag &= ~(ISIG); 
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &raw) != 0) {
        return false;
    }
    return true;
}
void restoreMode(const termios& settings) {
    tcsetattr(STDIN_FILENO, TCSADRAIN, &settings);
    std::cout << "\n"; // Ensure a newline after exit
}
void saveCursorPosition() {
    // ANSI code: Save cursor position
    std::cout << "\033[s" << std::flush;
}
void clearOutput() {
    // 1. Move cursor to saved position
    std::cout << "\033[u"; 
    // 2. Clear from cursor to end of screen
    std::cout << "\033[J" << std::flush;
}
bool commandOutput(std::wstring command, std::vector<std::wstring>& lines) {
    int pipefd[2];
    if (pipe(pipefd) == -1) return false;
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]); close(pipefd[1]);
        return false;
    }
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        try {
            std::string narrowCommand = wstring_to_utf8(command);
            execl("/bin/sh", "sh", "-c", narrowCommand.c_str(), nullptr);
        } catch(...) {
            // todo 
        }
        _exit(127);
    } else {
        close(pipefd[1]);
        size_t originalSize = lines.size();
        size_t writeIdx = 0;
        std::wstring currentLine;
        char buffer[4096];
        ssize_t bytesRead;
        auto commitLine = [&](std::wstring& line) {
            if (writeIdx < originalSize) {
                if(line.compare(lines[writeIdx])!=0)
                    lines[writeIdx] = std::move(line);
            } else {
                lines.push_back(std::move(line));
            }
            writeIdx++;
            line.clear();
        };
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            for (ssize_t i = 0; i < bytesRead; ++i) {
                if (buffer[i] == '\n') {
                    if (!currentLine.empty() && currentLine.back() == L'\r') {
                        currentLine.pop_back();
                    }
                    commitLine(currentLine);
                } else {
                    unsigned char c = buffer[i];
                    currentLine += static_cast<wchar_t>(c);
                }
            }
        }
        if (!currentLine.empty()) commitLine(currentLine);
        // Fill remaining original slots with empty strings
        int it_count = 0;
        for (size_t i = writeIdx; i < originalSize; ++i) {
            if(it_count>5) break;
            lines[i] = L"";
            it_count++;
        }
        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}
bool changeDirectory(const std::wstring& path) {
    try {
        std::filesystem::current_path(path);
        return true;
    } catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}
std::vector<std::wstring> split(const std::wstring& input, wchar_t delimiter) {
    std::vector<std::wstring> tokens;
    std::wstring current;
    for (wchar_t ch : input) {
        if (ch == delimiter) {
            tokens.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}
std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::string current;
    for (char ch : input) {
        if (ch == delimiter) {
            tokens.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}
void openTerminal(const std::filesystem::path& path, const std::string& term) {
    pid_t pid = fork();
    if (pid == 0) {
        if(term.empty()) {
            execlp("xterm", "xterm", "-e",
                   ("cd " + path.string() + " && bash").c_str(),
                   nullptr);
        } else {
            execlp(term.c_str(), term.c_str(), "-e",
                   ("cd " + path.string() + " && bash").c_str(),
                   nullptr);
        }
        _exit(127); // exec failed
    }
}
std::wstring utf8_to_wstring(const std::string& str) {
    if (str.empty()) return L"";

    // Linux-specific: wchar_t is always 32-bit (UTF-32).
    // We only need to detect Endianness.
    #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        const char* w_encoding = "UTF-32BE";
    #else
        // Default to Little-Endian (covers x86_64, ARM64, RISC-V)
        const char* w_encoding = "UTF-32LE";
    #endif

    iconv_t cd = iconv_open(w_encoding, "UTF-8");
    if (cd == (iconv_t)-1) {
        throw std::runtime_error("iconv_open failed: " + std::string(strerror(errno)));
    }

    const char* in_ptr = str.data();
    size_t in_bytes = str.size();

    // Allocation: UTF-8 (max 4 bytes) -> UTF-32 (4 bytes)
    // Safe upper bound: 1 input byte -> 1 output wchar_t
    size_t out_bytes = (in_bytes + 1) * sizeof(wchar_t); 
    std::vector<char> buffer(out_bytes);
    
    char* out_ptr = buffer.data();
    size_t out_bytes_left = out_bytes;

    size_t res = iconv(cd, const_cast<char**>(&in_ptr), &in_bytes, &out_ptr, &out_bytes_left);
    
    iconv_close(cd);

    if (res == (size_t)-1) {
        throw std::runtime_error("iconv conversion failed: " + std::string(strerror(errno)));
    }

    size_t bytes_written = out_bytes - out_bytes_left;
    size_t wchar_count = bytes_written / sizeof(wchar_t);
    
    return std::wstring(reinterpret_cast<wchar_t*>(buffer.data()), wchar_count);
}      
std::string wstring_to_utf8(const std::wstring& wstr) {
    std::string utf8_line;
    utf8_line.reserve(wstr.size() * 4); 

    for (wchar_t wc : wstr) {
        // Optional: Handle invalid surrogate pairs if porting from Windows logic
        // (Not needed on Linux as wchar_t is 32-bit, but good for safety)
        if (wc >= 0xD800 && wc <= 0xDFFF) {
            // Invalid surrogate in UTF-32 context. 
            // Replace with U+FFFD or skip.
            utf8_line.push_back(static_cast<char>(0xEF));
            utf8_line.push_back(static_cast<char>(0xBF));
            utf8_line.push_back(static_cast<char>(0xBD));
            continue;
        }

        if (wc < 0x80) {
            // 1-byte (ASCII)
            utf8_line.push_back(static_cast<char>(wc));
        } else if (wc < 0x800) {
            // 2-byte
            utf8_line.push_back(static_cast<char>(0xC0 | (wc >> 6)));
            utf8_line.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
        } else if (wc < 0x10000) {
            // 3-byte
            utf8_line.push_back(static_cast<char>(0xE0 | (wc >> 12)));
            utf8_line.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
            utf8_line.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
        } else if (wc <= 0x10FFFF) {
            // 4-byte (VALID Unicode range only)
            utf8_line.push_back(static_cast<char>(0xF0 | (wc >> 18)));
            utf8_line.push_back(static_cast<char>(0x80 | ((wc >> 12) & 0x3F)));
            utf8_line.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
            utf8_line.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
        } else {
            // INVALID code point (> 0x10FFFF). 
            // Replace with U+FFFD (Replacement Character)
            utf8_line.push_back(static_cast<char>(0xEF));
            utf8_line.push_back(static_cast<char>(0xBF));
            utf8_line.push_back(static_cast<char>(0xBD));
        }
    }
    return utf8_line;
}   
std::wstring getTimeStamp() {
    try {
        auto now = std::chrono::system_clock::now();
        auto local_time = std::chrono::current_zone()->to_local(now);
        return std::format(L"{:%Y-%m-%dT%H-%M-%S}", local_time);
    } catch (...) {
        return L"YYYY-MM-DDT00-00-00";
    }
}
std::string str_getTimeStamp() {
    try {
        auto now = std::chrono::system_clock::now();
        auto local_time = std::chrono::current_zone()->to_local(now);
        return std::format("{:%Y-%m-%dT%H-%M-%S}", local_time);
    } catch (...) {
        return "YYYY-MM-DDT00-00-00";
    }
}
bool saveFile(std::filesystem::path dest, std::wstring filename, std::wstring content) {
    if (!dest.is_absolute()) return false;
    if (!std::filesystem::exists(dest)) return false;
    try {
        std::filesystem::path fullPath = dest / filename;
        std::string utf8Content = wstring_to_utf8(content);
        std::ofstream outFile(fullPath, std::ios::out | std::ios::trunc);
        if (!outFile.is_open()) return false;
        outFile << utf8Content;
        if (!outFile.good()) return false;
        outFile.close();
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        // Handle path errors (permissions, invalid chars, etc.)
        return false;
    } catch (...) {
        // Handle any other unexpected errors
        return false;
    }
}
bool saveFile(std::filesystem::path dest, std::wstring filename, std::string content) {
    // Validate destination
    if (!dest.is_absolute()) return false;
    
    // Ensure destination exists and is a directory
    if (!std::filesystem::exists(dest) || !std::filesystem::is_directory(dest)) {
        return false;
    }

    try {
        // Construct full path
        // Note: std::filesystem::path handles mixing wstring filename and path seamlessly on Linux
        std::filesystem::path fullPath = dest / filename;

        // Write directly to file
        // No conversion needed: content is already std::string (UTF-8)
        std::ofstream outFile(fullPath, std::ios::out | std::ios::trunc);

        if (!outFile.is_open()) {
            return false;
        }

        outFile << content;

        // Check for write errors
        if (!outFile.good()) {
            return false;
        }
        
        outFile.close();
        return true;

    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    } catch (...) {
        return false;
    }
}   
std::string bash_escape(const std::string& input) {
    std::string output;
    output.reserve(input.size() + 10); // Reserve to avoid reallocations
    output.push_back('\''); // Start single quote
    
    for (char c : input) {
        if (c == '\'') {
            // End quote, escaped single quote, start quote
            output += "'\\''";
        } else {
            output.push_back(c);
        }
    }
    
    output.push_back('\''); // End single quote
    return output;
}
std::string createCopyToBashScript(std::set<std::filesystem::path> list, std::filesystem::path dest) {
    if (list.empty()) return "";
    if (!dest.is_absolute()) return "";
    if (!std::filesystem::exists(dest)) return "";
    if (!std::filesystem::is_directory(dest)) return "";
    try {
        std::ostringstream script;
        // Write Shebang
        script << "#!/bin/bash\n";
        script << "# Auto-generated copy script\n";
        script << "set -e  # Exit on error\n\n";
        // Write copy commands
        // Using 'cp' with explicit escaping for every path
        for (const auto& filePath : list) {
            if (std::filesystem::exists(filePath)) {
                // Convert paths to native strings and escape them
                std::string srcEscaped = bash_escape(filePath.string());
                std::string destEscaped = bash_escape(dest.string());
                // Check if the source is a directory
                if (std::filesystem::is_directory(filePath)) {
                    // Add -r flag for directories
                    script << "cp -r " << srcEscaped << " " << destEscaped << "\n";
                } else {
                    // Standard copy for files
                    script << "cp " << srcEscaped << " " << destEscaped << "\n";
                }
            }
        }
        return script.str();
    } catch (const std::filesystem::filesystem_error& e) {
        return "";
    } catch (...) {
        return "";
    }
}
std::string createMoveToBashScript(std::set<std::filesystem::path> list, std::filesystem::path dest) {
    if (list.empty()) return "";
    if (!dest.is_absolute()) return "";
    if (!std::filesystem::exists(dest)) return "";
    if (!std::filesystem::is_directory(dest)) return "";
    try {
        std::ostringstream script;
        // Write Shebang
        script << "#!/bin/bash\n";
        script << "# Auto-generated move script\n";
        script << "set -e  # Exit on error\n\n";
        // Write copy commands
        // Using 'cp' with explicit escaping for every path
        for (const auto& filePath : list) {
            if (std::filesystem::exists(filePath)) {
                // Convert paths to native strings and escape them
                std::string srcEscaped = bash_escape(filePath.string());
                std::string destEscaped = bash_escape(dest.string());
                // Generate: mv 'source' 'destination/'
                script << "mv " << srcEscaped << " " << destEscaped << "\n";
            }
        }
        return script.str();
    } catch (const std::filesystem::filesystem_error& e) {
        return "";
    } catch (...) {
        return "";
    }
}
std::string createDeleteBashScript(std::set<std::filesystem::path> list) {
    if (list.empty()) return "";
    try {
        std::ostringstream script;
        // Write Shebang
        script << "#!/bin/bash\n";
        script << "# Auto-generated delete script\n";
        script << "set -e  # Exit on error\n\n";
        for (const auto& filePath : list) {
            if (std::filesystem::exists(filePath)) {
                // Convert paths to native strings and escape them
                std::string srcEscaped = bash_escape(filePath.string());
                // Check if the source is a directory
                if (std::filesystem::is_directory(filePath)) {
                    // Add -r flag for directories
                    script << "rm -r " << srcEscaped << "\n";
                } else {
                    // Standard copy for files
                    script << "rm " << srcEscaped << "\n";
                }
            }
        }
        return script.str();
    } catch (const std::filesystem::filesystem_error& e) {
        return "";
    } catch (...) {
        return "";
    }
}

// -- classes
TerminalExplorer::TerminalExplorer() {
    this->updateLsCommand();
    this->currentDir = std::filesystem::current_path();
    this->path2index[this->currentDir] = 0;
}
int TerminalExplorer::getIndex(const std::filesystem::path& dirPath) { 
    if ( this->path2index.count(dirPath) == 0 ) return 0;
    if ( this->path2index[dirPath] < 0 ) {
        this->path2index[dirPath] = 0;
    }
    return this->path2index[dirPath];
}
void TerminalExplorer::update() { 
    std::wcout << std::endl;
    std::wcout << L"Curr Dir :   " << this->currentDir << std::endl;
    std::wcout << L"Drop Dir :   " << this->dropDir << std::endl;
    std::wcout << L"Terminal :   " << utf8_to_wstring(this->terminal) << std::endl;
    std::wcout << L"... press 'a' for help ..." << std::endl;
    coutSelectedFiles();
    if ( this->b_update_ls_list ) this->updateLsCommand(); 
    int it = 0;
    if (this->lines.size()>3) {
        if ( getIndex(this->currentDir)==0 )
            this->path2index[this->currentDir] = 3;
    }
    std::wstring current = L"";
    std::wstring is_selected = L"";
    for(const std::wstring& item: this->lines) {
        if (item.empty()) break; 
        int index = getIndex(this->currentDir);
        is_selected = this->isLineSelectedItem(it) ? L" * " : L"";
        if ( it == index - this->range ) {
            std::wcout << L"..." << std::endl;
            it++;
            continue; 
        }
        if ( it < index - this->range ) { 
            it++;
            continue; 
        }
        if ( it > index + this->range ) { 
            std::wcout << L"..." << std::endl;
            break; 
        }
        if ( index == it ) {
            std::wcout << L">  " << item << is_selected << L"  <" << std::endl;
            current = item;
        } else {
            std::wcout << L"   " << item << is_selected << std::endl;
        }
        it++;
    }
    std::wcout << std::endl;
    std::wcout << L"Current File/Folder: " << current << L" ";    
}
void TerminalExplorer::up(int step) {
    int count = 0;
    if ( this->path2index.count(this->currentDir)==0 ) return ;
    int currentIndex = this->path2index[this->currentDir];
    currentIndex=currentIndex-step;
    if ( currentIndex < 0 ) return;
    this->path2index[this->currentDir] = currentIndex;
}
void TerminalExplorer::down(int step) {
    int count = 0;
    if ( this->path2index.count(this->currentDir)==0 ) return ;
    int currentIndex = this->path2index[this->currentDir];
    // -- 
    for(const std::wstring& item: this->lines) {
        if (item.empty()) continue; 
        count++;
    }
    if (count == 0) return; 
    currentIndex=currentIndex+step;
    if ( currentIndex>=count ) return;
    this->path2index[this->currentDir] = currentIndex;
}
void TerminalExplorer::left() {
    changeDirectory(L"..");
    this->currentDir = std::filesystem::current_path();
    if ( this->path2index.count(this->currentDir) == 1 ) return;
    this->path2index[this->currentDir] = 0;
}
void TerminalExplorer::right() {
    int index = this->getIndex(this->currentDir);
    std::filesystem::path path = getPathFromLine(index);
    if (path.empty()) return;
    if (!std::filesystem::is_directory(path)) return;    
    changeDirectory(path.wstring());
    this->currentDir = std::filesystem::current_path();
    if ( this->path2index.count(this->currentDir) == 1 ) return;
    this->path2index[this->currentDir] = 0;
}
void TerminalExplorer::openTerminal() {
    ::openTerminal( this->currentDir, this->terminal );
}
bool TerminalExplorer::updateLsCommand() {
    std::wstring cmd = L"ls -la --human-readable --no-group --group-directories-first "+this->currentDir.wstring();
    return 
        commandOutput(cmd+L" --color=always", this->lines ) &&
        commandOutput(cmd, this->raw_lines)
    ;
}
void TerminalExplorer::coutSelectedFiles() {
    int count = 1;
    if ( this->selected_files.size()==0 ) goto end;
cout_display:
    std::wcout << std::endl;
    std::wcout << L"Selected Files/Folders:" << std::endl;
    for(const auto& item: this->selected_files) {
        std::wcout << count << ". " << item << std::endl;
        count++;
    }
end:    
    std::wcout << std::endl;
}
std::filesystem::path TerminalExplorer::getPathFromLine(int index) {
    std::wstring str_path = L"";
    std::filesystem::path path;
    if (index<0 || index >= this->raw_lines.size() ) goto fail;     
    str_path = this->raw_lines[index];
    if ( str_path.empty() ) goto fail;
    str_path = split(str_path,L' ').back();
    if ( str_path.empty() ) goto fail;
    path = std::filesystem::path(str_path);
    if (path.empty()) goto fail;
    if (!std::filesystem::exists(path)) goto fail;
    return path;
fail:
    return std::filesystem::path{};
} 
void TerminalExplorer::selectFile() {
    int index = getIndex(this->currentDir);
    std::filesystem::path path = getPathFromLine(index);
    if (path.string().compare(".")==0 || path.string().compare("..")==0) return;
    if (path.empty()) return;
    this->selected_files.insert( std::filesystem::absolute(path) );
}
void TerminalExplorer::deselectFile() {
    int index = getIndex(this->currentDir);
    std::filesystem::path path = getPathFromLine(index);
    if (path.string().compare(".")==0 || path.string().compare("..")==0) return;
    if (path.empty()) return;
    this->selected_files.erase( std::filesystem::absolute(path) );
}
bool TerminalExplorer::isLineSelectedItem(int index) {
    std::filesystem::path path = getPathFromLine(index);
    if (path.string().compare(".")==0 || path.string().compare("..")==0) return false;
    if (path.empty()) return false;
    return this->selected_files.contains( std::filesystem::absolute(path) );
}
bool TerminalExplorer::generateCopyFilesScript() {
    std::string content = createCopyToBashScript(this->selected_files, this->currentDir);
    if ( content.empty() ) return false;
    std::wstring tstamp = getTimeStamp();
    if ( saveFile(this->dropDir, L"copy_script_"+tstamp+L".sh", content) ) {
        std::wcout << L" -- Copy Script Generate at Drop Directory -- " << std::endl;
        return true;
    } else {
        std::wcout << L" -- Failed to Create Copy Script -- " << std::endl;
        return false;
    }
}
bool TerminalExplorer::generateMoveFilesScript() {
    std::string content = createMoveToBashScript(this->selected_files, this->currentDir);
    if ( content.empty() ) return false;
    std::wstring tstamp = getTimeStamp();
    if ( saveFile(this->dropDir, L"move_script_"+tstamp+L".sh", content) ) {
        std::wcout << L" -- Move Script Generate at Drop Directory -- " << std::endl;
        return true;
    } else {
        std::wcout << L" -- Failed to Create Move Script -- " << std::endl;
        return false;
    }
}
bool TerminalExplorer::generateDeleteFilesScript() {
    std::string content = createDeleteBashScript(this->selected_files);
    if ( content.empty() ) return false;
    std::wstring tstamp = getTimeStamp();
    if ( saveFile(this->dropDir, L"delete_script_"+tstamp+L".sh", content) ) {
        std::wcout << L" -- Delete Script Generate at Drop Directory -- " << std::endl;
        return true;
    } else {
        std::wcout << L" -- Failed to Create Delete Script -- " << std::endl;
        return false;
    }
}


/// END CODEX_termios.h 






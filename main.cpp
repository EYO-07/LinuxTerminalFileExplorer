/// BEGIN GOLEM main.cpp { termios }

// -- preprocessor directives 
#include "CODEX_termios.h"
static std::string VERSION="2026-06-23_22";
static std::string USAGE_TEXT = R"(
Usage: terminalFileExplorer [options]
    terminalFileExplorer                         Starts the Navigation
    terminalFileExplorer --terminal=cd           Starts Strict Navigation Mode
    terminalFileExplorer --help                  Display this message 

Options:
    --help                                       Show usage.
    --terminal=<terminal_application>            Set the terminal application.

Interactive Navigation:
    1. press q to quit 
    2. press i,j,k,l to navigate 
    3. press h,y to fast navigate
    4. press u to update the directories list 
    5. press s,d to select or deselect a file or folders
    6. press f to clear the selected files and folders 
    
    ... this program don't copy, delete or move files/folders directly
    ... instead it generates a bash script inside the starting folder 
    ... check it with `ls` and `cat SCRIPT` command and run it with `bash SCRIPT`
    ... the current directory will be the destination diretory for copy and move operations.
    1. press m to generate a move-bash-script on drop directory 
    2. press c to generate a copy-bash-script on drop directory
    3. press x to generate a delete-bash-script on drop directory
)";

// -- ENTRY POINT 
int main(int argc, char *argv[]) {
    // -- components
    bool b_exit = false;
    termios settings;
    TerminalExplorer Explorer = TerminalExplorer();
    Explorer.setDropDir(std::filesystem::current_path());
    char key;
    // -- arguments 
    if (argc>1) { // processing argument options
        int i;
        for (int i = 1; i < argc; ++i) {
            std::string strArgument = argv[i];
            std::wstring wstrArgument = utf8_to_wstring(strArgument);
            if( strArgument.starts_with("--terminal") ) {
                Explorer.setTerminal( split(strArgument,'=').back() );
            }
            if( strArgument.starts_with("--help") ) {
                std::cout << USAGE_TEXT << std::endl;
                b_exit = true;
                break;
            }
            if( strArgument.starts_with("--version") ) {
                std::cout << VERSION << std::endl;
                b_exit = true;
                break;
            }
        }
    }
    bool b_interactive_cd_mode = ( Explorer.getTerminal().compare("cd")==0 );
    // -- start 
    if (!disableRawMode(settings)) {
        std::cout << std::endl;
        std::cerr << "Failed to set raw mode." << std::endl;
        return 1;
    }
    if (b_exit) goto end_main;
    saveCursorPosition();
    Explorer.update();    
    while (!b_exit) {
        if (read(STDIN_FILENO, &key, 1) != 1) continue;
        if (key == 'q' || key == 'Q') {
            Explorer.outKeepPath();
            break;
        }
        clearOutput();
        if (key=='i') Explorer.up();
        if (key=='k') Explorer.down();
        if (key=='l') {
            Explorer.setUpdateList(true);
            Explorer.right();
        }
        if (key=='j') {
            Explorer.setUpdateList(true);
            Explorer.left();
        }
        if (key=='t') { 
            if ( b_interactive_cd_mode ) {
                Explorer.outChangePath();
                break;
            } else {
                Explorer.openTerminal();
            }
        }
        if (key=='y') Explorer.up(10);
        if (key=='h') Explorer.down(10);
        if (key=='s' && !b_interactive_cd_mode) Explorer.selectFile(); // select current file or directory
        if (key=='d' && !b_interactive_cd_mode) Explorer.deselectFile(); // select current file or directory
        if (key=='f' && !b_interactive_cd_mode) Explorer.clearSelectedFiles(); // clear all
        if (key=='m' && !b_interactive_cd_mode) { // move selected to current directory
            if ( Explorer.generateMoveFilesScript() ) break;
        } 
        if (key=='c' && !b_interactive_cd_mode) { // copy selected to current directory
            if ( Explorer.generateCopyFilesScript() ) break;
        } 
        if (key=='x' && !b_interactive_cd_mode) { // delete selected 
            if ( Explorer.generateDeleteFilesScript() ) break;
        } 
        if (key=='u') Explorer.setUpdateList(true); // update ls list 
        if (key=='a') { 
            std::cout << USAGE_TEXT << std::endl;
        } else {
            Explorer.update();
        }
        Explorer.setUpdateList(false);
        std::cout.flush(); // Force output immediately 
    }
end_main:
    restoreMode(settings);
    return 0;
}

/// END GOLEM main.cpp { termios }
# BEGIN : build.sh 

clear 
echo "Starting Compilation using g++ ..."
g++ -std=c++20 main.cpp CODEX_termios.cpp -o terminalFileExplorer
echo "Script Finished, test it using ./terminalFileExplorer"
echo "Check Instructions with ./terminalFileExplorer --help"

# END
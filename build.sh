# BEGIN : build.sh 

clear 
echo "starting compilation using g++ ..."
g++ -std=c++20 main.cpp CODEX_termios.cpp -o terminalFileExplorer
echo "... script finished, test it using ./terminalFileExplorer"
echo "... check instructions with ./terminalFileExplorer --help"

# END
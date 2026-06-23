/// BEGIN Grimoire Linux Interactive Terminal
/** MOPP : Magic Oriented Programming Paradigm 

... programming is the closest thing to magic in our world

Paradigm [ Magic Oriented Programming Paradigm ] : Concept mapping between programming and magic. 

Motto [ Wizards uses Magic ] : An adept of MOPP should not have aversion of new technologies, if it can make your life easy like magic, then you should master it. 
1. MOPP is specially designed for LLM's and code generators A.I.'s 
2. How much control you want for your project? This will dictate how you use magic.

Concepts { Magic Oriented Programming Paradigm }
1. Spellcasting : Spell is a function and casting is the use of the appropriate syntax.
2. Oracles : Are the LLM's, a wizard should use it to craft new spells or consulting.
3. Grimoires : list of existing spells, generaly crafted as a `cheat sheet comment block` format with aid of oracules. Grimoires can be safely handled by oracles. 
4. Golems : the golem is the manually crafted part of code, managed primarly by humans, the golem should be as close as possible (have the form) of the project's logic. Golem could be the main file or any other file intended for this purpose.
5. Codex : the codex are the crafted spells (framework).
6. LLM Spellcrafting : humans design the spell (syntax) and ask for implementation for oracles.
7. Golem Sculpting : can be or not be made by LLM's, golem sculping is the creation of dummy and placeholder classes and functions for your project, it will be the 'skeleton' of your project.
8. Golem Infusion : is the implementation of the dummies and placeholders of your project.
9. Golem Craft : is the programming itself, the main objective of a wizard is the golem creation, the golem is the program which is designed to do some hard work. 
10. Divination : Oracle consulting 
11. Library : Official Documentation, Search Engines 

{ Grimoire Craft, Golem Sculpting, Spellcast, Codex Spellcraft, Golem Infusion, Divination }
Grimoire Craft % Divination, Library
Golem Sculpting % Divination, Library 
Golem Infusion % Spellcast { Grimoire Spellcast, Codex Spellcast } 
Codex % Divination, Library

... once the project begins these will be the main activities for development
... (Golem Sculpting, Codex/Grimoire Maintenance, Golem Infusion)

... Every time you need to manually write a code, that is considered golem craft, so if you manually write a spell in the codex, the wizard should understand the activity as golem craft, so mentally you will perform that activity sculpting, crafting new helper spells, and performing infusion. The wizard should differentiate this from the global project organization.

... (Sculpting, Spellcrafting, Infusion) is how a programmer sees the programming as a wizard.
... (Sculpting, Infusion) is a shortcut workflow, when spellcraft isn't necessary if the spells necessary for the project is already in the codex or already in the built-in libraries from the grimoire.

Workflow [ Programming ] { Magic Oriented Programming Paradigm } 
-> Coding || Design || Dialetic | Logic | Workflow
-> Coding || Design | Project Sculpting | & || $ Grimoire | &? || Sculpting | % || Spellcraft
-> Coding || Design | Project Sculpting | & || $ Grimoire | &? || Sculpting | % | Infusion 

*/

/* Inventory [] { C++ }
1. SYNTAX // SHORTDESCRIPTION 
*/ 

/* Inventory [] { C++, Linux }
1. SYNTAX // SHORTDESCRIPTION 
*/ 


// ==============================================================================================
// runomancy : language basics 

/// Compilers
/* Inventory [ GCC Usage ] { C++, Linux }
1. g++ main.cpp -o app // Compile single file
2. g++ -Wall -Wextra -g main.cpp -o app // Compile with warnings & debug
3. g++ -O2 main.cpp -o app // Compile with optimizations (-O0 to -O3)
4. g++ main.cpp utils.cpp -o app // Compile multiple files
5. g++ -DDEBUG main.cpp -o app // Define macros at compile time
6. g++ main.cpp -o app // Link against standard library (usually auto)
7. g++ main.cpp -lpthread -o app // Link against pthread
8. g++ main.cpp -lm -o app // Link against math library (libm)
9. g++ -c main.cpp // Check syntax only (no linking)
10. g++ -E main.cpp // Show preprocessed output
11. g++ -S main.cpp // Show assembly output
12. g++ -std=c++17 main.cpp -o app // Enable C++ standard version
13. g++ -Wall -Wextra -Wpedantic -Werror main.cpp -o app // Use strictest warnings
14. g++ main.cpp -L./lib -lmylib -o app // Link static library (.a)
15. g++ main.cpp -L./lib -lmylib -o app // Link dynamic library (.so)
16. g++ -MM main.cpp // Generate dependency file (Makefile friendly)
17. g++ --version // Show version
18. LD_LIBRARY_PATH=./lib ./app // Run with environment variable
19. g++ -flto main.cpp -o app // Compile with LTO (Link Time Optimization)
20. g++ -m32 main.cpp -o app // Force 32-bit compilation (if multilib installed)
-> g++ -std=c++20 your_file.cpp -o your_program // ... 
*/

/// Basic Types
/* Inventory [ std::string ] { C++20 }
1. s.length(); / s.size(); // Returns number of characters
2. s.empty(); // Returns true if string is empty
3. s.append(str); / s += str; // Appends string to the end
4. s.push_back(c); // Adds character c to the end
5. s.starts_with(pref); // C++20: Returns true if string begins with pref
6. s.ends_with(suff); // C++20: Returns true if string ends with suff
7. s.contains(sub); // C++23 (often used with C++20): Checks for substring
8. s.substr(pos, len); // Returns a sub-string starting at pos
9. s.find(sub); // Returns index of first occurrence or std::string::npos
10. s.replace(pos, len, str); // Replaces portion of string
11. s.insert(pos, str); // Inserts string at specified position
12. s.erase(pos, len); // Removes characters from string
13. s.compare(str); // Returns 0 if strings are equal
14. s.front(); / s.back(); // Accesses first / last character
15. s.c_str(); // Returns pointer to null-terminated C-string
16. std::stoi(s); / std::to_string(val); // Numeric conversions
*/

/// Collections 
/* Inventory [ Vector ] { C++, std::vector } 
1. std::vector<T> v; // Declares an empty vector of type T
2. v.push_back(val); // Adds element to the end
3. v.emplace_back(args); // Constructs element in-place at the end
4. v.pop_back(); // Removes the last element
5. v.size(); // Returns number of elements
6. v.empty(); // Returns true if size is 0
7. v[i]; // Access element at index i (no bounds check)
8. v.at(i); // Access element at index i (with bounds check)
9. v.front(); / v.back(); // Access first / last element
10. v.clear(); // Removes all elements
11. v.reserve(n); // Allocates capacity for at least n elements
12. v.resize(n); // Changes size to n elements
13. v.begin(); / v.end(); // Returns iterator to start / past-the-end
14. v.insert(pos, val); // Inserts val before iterator pos
15. v.erase(pos); // Removes element at iterator pos
*/
/* Inventory [ <unordered_map> ] { C++ }
1. std::unordered_map<K, V> m; // Declares hash map with key type K and value type V
2. m[key] = val; // Inserts or updates element (creates default if key missing)
3. m.at(key); // Accesses value with bounds checking (throws if missing)
4. m.insert({key, val}); // Inserts pair; does nothing if key exists
5. m.emplace(args); // Constructs pair in-place for efficiency
6. m.erase(key); // Removes element by key
7. m.find(key); // Returns iterator to element, or m.end() if not found
8. m.count(key); // Returns 1 if key exists, 0 otherwise
9. m.size(); // Returns number of elements
10. m.empty(); // Returns true if map is empty
11. m.clear(); // Removes all elements
12. m.reserve(n); // Sets bucket count to accommodate n elements
13. m.bucket_count(); // Returns current number of hash buckets
14. m.load_factor(); // Returns average elements per bucket (size/bucket_count)
15. for(auto& [k, v] : m); // Structured binding loop to iterate pairs
*/
/* Inventory [ <set> ] { C++20 }
1. std::set<T>              // Sorted unique elements (Tree-based)
2. std::unordered_set<T>    // Unsorted unique elements (Hash-based)
3. std::erase(c, val)       // Remove all elements == val (C++20)
4. std::erase_if(c, pred)   // Remove elements matching pred (C++20)
5. c.contains(val)          // Check existence (C++20)
6. c.emplace(args...)       // Construct in-place (C++11+, C++20 refined)
7. c.insert_or_assign(k, v) // Insert or update (Map/Set generic)
8. view | std::views::filter// Range-based filtering (C++20)
9. std::ranges::set_union   // Merge sorted ranges efficiently
10. std::ranges::set_intersection // Find common elements
11. s.insert(element);
*/
/* Inventory [ Set ] { C++20 }
1. std::set<T> / std::unordered_set<T>
2. c.count(val)          // Returns 1 if found, 0 otherwise (Redundant with .contains() in C++20)
3. c.contains(val)       // Returns bool (true/false) - Preferred in C++20
4. c.size()              // Total number of unique items
5. c.empty()             // Check if inventory is empty
6. c.erase(val)          // Remove item (returns 1 if removed, 0 if not found)
7. std::erase_if(c, pred)// Bulk remove by condition (C++20)
8. c.insert(val)         // Add item (returns pair<iterator, bool>)
9. c.emplace(args...)    // Construct item in-place
10. c.find(val)          // Get iterator to item (or end())
11. c.clear()            // Remove all items
*/   

/// Filesystem 
/* Inventory [ <filesystem> ] { C++ }
1. fs::path p(str); // Creates a path object
2. fs::exists(p); // Checks if path exists
3. fs::is_directory(p); // Checks if path is a directory
4. fs::is_regular_file(p); // Checks if path is a file
5. fs::file_size(p); // Returns size of file in bytes
6. fs::create_directory(p); // Creates a new directory
7. fs::copy(src, dst); // Copies files or directories
8. fs::remove(p); // Deletes a file or empty directory
9. fs::remove_all(p); // Deletes directory and all contents
10. fs::rename(old, new); // Renames or moves a file/dir
11. fs::current_path(); // Gets or sets working directory
12. p.extension(); // Returns the file extension (e.g., ".txt")
13. p.filename(); // Returns the generic name component
14. p.parent_path(); // Returns the path to the parent directory
15. fs::directory_iterator(p); // Iterator for directory contents
16. fs::recursive_directory_iterator(p); // Iterator for contents + subfolders
*/
/* Inventory [ std::filesystem::path ] { C++ }
1. p.string(); // Returns path as a std::string
2. p.parent_path(); // Returns path of the parent directory
3. p.filename(); // Returns the filename component (name + ext)
4. p.stem(); // Returns filename without the extension
5. p.extension(); // Returns the extension (including the dot)
6. p.is_absolute(); / p.is_relative(); // Checks path type
7. p.root_name(); / p.root_directory(); // Returns root components (e.g., "C:" or "/")
8. p.replace_filename(new_f); // Replaces the last component of the path
9. p.replace_extension(new_e); // Replaces or removes (if empty) the extension
10. p.clear(); // Empties the path object
11. p1 / p2; // Operator/ joins paths with the platform's preferred separator
12. p1 /= p2; // Appends p2 to p1 using a directory separator
13. p.lexically_normal(); // Returns path with dots and redundant separators removed
14. p.lexically_relative(base); // Returns path relative to a base path
15. p.begin(); / p.end(); // Iterates through each component of the path
16. std::filesystem::absolute(path); // convert to absolute path
*/


// ==============================================================================================
// incantation : user interface 

/* Inventory [ Termios ] { C++, Linux, Basics }
1. #include <termios.h> // Include header
2. struct termios oldt, newt; // Declare terminal structures
3. tcgetattr(STDIN_FILENO, &oldt); // Save current settings
4. tcgetattr(STDIN_FILENO, &newt); // Load settings for modification
5. newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode & echo
6. newt.c_cc[VMIN] = 1; // Set min chars to read
7. newt.c_cc[VTIME] = 0; // Set timeout (0 = infinite)
8. tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings immediately
9. tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original settings
10. cfmakeraw(&newt); // Shortcut to set raw mode
11. STDIN_FILENO // File descriptor for standard input (0)
12. TCSANOW // Change takes effect immediately
13. TCSADRAIN // Change after output is written
14. TCSAFLUSH // Change after output, discard unread input
*/

/* Inventory [ Interactive Terminal ] { C++, Linux, Termios }
1. #include <unistd.h> // For read(), STDIN_FILENO
2. #include <iostream> // For cout
3. char c; // Variable to store input
4. std::cout << "Press a key: "; // Prompt user
5. read(STDIN_FILENO, &c, 1); // Read single char without Enter
6. std::cout << "\nYou pressed: " << c << std::endl; // Display result
7. // Always restore settings in destructor or try-catch/final block
8. // Use signal handler to restore on SIGINT (Ctrl+C)
9. // Check return value of tcgetattr/tcsetattr for errors (-1)
10. // Use 'fflush(stdout)' to ensure prompt appears before read
*/






/// END Grimoire Linux Interactive Terminal
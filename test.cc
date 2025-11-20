#include <iostream>
#include <string>
#include <vector>
#include <cstring>   // for strcmp, strlen
#include <dirent.h>  // POSIX directory handling
#include <sys/types.h>

// --- MOCK FUNCTIONS & HELPERS ---

// Mocking the 'excluded' function. 
// Returns true if you want to skip a file. 
// Currently returns false (include everything).
bool excluded(const std::string& path) {
    // Example: Ignore files ending in .o
    // if (path.length() > 2 && path.substr(path.length()-2) == ".o") return true;
    return false;
}

// Mocking push_excludes (returns dummy stack index)
int push_excludes_from(const std::string& base) {
    // std::cout << "[DEBUG] Pushing ignore rules for: " << (base.empty() ? "root" : base) << "\n";
    return 0; 
}

// Mocking pop_excludes
void pop_excludes(int stack_idx) {
    // std::cout << "[DEBUG] Popping ignore rules\n";
}

// The final print function
void dir_add_entry(std::string& name, int len) {
    std::cout << "  [ADDED FILE]: " << name << std::endl;
}

// --- CORE RECURSIVE LOGIC ---

int read_work_tree_recursive(const std::string& path, const std::string& base) {
    DIR* dir = opendir(path.c_str());
    
    // 1. Setup buffer. Base represents the prefix (e.g., "src/")
    std::string fullname = base;
    
    // 2. Save the "reset point" length
    size_t baselen = fullname.size(); 

    int contents = 0;

    if (dir) {
        struct dirent* d;
        
        // Load .gitignore for this directory
        int excludes = push_excludes_from(base);
        
        while ((d = readdir(dir)) != nullptr) {
            
            // Skip ".", "..", and ".git"
            if (d->d_name[0] == '.' && (d->d_name[1] == '\0' || 
               (d->d_name[1] == '.' && d->d_name[2] == '\0') || 
               strcmp(d->d_name, ".git") == 0)) {
                continue;
            }

            // 3. Rewind the buffer to the base length
            fullname.resize(baselen);
            
            // 4. Append current file name
            fullname.append(d->d_name);
            
            int de_name_len = strlen(d->d_name);

            // Check if ignored
            if (excluded(fullname))
                continue; 

            switch (d->d_type) {
            case DT_DIR:
                // It's a directory: Append '/' and recurse
                fullname.push_back('/');
                // Recurse! Note we pass fullname as both path and base
                contents += read_work_tree_recursive(fullname, fullname);
                
                // Important: We 'continue' here because we don't want to 
                // run dir_add_entry for the directory folder itself, 
                // only the files inside it.
                continue; 

            case DT_REG:
                contents++;
                break; // Break out to hit dir_add_entry below
            case DT_LNK:
                contents++;
                break; // Break out to hit dir_add_entry below
            default:
                fprintf(stderr, "Skipping non-regular file: %s\n", d->d_name);
                continue;
            }

            // Add the file to the list
            dir_add_entry(fullname, de_name_len);
        }
        
        // Unload .gitignore rules
        pop_excludes(excludes);
        closedir(dir);
    } else {
        std::cerr << "Could not open directory: " << path << std::endl;
    }
    
    return contents;
}

int main() {
    std::cout << "Scanning current directory recursively...\n" << "-----------------------------------------\n";
    
    // Start scanning the current directory (".")
    // Base is empty "" because we want files to look like "main.cpp", not "./main.cpp"
    int total_files = read_work_tree_recursive(".", "");

    std::cout << "-----------------------------------------\n";
    std::cout << "Total files found: " << total_files << std::endl;

    return 0;
}
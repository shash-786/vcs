#include <vector>
#include <string>
#include <memory>

struct worktree_entries {
    std::string name ;
} ;

struct directory_ignore_ruleset {
    int nr ;
    struct entry {
        std::string rule ;
        std::string base ;
    } ;
    std::vector<entry>  rules ;
} ;

struct worktree {
    // NOTE: we will only be storing file names here sorted
    std::vector<worktree_entries> files ;
    std::unique_ptr<directory_ignore_ruleset> dr;
} ;
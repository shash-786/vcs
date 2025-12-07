#include "core/object/Blob.h"
#include "core/object/Commit.h"
#include "core/object/Tree.h"
#include <memory>

namespace Object 
{
    class GitObjectfactory {
        public:
        static std::unique_ptr<Object::GitObject> get_object (Object::type t, const std::string& hash, std::vector<uint8_t> data, size_t size) {
            switch (t)
            {
            case Object::type::OBJECT_BLOB :
                return std::make_unique<Object::Blob> (hash, std::move(data), size) ;
            case Object::type::OBJECT_COMMIT :
                return std::make_unique<Object::Commit> (hash, std::move(data), size) ;
            case Object::type::OBJECT_TREE :
                return std::make_unique<Object::Tree> (hash, std::move(data), size) ;
            default:
                fprintf(stderr, "UNKNOWN OBJECT TYPE") ;
                exit(1) ;
                break ;
            }
        }
    } ;
}

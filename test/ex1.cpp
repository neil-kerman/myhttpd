#include <iostream>
#include <memory>
#include <set>

class object {
private:
    std::string _id;
public:
    object(std::string id): _id(id) {
        std::cout << "default constructor called, id: " << this->_id << std::endl;
    }
    object(const object &src): _id(src._id) {
        std::cout << "copy constructor called, id: " << this->_id << std::endl;
    }
    object(object &&src) {
        this->_id = src._id;
        src._id += " moved";
        std::cout << "move constructor called, id: " << this->_id << std::endl;
    }
    ~object() {
        std::cout << "decontructor called, id: " << this->_id << std::endl;
    }
    void add_tag(std::string tag) {
        this->_id += (" " + tag);
    }
};

object func_a(object obj) {
    obj.add_tag("passed func_a");
    return obj;
}

int main(int argc, char *argv[]) {
    object o2 = func_a(object("obj1"));
}
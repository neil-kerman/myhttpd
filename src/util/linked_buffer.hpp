#include <list>
#include <array>
#include <cstddef>


namespace myhttpd {
#define BlockSize 4096
    //template <typename BlockSize>
    class linked_buffer {

    private:
        typedef std::array<char, BlockSize> BlockType;
    
    public:
        struct block {
            char* base;
            std::size_t size;
        };

    private:
        std::size_t _size = 0;

        std::size_t _capacity;

        std::size_t _offset = 0;

        std::list<BlockType> _blocks;

    private:

    public:
        linked_buffer(std::size_t blocks = 1)
        : _blocks(std::list<BlockType>(blocks), _capacity(this->_blocks.size()* BlcokSize) {}


        linked_buffer(linked_buffer&& buf) 
        : _blocks(std::move(buf._blocks)), _size(buf._size), _capacity(buf._capacity) {}

        linked_buffer(const linked_buffer& buf)
        : _blocks(std::copy(buf._blocks)), _size(buf._size), _capacity(buf._capacity) {}

        void consume(std::size_t size) {

        }

        void commit(std::size_t size) {

        }

        std::size_t size() {

        }

        std::list<block> get_available_block() {

        }

        std::list<block> get_data_block() {

        }
    };
}
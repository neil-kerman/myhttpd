#ifndef LINKED_BUFFER_HPP
#define LINKED_BUFFER_HPP

#include <list>
#include <array>
#include <cstddef>

namespace myhttpd::protocol::http {
    
    template <std::size_t BlockSize> class linked_buffer {

    private:
        typedef std::array<char, BlockSize> BlockType;
    
    public:
        struct block {

            char* data;
            std::size_t size;
        };

    private:
        std::size_t _size = 0;

        std::size_t _capacity = 0;

        std::size_t _offset = 0;

        std::list<BlockType> _blocks;
    
    public:
        void consume(std::size_t size) {

            this->_size -= size;
            this->_offset += size;

            while(this->_offset >= BlockSize) {

                this->_offset -= BlockSize;
                this->_blocks.pop_front();
            }
        }

        void commit(std::size_t size) {

            this->_size += size;
            this->_capacity -= size;
        }

        std::size_t size() {

            return this->_size;
        }

        block get_available_block() {

            if (this->_capacity == 0) {

                this->_blocks.push_back(BlockType());
                this->_capacity += BlockSize;
                return block{ this->_blocks.back().data(), BlockSize };

            } else {

                return block{ this->_blocks.back().data() + ((this->_offset + this->_size) % BlockSize), this->_capacity};
            }
        }

        std::list<block> get_data_blocks(std::size_t size) {

            std::list<block> data_blocks;

            if (size <= this->_size) {

                auto remaminder_size = size;
                auto block_base_offset = this->_offset;
                auto it = this->_blocks.begin();

                while (remaminder_size != 0) {

                    auto block_size = (remaminder_size < BlockSize - block_base_offset) ?
                        remaminder_size : BlockSize - block_base_offset;
                    data_blocks.push_back(block{ it->data() + block_base_offset, block_size });
                    remaminder_size -= block_size;
                    block_base_offset -= block_base_offset;
                    it++;
                }
            }

            return data_blocks;
        }

        std::list<block> get_data_blocks() {

            return this->get_data_blocks(this->size());
        }

        block get_last_data_block() {

            return this->get_data_blocks().back();
        }

        void take_data(char* buf, std::size_t size) {

            auto buf_ptr = buf;
            auto data_blocks = this->get_data_blocks(size);
            std::size_t remainder_size = size;

            for (auto& blk : data_blocks) {

                if (blk.size < remainder_size) {

                    std::memcpy(buf_ptr, (char*)blk.data, blk.size);
                    buf_ptr += blk.size;
                    remainder_size -= blk.size;

                } else {

                    std::memcpy(buf_ptr, (char*)blk.data, remainder_size);
                    break;
                }
            }

            return;
        }

    public:
        linked_buffer() = default;

        linked_buffer(linked_buffer&& buf)
        : _blocks(std::move(buf._blocks)), _size(buf._size), _capacity(buf._capacity) {

        }
    };
}

#endif // LINKED_BUFFER_HPP

#pragma once

#include "extended_char.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace NStructures {

    struct Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        uint64_t cnt = 0;
        uint64_t cnt_terminated = 0;
        ExtendedChar data;
        bool is_terminated = false;

        Node(ExtendedChar c, uint64_t cnt);
        Node(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right);
    };

    class Trie {
    public:
        using Code = std::pair<ExtendedChar, std::vector<bool>>;

        explicit Trie(const std::unordered_map<ExtendedChar, uint64_t>& cnt);

        std::vector<Code> AllCodes() const;
    
    private:
        Code FindCodeByIndex(size_t index) const;

    private:
        std::shared_ptr<Node> root_;
    };

}

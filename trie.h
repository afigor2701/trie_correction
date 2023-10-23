#pragma once

#include "extended_char.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace NStructures {

    class Trie {
    private:
        struct Node {
            std::shared_ptr<Node> left;
            std::shared_ptr<Node> right;
            uint64_t cnt = 0;
            ExtendedChar data;
            bool is_terminated = false;

            Node(ExtendedChar c, uint64_t cnt);
            Node(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right);

            void Dfs(std::vector<std::pair<ExtendedChar, std::vector<bool>>>& codes, std::vector<bool>& curr_code) const;
        };

        std::shared_ptr<Node> root_;

    public:
        explicit Trie(const std::unordered_map<ExtendedChar, uint64_t>& cnt);

        std::vector<std::pair<ExtendedChar, std::vector<bool>>> AllCodes() const;
    };

}

#include "trie.h"

#include <queue>

#include <algorithm>
#include <tuple>
#include <stdexcept>

namespace NStructures {

    Trie::Node::Node(ExtendedChar c, uint64_t cnt) : cnt(cnt), data(c), is_terminated(true) {
    }

    Trie::Node::Node(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right)
        : left(left),
          right(right),
          cnt(left->cnt + right->cnt),
          data(std::min(left->data, right->data)),
          is_terminated(false) {
    }

    void Trie::Node::Dfs(std::vector<std::pair<ExtendedChar, std::vector<bool>>>& codes,
                        std::vector<bool>& curr_code) const {
        if (!is_terminated) {
            auto process_node_ptr = [&codes, &curr_code] (const std::shared_ptr<Node>& node, bool value) {
                if (node) {
                    curr_code.push_back(value);
                    node->Dfs(codes, curr_code);
                    curr_code.pop_back();
                }
            };
            
            process_node_ptr(left, false);
            process_node_ptr(right, true);
        } else {
            codes.push_back({data, curr_code});
        }
    }

    Trie::Trie(const std::unordered_map<ExtendedChar, uint64_t>& cnt) : root_(nullptr) {
        static constexpr auto cmp = [](const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right) {
            return std::tie(left->cnt, left->data) > std::tie(right->cnt, right->data);
        };

        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> max_size_node(cmp);
        for (const auto& curr : cnt) {
            max_size_node.push(std::make_shared<Node>(curr.first, curr.second));
        }

        while (max_size_node.size() > 1) {
            auto curr1 = max_size_node.top();
            max_size_node.pop();
            auto curr2 = max_size_node.top();
            max_size_node.pop();

            max_size_node.push(std::make_shared<Node>(curr1, curr2));
        }

        if (!max_size_node.empty()) {
            root_ = max_size_node.top();
        }
    }

    std::vector<std::pair<ExtendedChar, std::vector<bool>>> Trie::AllCodes() const {
        std::vector<std::pair<ExtendedChar, std::vector<bool>>> result;
        if (root_) {
            std::vector<bool> curr_code;
            root_->Dfs(result, curr_code);
        }
        return result;
    }

}

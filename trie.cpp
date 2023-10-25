#include "trie.h"

#include <queue>

#include <algorithm>
#include <cassert>
#include <tuple>
#include <stdexcept>

namespace NStructures {

    Node::Node(ExtendedChar c, uint64_t cnt) : cnt(cnt), cnt_terminated(1), data(c), is_terminated(true) {
    }

    Node::Node(const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right)
        : left(left),
          right(right),
          cnt(left->cnt + right->cnt),
          cnt_terminated(left->cnt_terminated + right->cnt_terminated),
          data(std::min(left->data, right->data)),
          is_terminated(false) {
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

    std::vector<Trie::Code> Trie::AllCodes() const {
        std::vector<Code> result;
        if (root_) {
            result.resize(root_->cnt_terminated);
            for (size_t i = 0; i < result.size(); ++i) {
                result[i] = FindCodeByIndex(i);
            }
        }
        return result;
    }

    Trie::Code Trie::FindCodeByIndex(size_t index) const {
        assert(root_ && index < root_->cnt_terminated);

        Code code;
        auto node = root_;
        while (!node->is_terminated) {
            if (node->left->cnt_terminated > index) {
                code.second.push_back(false);
                node = node->left;
            } else {
                code.second.push_back(true);
                index -= node->left->cnt_terminated;
                node = node->right;
            }
        }
        code.first = node->data;
        return code;
    }

}

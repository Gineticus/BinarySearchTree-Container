#pragma once

#include <cstdint>

template <
    typename T, typename DefaultTraversal = InOrder<T>,
    typename Compare = std::less<T>, typename Allocator = std::allocator<T>,
    typename =
        std::void_t<decltype(DefaultTraversal::next, DefaultTraversal::prev)>>
class OrderedTree;

template <typename Traversal, typename Container,
          typename = std::void_t<decltype(Traversal::next, Traversal::prev)>>
class TreeIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Container::value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using traversal = Traversal;

   private:
    Node<value_type>* node_;
    const BST<value_type, typename Container::compare,
              typename Container::allocator>* bst_;
    bool reverse_;

    friend class OrderedTree<value_type, typename Container::default_traversal,
                             typename Container::compare,
                             typename Container::allocator>;

   public:
    TreeIterator(Node<value_type>* node,
                 const BST<value_type, typename Container::compare,
                           typename Container::allocator>* bst,
                 bool reverse = false)
        : node_(node), bst_(bst), reverse_(reverse) {}
    TreeIterator(const TreeIterator& other)
        : node_(other.node_), bst_(other.bst_), reverse_(other.reverse_) {}
    ~TreeIterator() = default;

    TreeIterator& operator=(const TreeIterator& other) {
        if (bst_ != other.bst_) {
            throw std::invalid_argument("Iterators are from different trees");
        }
        node_ = other.node_;
        reverse_ = other.reverse_;
        return *this;
    }

    [[nodiscard]] reference operator*() const { return node_->data_; }

    [[nodiscard]] pointer operator->() const { return &node_->data_; }

    bool operator==(const TreeIterator& other) const {
        return node_ == other.node_;
    }

    bool operator!=(const TreeIterator& other) const {
        return !(*this == other);
    }

    TreeIterator& operator++() {
        if (reverse_) {
            node_ = traversal::prev(node_, bst_->root());
            return *this;
        }
        node_ = traversal::next(node_, bst_->root());
        return *this;
    }
    TreeIterator& operator--() {
        if (reverse_) {
            node_ = traversal::next(node_, bst_->root());
            return *this;
        }
        node_ = traversal::prev(node_, bst_->root());
        return *this;
    }

    TreeIterator operator++(int) {
        TreeIterator temp = *this;
        ++(*this);
        return temp;
    }

    TreeIterator operator--(int) {
        TreeIterator temp = *this;
        --(*this);
        return temp;
    }
};
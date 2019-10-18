#pragma once

#include <assert.h>
#include <string>
#include <memory>
#include <utility>
#include <list>

namespace BraceExp
{
class Node
{
public:
    enum class Type { Set, Mul, Val };
    void Compute()
    {
        for (auto n : children_) {
            n->Compute();
        }
        if (type_ == Type::Mul) {
            auto itr = children_.begin();
            auto rhs = *itr;
            ++itr;
            auto lhs = *itr;
            children_.clear();
            for (auto i1 : rhs->children_) {
                assert(i1->type_ == Type::Val);
                for (auto i2 : lhs->children_) {
                    assert(i2->type_ == Type::Val);
                    Node* n = new Node();
                    n->type_ = Type::Val;
                    n->val_ = i1->val_ + i2->val_;
                    children_.push_back(n);
                }
            }
            type_ = Type::Set;
        }
        auto itr = children_.begin();
        while (itr != children_.end()) {
            auto n = *itr;
            if (n->type_ == Type::Set) {
                children_.erase(itr++);
                for (auto c : n->children_) {
                    Node* nn = new Node();
                    nn->type_ = Type::Val;
                    nn->val_ = c->val_;
                    children_.insert(itr, nn);
                }
            } else {
                ++itr;
            }
        }
    }
    Type type_ = Type::Set;
    std::list<Node*> children_;
    std::string val_;

private:
};

void test()
{
    Node root;
    root.type_ = Node::Type::Mul;

    Node* abcd = new Node();
    abcd->type_ = Node::Type::Set;
    root.children_.push_back(abcd);

    Node* a = new Node();
    a->type_ = Node::Type::Val;
    a->val_ = "A";
    abcd->children_.push_back(a);

    Node* bcd = new Node();
    bcd->type_ = Node::Type::Mul;
    abcd->children_.push_back(bcd);

    Node* bl = new Node();
    bl->type_ = Node::Type::Set;
    Node* b = new Node();
    b->type_ = Node::Type::Val;
    b->val_ = "B";
    bcd->children_.push_back(bl);
    bl->children_.push_back(b);
    Node* cd = new Node();
    cd->type_ = Node::Type::Set;
    bcd->children_.push_back(cd);

    Node* c = new Node();
    c->type_ = Node::Type::Val;
    c->val_ = "C";
    cd->children_.push_back(c);
    Node* d = new Node();
    d->type_ = Node::Type::Val;
    d->val_ = "D";
    cd->children_.push_back(d);

    Node* ef = new Node();
    ef->type_ = Node::Type::Set;
    root.children_.push_back(ef);

    Node* e = new Node();
    e->type_ = Node::Type::Val;
    e->val_ = "E";
    ef->children_.push_back(e);
    Node* f = new Node();
    f->type_ = Node::Type::Val;
    f->val_ = "F";
    ef->children_.push_back(f);

    root.Compute();
}


void test1(std::string s)
{
}

} // namespace BraceExp

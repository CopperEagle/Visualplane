#ifndef INTERVALLTREE_H
#define INTERVALLTREE_H

#include <cstdint>
#include "../graphics/scene/AnimationInstruction.h"

/*!
* Manages events that have a span. You can then give a particular timepoint and
* it will return to you the events that are active at that point in time.
*
* Currently does it in an inefficient way, as linked list.
* \todo Use a better datastructure here.$
*/


template <class T>
struct IntvTreeNode
{
    uint64_t start, end;
    T* data;
    IntvTreeNode* next;
};

template <class T>
class IntervallTree
{
public:
    IntervallTree(uint64_t start, uint64_t end);
    IntervallTree(){s = 0; t = 0;};
    ~IntervallTree();


    void addEvent(uint64_t start, uint64_t end, T& event);
    void deleteAll();
    T** getActiveEvents(uint64_t time, int& numEvents);

private:
    uint64_t s, t;
    IntvTreeNode<T>* root = nullptr;
    IntvTreeNode<T>* last = nullptr;

};

template <class T>
IntervallTree<T>::IntervallTree(uint64_t start, uint64_t end): s(start), t(end){}

template <class T>
IntervallTree<T>::~IntervallTree(){}

template <class T>
void IntervallTree<T>::addEvent(uint64_t start, uint64_t end, T& event){
    if (root == nullptr)
    {
        root = new IntvTreeNode<T>{start, end, &event, nullptr};
        last = root;
        return;
    }
    last->next = new IntvTreeNode<T>{start, end, &event, nullptr};
    last = last->next;
}

/*!
 * @brief Deletes the contents of the tree. Also deletes the data elements inside.
 */
template <class T>
void IntervallTree<T>::deleteAll()
{
    IntvTreeNode<T>* node = root;
    IntvTreeNode<T>* prevnode = root;
    while (node != nullptr)
    {
        delete node->data;
        prevnode = node;
        node = node->next;
        delete prevnode;
    };

}

/*
Dont forget to call delete on the return.*/
template <class T>
T** IntervallTree<T>::getActiveEvents(uint64_t time, int& numEvents)
{
    // yeah... this is extremely inefficient... we first need to count
    IntvTreeNode<T>* node = root;
    numEvents = 0;
    while (node != nullptr)
    {
        if (node->start <= time && node->end >= time)
        {
            numEvents++;
        }
        node = node->next;
    }

    T** ret = new T*[numEvents];
    int curr = 0;
    node = root;
    while (node != nullptr)
    {
        if (node->start <= time && node->end >= time)
        {
            ret[curr++] = node->data;
        }
        node = node->next;
    }

    return ret;
}

#endif // INTERVALLTREE_H
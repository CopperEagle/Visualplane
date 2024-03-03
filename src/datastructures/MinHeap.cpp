#include "MinHeap.h"
#include "../util/logging.h"

#include <cstring>


MinHeap::MinHeap(){}

MinHeap::~MinHeap()
{
    if (_data != nullptr) delete[] _data;
}

void MinHeap::swap(int idx1, int idx2)
{
    Shape* tmp = _data[idx1];
    _data[idx1] = _data[idx2];
    _data[idx2] = tmp;
}


void MinHeap::bubble_down(unsigned int idx)
{

    if (2*idx + 1 >= _size) return; // leaf
    int child_idx1 = 2*idx + 1;
    int child_idx2 = child_idx1 + 1;

    if (child_idx1 == _size - 1)
    {
        if (_data[idx]->get_z_layer() > _data[child_idx1]->get_z_layer()){
            swap(idx, child_idx1);
        }
        return;
    }

    // both children exist
    // now we need the lower child
    int chosen = child_idx1;
    if (_data[child_idx1]->get_z_layer() > _data[child_idx2]->get_z_layer())
    {
        chosen = child_idx2; // so child2 is lower
    }

    if (_data[chosen]->get_z_layer() < _data[idx]->get_z_layer()){ // if child has lower than idx
        swap(idx, chosen);
        bubble_down(chosen);
    }
}


Shape* MinHeap::pop()
{
    if (_data == nullptr)
        logging::error("MinHeap::pop", "Can only fetch element after freezing the MinHeap");
    if (_size == 0) return nullptr;

    Shape* res = _data[0];
    

    swap(0, _size - 1);
    _data[_size - 1] = nullptr; // no hanging pointers
    _size--;
    bubble_down(0);
    
    return res;
}

void MinHeap::add(Shape* shape)
{
    if (_data != nullptr)
        logging::error("MinHeap::add", "Cannot add element after freezing.");
    _prefreeze_buffer.push_back(shape);
    _capacity++;
}

void MinHeap::freeze()
{
    if (_data != nullptr)
        logging::error("MinHeap::freeze", "freeze should be called only once.");
    _data = new Shape*[2*_capacity];
    _backup_data = _data + _capacity;
    _size = _capacity;

    memcpy(_data, _prefreeze_buffer.data(), _capacity*sizeof(void*));

    for (int i = _capacity/2; i >= 0; i--)
    {
        bubble_down(i);
    }
    
    memcpy(_backup_data, _data, _capacity*sizeof(void*));
    _prefreeze_buffer.clear();

}

void MinHeap::restore()
{
    if (_data == nullptr)
        logging::error("MinHeap::restore", "Cannot restore before freezing.");
    memcpy(_data, _backup_data, _capacity*sizeof(void*));
    _size = _capacity;
}

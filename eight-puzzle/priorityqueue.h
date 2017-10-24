#ifndef priorityqueue_H
#define priorityqueue_H

#include <iostream>
#include <map>
using namespace std;

typedef float Priority;
template< typename Item>
class priorityqueue: public multimap<Priority, Item>
{
    public:

    multimap<Priority, Item> M;
    
    priorityqueue() {}
    
    Item& front()
    {
        auto it = M.begin();
        for(auto its = M.begin(); its != M.end(); its++)
        {
            if(it->first > its->first)
            {
                it = its;
            }
        }
        return it->second;
    }
    
    void pop()
    {
        if(M.empty())
        {
            return;
        }
        auto it = M.begin();
        for(auto its = M.begin(); its != M.end(); its++)
        {
            if(it->first > its->first)
            {
                it = its;
            }
        }
        M.erase(it);
    }
    
    void push(Item x, Priority p) // can you have multiple values in same priority?
    {
        //doing this as if cant and will overwrite the thing at same priority
        M.insert(M.end(), pair<Priority,Item>(p,x));    

    
    }

    priorityqueue<Item>(const priorityqueue<Item>& X) 
    {
        this->M = X.M;
    }
    
    
    priorityqueue<Item>& operator = (const priorityqueue<Item> &PRIORITY_QUEUE)
    {
        priorityqueue<Item>* NEW = new priorityqueue<Item>;
        NEW->M = PRIORITY_QUEUE.M;
        this->M = NEW->M;
        return *NEW;
    }
    
    bool empty() //made for testing
    {
        if(M.empty())
        {
            return true;
        }
        return false;
    }
};

#endif
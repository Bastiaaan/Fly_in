
#include "fly_in.hpp"

template<class T>
class ClassFactory
{
    private:
        Data<T> collected;
        int amountCreated;
    public:
        T *create();
        ClassFactory();
        ~ClassFactory();
        void collect();
};
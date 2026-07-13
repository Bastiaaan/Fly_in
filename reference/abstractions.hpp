#ifndef ABSTRACTIONS_HPP
# define ABSTRACTIONS_HPP

template<class C>
class Data {
    public:
        static Data init();
        template <typename T>
        void insert(T &type);
        void setLimit(int id);
        int limit;
    protected:
        int index = 0;
};

#endif

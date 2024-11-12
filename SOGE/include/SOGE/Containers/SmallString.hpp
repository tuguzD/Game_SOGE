#ifndef SOGE_CONTAINER_SMALLSTRING_HPP
#define SOGE_CONTAINER_SMALLSTRING_HPP


namespace soge
{
    class SmallString
    {
        using sizeType = eastl_size_t;

    private:
        sizeType m_capacity;

        union
        {
            struct
            {
                char* ptr;
                sizeType size;
            } heapbuf;

            char stackbuf[sizeof(heapbuf)];
        };

    public:
        SmallString();
        SmallString(const char* aString);
        SmallString(eastl::string aString);
        SmallString(std::string aString);
        SmallString(SmallString& aOtherSmallString);
        ~SmallString();

    };
}

#endif // !SOGE_CONTAINER_SMALLSTRING_HPP

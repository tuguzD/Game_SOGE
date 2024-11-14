#ifndef SOGE_CONTAINERS_LWSTRING_HPP
#define SOGE_CONTAINERS_LWSTRING_HPP


namespace soge
{
    class LWString
    {
        using bufferType = eastl::unique_ptr<char>;
        using sizeType = eastl_size_t;

    private:
        char* m_stringBuffer;

    private:
        void set(const char* aCString);
        void reserve(int aCount);

        char* local_buf();
        const char* local_buf() const;
        bool is_using_local_buf();

    public:
        LWString();
        LWString(const char* aCString);
        LWString(const LWString& aLWString);
        LWString(const std::string& aSTDString);
        LWString(const eastl::string& aEASTLString);
        ~LWString();

        char* c_str();
        const char* c_str() const;
        std::string std_str() const;
        eastl::string eastl_str() const;

        void clear();
        int length();

    public:
        char& operator[](sizeType aIndex);
        char operator[](sizeType aIndex) const;

        LWString& operator=(const char* aCString);
        LWString& operator=(const LWString& aLWString);
        LWString& operator=(const std::string& aSTDString);
        LWString& operator=(const eastl::string& aEASTLString);

        bool operator==(const char* aCString) const;
        bool operator==(const LWString& aLWString) const;
        bool operator==(const std::string& aSTDString) const;
        bool operator==(const eastl::string& aEASTLString) const;

    };
}

#endif // !SOGE_CONTAINERS_LWSTRING_HPP

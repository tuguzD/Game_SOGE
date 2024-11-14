#include "sogepch.hpp"
#include "SOGE/Containers/LWString.hpp"


namespace soge
{
    LWString::LWString()
    {
        m_stringBuffer = new char('\0');
    }

    LWString::LWString(const char* aCString) : LWString()
    {
        set(aCString);
    }

    LWString::LWString(const LWString& aLWString) : LWString()
    {
        set(aLWString.c_str());
    }

    LWString::LWString(const std::string& aSTDString) : LWString()
    {
        set(aSTDString.c_str());
    }

    LWString::LWString(const eastl::string& aEASTLString) : LWString()
    {
        set(aEASTLString.c_str());
    }

    LWString::~LWString()
    {
        if (!is_using_local_buf())
        {
            free(m_stringBuffer);
        }
    }

    void LWString::set(const char* aCString)
    {
        int bufferLength = static_cast<int>(strlen(aCString));
        if (length() < bufferLength)
            reserve(bufferLength);
        memcpy(m_stringBuffer, aCString, static_cast<sizeType>(bufferLength + 1));
    }

    void LWString::reserve(int aCount)
    {
        if (aCount <= length() - 1)
            m_stringBuffer = local_buf();
        else
            m_stringBuffer = (char*)malloc((sizeType)(aCount + 1) * sizeof(char));
    }

    char* LWString::local_buf()
    {
        return (char*)this + sizeof(LWString);
    }

    const char* LWString::local_buf() const
    {
        return (char*)this + sizeof(LWString);
    }

    bool LWString::is_using_local_buf()
    {
        return m_stringBuffer == local_buf() && length() != 0;
    }

    char* LWString::c_str()
    {
        return m_stringBuffer;
    }

    const char* LWString::c_str() const
    {
        return m_stringBuffer;
    }

    std::string LWString::std_str() const
    {
        return std::string(c_str());
    }

    eastl::string LWString::eastl_str() const
    {
        return eastl::string(c_str());
    }

    void LWString::clear()
    {
        if (!is_using_local_buf())
            free(m_stringBuffer);
        m_stringBuffer = new char('\0');
    }

    int LWString::length()
    {
        return static_cast<int>(strlen(m_stringBuffer));
    }

    char& LWString::operator[](sizeType aIndex)
    {
        return m_stringBuffer[aIndex];
    }

    char LWString::operator[](sizeType aIndex) const
    {
        return m_stringBuffer[aIndex];
    }

    LWString& LWString::operator=(const char* aCString)
    {
        set(aCString);
        return *this;
    }

    LWString& LWString::operator=(const LWString& aLWString)
    {
        set(aLWString.c_str());
        return *this;
    }

    LWString& LWString::operator=(const std::string& aSTDString)
    {
        set(aSTDString.c_str());
        return *this;
    }

    LWString& LWString::operator=(const eastl::string& aEASTLString)
    {
        set(aEASTLString.c_str());
        return *this;
    }

    bool LWString::operator==(const char* aCString) const
    {
        return strcmp(c_str(), aCString) == 0;
    }

    bool LWString::operator==(const LWString& aLWString) const
    {
        return strcmp(c_str(), aLWString.c_str()) == 0;
    }

    bool LWString::operator==(const std::string& aSTDString) const
    {
        return strcmp(c_str(), aSTDString.c_str()) == 0;
    }

    bool LWString::operator==(const eastl::string& aEASTLString) const
    {
        return strcmp(c_str(), aEASTLString.c_str()) == 0;
    }


}

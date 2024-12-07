#include "sogepch.hpp"

#include "SOGE/Input/KeyMapManager.hpp"


namespace soge
{
    SGScanCode KeyMapManager::GetScanCodeFromKey(const Key& aKey)
    {
        if (!aKey.IsValid())
        {
            return 0;
        }

        for (const auto& [scanCode, key] : m_keyMap)
        {
            if (key == aKey)
            {
                return scanCode;
            }
        }
        return 0;
    }

    const Key& KeyMapManager::GetKeyFromScanCode(const SGScanCode aScanCode)
    {
        if (const Key& key = m_keyMap[aScanCode]; key.IsValid())
        {
            return key;
        }
        return Keys::UndefinedKey;
    }

    void KeyMapManager::SetScanCode(const SGScanCode aScanCode, const Key& aKey)
    {
        if (aKey.IsValid())
        {
            m_keyMap[aScanCode] = aKey;
        }
    }
}

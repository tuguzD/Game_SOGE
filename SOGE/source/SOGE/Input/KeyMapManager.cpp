#include "sogepch.hpp"

#include "SOGE/Input/KeyMapManager.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, KeyMapManager.hpp)


namespace soge
{
    SharedPtr<KeyMapManager> KeyMapManager::s_instance;

    KeyMapManager* KeyMapManager::GetInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = SharedPtr<KeyMapManager>(new KeyMapManager());
        }

        return s_instance.get();
    }

    KeyMapManager::KeyMapManager()
    {
        m_keyMapManagerImpl = CreateShared<ImplKeyMapManager>();
        m_keyMapManagerImpl->SetupKeyMappings();
    }

    void KeyMapManager::SetupKeyMappings()
    {
        m_keyMapManagerImpl->SetupKeyMappings();
    }

    SGScanCode KeyMapManager::GetScanCodeFromKey(const Key& aKey)
    {
        return m_keyMapManagerImpl->GetScanCodeFromKey(aKey);
    }

    const Key& KeyMapManager::GetKeyFromScanCode(const SGScanCode aScanCode)
    {
        return m_keyMapManagerImpl->GetKeyFromScanCode(aScanCode);
    }

    /////////////////////////
    // KeyMapManagerImpl
    /////////////////////////

    void impl::KeyMapManagerImpl::SetScanCode(SGScanCode aScanCode, const Key& aKey)
    {
        if (aKey.IsValid())
        {
            m_keyMap.insert(KeyValuePair(aScanCode, aKey));
        }
    }

    SGScanCode impl::KeyMapManagerImpl::GetScanCodeFromKey(const Key& aKey)
    {
        // TODO: returns 0 now
        return SGScanCode();
    }

    const Key& impl::KeyMapManagerImpl::GetKeyFromScanCode(const SGScanCode aScanCode)
    {
        const Key& key = m_keyMap[aScanCode];
        if (key.IsValid())
        {
            return key;
        }

        return Keys::UndefinedKey;
    }
}

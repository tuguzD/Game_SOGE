#ifndef SOGE_INPUT_KEYMAPMANAGER_HPP
#define SOGE_INPUT_KEYMAPMANAGER_HPP

#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class KeyMapManager
    {
    private:
        eastl::map<SGScanCode, Key> m_keyMap;

    protected:
        void SetScanCode(SGScanCode aScanCode, const Key& aKey);

    public:
        explicit KeyMapManager() = default;

        explicit KeyMapManager(const KeyMapManager&) = delete;
        KeyMapManager& operator=(const KeyMapManager&) = delete;

        explicit KeyMapManager(KeyMapManager&&) noexcept = default;
        KeyMapManager& operator=(KeyMapManager&&) noexcept = default;

        virtual ~KeyMapManager() = default;

        SGScanCode GetScanCodeFromKey(const Key& aKey);
        const Key& GetKeyFromScanCode(SGScanCode aScanCode);
    };
}

#endif // SOGE_INPUT_KEYMAPMANAGER_HPP

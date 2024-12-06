#ifndef SOGE_INPUT_KEYMAPMANAGER_HPP
#define SOGE_INPUT_KEYMAPMANAGER_HPP

#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    namespace impl
    {
        class KeyMapManagerImpl
        {
        private:
            using KeyValuePair = eastl::pair<SGScanCode, const Key>;

            eastl::map<SGScanCode, const Key> m_keyMap;

        protected:
            void SetScanCode(SGScanCode aScanCode, const Key& aKey);

        public:
            virtual ~KeyMapManagerImpl() = default;

            virtual void SetupKeyMappings() = 0;
            SGScanCode GetScanCodeFromKey(const Key& aKey);
            const Key& GetKeyFromScanCode(SGScanCode aScanCode);
        };
    }

    class KeyMapManager
    {
    private:
        SharedPtr<impl::KeyMapManagerImpl> m_keyMapManagerImpl;

    protected:
        static SharedPtr<KeyMapManager> s_instance;

        KeyMapManager();

    public:
        static KeyMapManager* GetInstance();

        KeyMapManager(const KeyMapManager&) = delete;
        KeyMapManager& operator=(const KeyMapManager&) = delete;

        KeyMapManager(KeyMapManager&&) = delete;
        KeyMapManager& operator=(KeyMapManager&&) = delete;

        ~KeyMapManager() = default;

        void SetupKeyMappings();
        SGScanCode GetScanCodeFromKey(const Key& aKey);
        const Key& GetKeyFromScanCode(SGScanCode aScanCode);
    };
}

#endif // SOGE_INPUT_KEYMAPMANAGER_HPP

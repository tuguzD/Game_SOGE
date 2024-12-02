#ifndef SOGE_INPUT_KEYMAPMANAGER_HPP
#define SOGE_INPUT_KEYMAPMANAGER_HPP

#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    namespace impl
    {
        class KeyMapManagerImpl
        {
            using KeyValuePair = eastl::pair<SGScanCode, const Key>;

        private:
            eastl::map<SGScanCode, const Key> m_keyMap;

        protected:
            void SetScanCode(SGScanCode aScanCode, const Key aKey);

        public:
            virtual ~KeyMapManagerImpl() = default;

            virtual void SetupKeyMappings() = 0;
            SGScanCode GetScanCodeFromKey(const Key aKey);
            const Key& GetKeyFromScanCode(SGScanCode aScanCode);

        };
    }

    class KeyMapManager
    {
    private:
        SharedPtr<impl::KeyMapManagerImpl> m_keyMapManagerImpl;

    protected:
        KeyMapManager();
        static SharedPtr<KeyMapManager> s_instance;

    public:
        KeyMapManager(KeyMapManager&)   = delete;
        KeyMapManager(KeyMapManager&&)  = delete;

        auto operator=(KeyMapManager&)  = delete;
        auto operator=(KeyMapManager&&) = delete;
        ~KeyMapManager() = default;

        void SetupKeyMappings();
        SGScanCode GetScanCodeFromKey(const Key aKey);
        const Key& GetKeyFromScanCode(SGScanCode aScanCode);

    public:
        static KeyMapManager* GetInstance();

    };
}

#endif // !SOGE_INPUT_KEYMAPMANAGER_HPP

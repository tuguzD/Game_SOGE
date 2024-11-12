#ifndef SOGE_INPUT_KEYBOARD_HPP
#define SOGE_INPUT_KEYBOARD_HPP

#include "SOGE/Input/InputDevice.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    class Keyboard : public InputDevice
    {
    private:


    public:
        Keyboard(eastl::string aName);
        virtual ~Keyboard() = default;

        /**
        * @brief function to check if key is pressed.
        * @returns true if pressed else false.
        */
        virtual bool IsKeyPressed() = 0;

         /**
         * @brief function to check if key is released.
         * @returns true if released else false.
         */
        virtual bool IsKeyReleased() = 0;

         /**
         * @brief function to check if key is pressed.
         * @returns true if pressed else false.
         */
        virtual eastl::string KeyToString() = 0;

    };
}

#endif // !SOGE_INPUT_KEYBOARD_HPP

#ifndef SOGE_INPUT_INPUTMANAGER_HPP
#define SOGE_INPUT_INPUTMANAGER_HPP


namespace soge
{
    class Keyboard;
    //class Mouse;
    //class Gamepad;

    class InputManager
    {
    public:
        InputManager()          = default;
        virtual ~InputManager() = default;

        virtual void LockInput(bool aLockInput)             = 0;
        virtual void UseRelativeMouseMode(bool aRelMouse)   = 0;
        virtual void BeginUpdateInput()                     = 0;
        virtual void EndUpdateInput()                       = 0;


        virtual Keyboard* CreateKeyboard()  = 0;
        //virtual Mouse* CreateMouse()        = 0;
        //virtual Gamepad* CreateGamepad()    = 0;
    };
}

#endif // SOGE_INPUT_INPUTMANAGER_HPP

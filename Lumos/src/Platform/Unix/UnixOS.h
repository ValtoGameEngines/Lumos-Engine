#include "lmpch.h"
#include "Core/OS/OS.h"

namespace Lumos
{
    class UnixOS : public OS
    {
    public:
        UnixOS() = default;
        ~UnixOS() = default;

        void Init();
        void Run() override;
        String GetExecutablePath() override { return ""; }

    };
}

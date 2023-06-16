//Makes sure the correct version of the ZScript interpreter is called based on
//the quest file's version, thus allowing for a re-write of the interpreter,
//which would otherwise break scripts from the LI build
//~Joe123

#ifndef _ZSCRIPTVER_H
#define _ZSCRIPTVER_H

#include "base/zdefs.h"
#include "zc/ffscript.h"

extern FFScript FFCore;

class ZScriptVersion
{
public:
    static inline void setVersion(int32_t newVersion)
    {
        CurrentVersion = newVersion;
    }
    
    static inline int32_t RunScript(const byte type, const word script, const int32_t i = -1)
    {
        return run_script(type, script, i);
    }
    
    static inline void RunScrollingScript(int32_t scrolldir, int32_t cx, int32_t sx, int32_t sy, bool end_frames, bool waitdraw)
    {
        if (CurrentVersion >= 6)
            ScrollingScript(scrolldir, cx, sx, sy, end_frames,waitdraw);
    }
    
private:
    static int32_t CurrentVersion;
    
    static void ScrollingScript(int32_t scrolldir, int32_t cx, int32_t sx, int32_t sy, bool end_frames, bool waitdraw);
};

#endif

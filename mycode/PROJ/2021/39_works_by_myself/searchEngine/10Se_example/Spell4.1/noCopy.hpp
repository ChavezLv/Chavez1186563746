#pragma once

namespace wd{
class noCopy
{
protected:
    noCopy() {}
    ~noCopy() {}
    
    noCopy& operator=(const noCopy&)=delete;
    noCopy(const noCopy&)=delete;
};

}

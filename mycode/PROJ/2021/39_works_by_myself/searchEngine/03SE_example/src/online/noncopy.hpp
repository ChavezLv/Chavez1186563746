#ifndef __RXL_NONCOPY_H__
#define __RXL_NONCOPY_H__
namespace rxl{
struct Noncopy{
protected:
	Noncopy(){};
	~Noncopy(){};
	Noncopy(const Noncopy&) = delete;
	Noncopy& operator=(const Noncopy&) = delete;
};
}
#endif

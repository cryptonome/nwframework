#ifndef DSUTILSOBJ_H_
#define DSUTILSOBJ_H_

struct IBaseFilter;
struct IPin;

//********************************************************************
// DS_RELEASE macro
//********************************************************************
#define DS_RELEASE(var)     \
                            if ( var != 0 ) \
                            { \
                                var->Release(); \
                                var = 0; \
                            }


//********************************************************************
//
//********************************************************************
namespace DSUtils
{
    ////////////////////////
    // DSObj
    ////////////////////////
    template <typename T>
    class DSObj
    {
    public:
        DSObj(T* _obj) : mObj(_obj) { mObj->AddRef(); }
        DSObj(const DSObj<T>& _obj) { mObj = _obj.getObj(); mObj->AddRef(); }
        ~DSObj() { DS_RELEASE(mObj); }

        DSObj& operator= (const DSObj& obj)
        {
            mObj = obj.getObj();
            mObj->AddRef();
            return *this;
        }

        T* getObj() const { return mObj; }
    private:
        T* mObj;
    };
    typedef DSObj<IBaseFilter> DSFilter;
    typedef std::list<DSFilter> DSFilterList;
    typedef DSObj<IPin> DSPin;
    typedef std::list<DSPin> DSPinList;
} // DSUtils

#endif

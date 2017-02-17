#ifndef SINGLETON_H
#define SINGLETON_H

#include "mylibrary_global.h"

template <class T>
class MYLIBRARYSHARED_EXPORT singleton
{
public:
    static T* md_p_instance;


    explicit singleton(){ }
    virtual ~singleton(){ }
        //singleton(singleton const &);
        //singleton &operator=(singleton const &);
    public:
        static void destroy_instance()
        {
            if(md_p_instance != 0) {
                //delete md_p_instance;
                //md_p_instance= 0;
            }
        }

        static T* get_instance()
        {
            if(md_p_instance == 0)
            {
                md_p_instance = new T();
            }

            return md_p_instance;
        }


};

template<class T>
T* singleton<T>::md_p_instance = 0;

#define SINGLETON(T) singleton< T >::get_instance()
#define SINGLETON_DESTROY(T) singleton< T >::destroy_instance()



//#define SINGLETON(T) singleton< T >::mf_sr_get_instance()
//#define SINGLETON_DELETE(T) singleton< T >::mf_s_destroy_singleton()
//template< typename T>T* singleton<T>::md_p_instance = 0;
//#define SINGLETON(T) singleton< T >::mf_sr_get_instance()

#endif // SINGLETON_H

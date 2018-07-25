#pragma once
#include <vector>
#include "../core/gxDebug.h"

#define REF_RELEASE(x)  {if(x){ x->release();   }}
#define REF_RETAIN(x)   {if(x){ x->retain();    }}

namespace GEAR{
    namespace Memory{

        #pragma mark Ref
        class DECLSPEC Ref
        {
        public:
            virtual ~Ref();
            
            void autoRelease();
            void release();
            void retain();
            int getRetainCount()    {   return retainCount; }
            
        protected:
            Ref();
            
        private:
            int retainCount;
        };

        #pragma mark AutoReleasePool
        class DECLSPEC AutoReleasePool
        {
        public:
            static AutoReleasePool& getInstance();
            
            ~AutoReleasePool();
            
            void clearPool();
            
            friend void Ref::autoRelease();
            friend void Ref::release();
            
            int getRefCount()   {   return (int)memoryPool.size();   }
            const std::string toString();
            
        private:
            void destroyAllFromPool();
            bool addRefToPool(Ref* ref);
            bool destroyRefFromPool(Ref* ref);

            std::vector<Ref*> memoryPool;
            std::vector<Ref*> destroyPool;
        };
    }//namespace Memory
}//namespace GEAR
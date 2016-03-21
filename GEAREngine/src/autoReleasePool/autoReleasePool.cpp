#include "autoReleasePool.h"
#include <assert.h>
#include <algorithm>
#include <sstream>

namespace GEAR{
    namespace Memory{

        #pragma mark Ref
        Ref::Ref()
        {
            retainCount=0;
        }

        Ref::~Ref()
        {
            assert(retainCount==0);
        }

        void Ref::autoRelease()
        {
            AutoReleasePool::getInstance().addRefToPool(this);
        }

        void Ref::release()
        {
            retainCount--;
            
            if(retainCount<0)
            {
                DEBUG_PRINT("INFO : (Ref) - retainCount cannot be less than zero !!!.");
                assert(false);
            }
            else if(retainCount==0)
            {
                AutoReleasePool::getInstance().destroyRefFromPool(this);
            }
        }

        void Ref::retain()
        {
            retainCount++;
        }
        
        #pragma mark AutoReleasePool
        AutoReleasePool& AutoReleasePool::getInstance()
        {
            static AutoReleasePool instance;
            return instance;
        }
        
        AutoReleasePool::~AutoReleasePool()
        {
            destroyAllFromPool();
        }
        
        bool AutoReleasePool::addRefToPool(Ref* ref)
        {
            if(std::find(memoryPool.begin(), memoryPool.end(), ref)!=memoryPool.end())
            {
                DEBUG_PRINT("ERROR : Ref object already added to pool.");
                return false;
            }
            
            memoryPool.push_back(ref);
            return true;
        }
        
        bool AutoReleasePool::destroyRefFromPool(Ref* ref)
        {
            //check if the Ref object is in memory pool or not.
            if(std::find(memoryPool.begin(), memoryPool.end(), ref)==memoryPool.end())
            {
                DEBUG_PRINT("ERROR : Couldn't find the Ref object to push in to destroy pool. Can't delete this object. Please check !!!.");
                return false;
            }
            
            memoryPool.erase(std::remove(memoryPool.begin(), memoryPool.end(), ref));
            
            //check if the Ref object already in destroy pool or not.
            if(std::find(destroyPool.begin(), destroyPool.end(), ref)!=destroyPool.end())
            {
                DEBUG_PRINT("ERROR : Ref objet already in destroy pool. It may be get deleted on next clearPool().");
                return false;
            }
            
            
            DEBUG_PRINT("INFO : Ref objet added to destroy pool. It may be get deleted on next clearPool().");
            destroyPool.push_back(ref);
            return true;
        }
        
        void AutoReleasePool::clearPool()
        {
            for (auto ref : destroyPool)
            {
                GX_DELETE(ref);
            }
            destroyPool.clear();
        }

        void AutoReleasePool::destroyAllFromPool()
        {
            while (memoryPool.size())
            {
                destroyRefFromPool(memoryPool[0]);
            }
            
            clearPool();
        }
        
        const std::string AutoReleasePool::toString()
        {
            std::string str;
            std::stringstream stringStream;
            stringStream << "[" << memoryPool.size() <<"] ";
            
            for (auto ref : memoryPool)
            {
                stringStream << "{" << ref->getRetainCount() <<"}";
            }
            
            str = stringStream.str();
            
            return str;
        }
    }//namespace Memory
}//namespace GEAR

#pragma once
#include <vector>
#include "../core/gxDebug.h"

namespace GEAR{
    namespace Memory{

        #pragma mark Ref
        class Ref
        {
        public:
            virtual ~Ref();
            
            void autoRelease();
            void release();
            void retain();

        protected:
            Ref();
            
        private:
            int retainCount;
        };

        #pragma mark AutoReleasePool
        class AutoReleasePool
        {
        public:
            static AutoReleasePool& getInstance();
            
            ~AutoReleasePool();
            
            void clearPool();
            void destroyAllFromPool();
            
            friend void Ref::autoRelease();
            friend void Ref::release();
            
        private:
            bool addRefToPool(Ref* ref);
            bool destroyRefFromPool(Ref* ref);

            std::vector<Ref*> memoryPool;
            std::vector<Ref*> destroyPool;
        };
    }//namespace Memory
}//namespace GEAR

#pragma once

namespace actors
{

    namespace updateable
    {

        class Updateable
        {
        public:
            virtual void update() = 0;
        };
    }

}
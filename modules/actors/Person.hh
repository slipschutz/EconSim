#pragma once

#include "Updateable.hh"
#include <string>

namespace actors
{
    namespace person
    {
        class Person : public updateable::Updateable
        {
        private:
            const int age;
            const std::string name;

        public:
            Person(int a, const std::string &name) : age(a),
                                                     name(name)
            {
            }

            void update()
            {
            }

            const std::string& getName() {
                return name;
            }

            void foo();
        };
    }
}
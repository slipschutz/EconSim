
#include <actors/Person.hh>

#include <iostream>

int main() {

    actors::person::Person temp(33, "Sam");

    std::cout<<" HI "<< temp.getName()<<"\n";
    temp.foo();

}
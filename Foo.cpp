#include <iostream>

#include "Foo.h"

Foo::Foo() {
    std::cout << "Foo ctor" << std::endl;
}

Foo::~Foo() {
    std::cout << "Foo dtor" << std::endl;
}

Bar::Bar() {
    std::cout << "Bar ctor" << std::endl;
}

Bar::~Bar() {
    std::cout << "Bar dtor" << std::endl;
}

// Use to debug when dtors get called.
void foo() {
    Foo foo;

    std::cout << "Foo about to be destroyed" << std::endl;
}
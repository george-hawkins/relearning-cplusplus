#ifndef RELEARNING_CPLUSPLUS_FOO_H
#define RELEARNING_CPLUSPLUS_FOO_H


class Bar {
public:
    Bar();
    ~Bar();
};

class Foo {
public:
    Foo();
    ~Foo();

private:
    Bar bar;
};

extern void foo();

#endif //RELEARNING_CPLUSPLUS_FOO_H

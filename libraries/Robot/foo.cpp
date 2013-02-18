/*
  Foo.cpp - 
  Released into the public domain.
*/

#include <Foo.h>

int Foo::id=111;

Foo::Foo() {
  id++;
}

void Foo::fah(){
  Serial.println("foo " + String(Foo::id));
}

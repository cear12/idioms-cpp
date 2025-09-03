class NoHeap {
protected:
  static void * operator new(std::size_t);     // #1: To prevent allocation of scalar objects
  static void * operator new [] (std::size_t); // #2: To prevent allocation of array of objects
};

class NoHeapTwo : public NoHeap {
};

int main(void) {
  new NoHeap;        // Not allowed because of #1
  new NoHeap[1];     // Not allowed because of #2
  new NoHeapTwo[10]; // Not allowed because of inherited protected new operator (#2).
}
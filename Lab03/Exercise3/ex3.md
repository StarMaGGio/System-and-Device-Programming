### Reply to the following questions:

01. What is dynamic memory allocation in C++?

    `The dinamic memory allocation in C++ refers to allocating and deallocating memory to variables directly at runtime and not at compile time. This is done using the key operators "new" and "delete".`

02. Explain the difference between stack memory and heap memory.

    `The Stack memory is used to store Non-static objects, like ones defined inside functions, that have a duration that depends on their scope. This part of memory is automatically managed by the compiler.`

    `The Heap memory is the part of memory used to store objects in a dynamic way. Unlike the stack, this memory remain allocated untill it is explicitly released in the code.`

03. What is the new operator in C++, and how does it relate to dynamic memory allocation?

    `The "new" operator is a special operator used to allocate a block of memory compatible with the type and the size of the object defined. Also it calls the constructor of that object to create it and return a pointer to that object.`

04. What is the purpose of the delete operator in C++?

    `The "delete" operator is used to free previously allocated memory. It calls the destructor of the specified object and deallocate the memory in which it was stored.`

05. What are memory leaks, and how can dynamic memory allocation contribute to them?

    `Memory leaks happens when a dynamic object is not explicitly deleted and so memory is not freed. Dynamic memory allocation contribute to that because if we allocate memory with the "new" operator and we forgot to deallocate the memory with the "delete" operator, that memory will remain occupied, even if the object is no more in scope.`

06. What are smart pointers in C++, and why are they used?

    `Smart pointers are special types of pointers in C++ that have special functions and behaviours. They are used because they automatically free dynamic objects memory when they are no longer needed.`

07. Describe the differences between std::unique_ptr, std::shared_ptr, and std::weak_ptr.

    `std::unique_ptr is a unique pointer. It owns the object that it points to, and only one unique pointer can point to an object at a time. It can be moved but it can not be copied.`

    `std::shared_ptr is a shared pointer. It allows several pointers to point to the same object. Each dynamic resource has a reference count counting the number of shared pointers pointing to it. When the counter goes to zero, the resource is released automatically. Shared pointers can be either moved and copied.`

    `std::weak_ptr is a weak pointer. It do not control the lifetime of the object to which it points to. It points to an object that is managed by a shared pointer. Also it do not increase the reference counter of the dynamic resource. They can be used to break circular dependencies between shared pointers.`

08. How do smart pointers help prevent memory leaks and other memory-related issues?

    `They help preventing memory leaks by automatically releasing the memory when the allocated object is no more nedeed. They also prevent circular dependencies, with shared pointers referencing to each other and not being deleted when out of scope, through weak pointers.`

09. What is the role of the destructor in smart pointers?

    `The role of the destructor in smart pointers is to be called by them when the object they refer to in no longer needed and the memory used to store it can be released.`

10. Explain the concept of ownership semantics in the context of smart pointers.

    `The ownership semantics refers to the set of rules that define which part of the program is responsible for managing the lifetime of an object. In the context of smart pointers the semantics describe how that responsibility is handled or transferred. - Unique ownership is when a pointer has the exclusive responsibility of referencing and managing the lifetime of an object. Unique ownership can be transferred. - Shared ownership is when multiple pointers refer and manage the lifetime of an object. - Weak ownership is when a pointer refers to an object but it does not manage its deletion. It also does not increase the object reference counter.`

11. Discuss the potential pitfalls of using raw pointers in C++ and how smart pointers address these issues.

    `Potential pitfalls of using raw pointers are Dangling pointers and Memory leaks. Using smart pointers these issues can be addressed because they have special behaviours that automatically manage memory and references even if, for example, we forget to delete objects.`

12. Discuss the potential overhead associated with using smart pointers compared to raw pointers and strategies for minimizing it.

    `Potential overhead associated with using smart pointers can be circular dependencies between shared pointers. If a shared pointer A points to a shared pointer B and viceversa, when they goes out of scope, they are not deleted. Strategies to minimize this problem is using weak pointers to break these circular dependencies. `
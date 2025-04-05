## 10. Key Assembly Takeaways

This part of the book has covered the basics of assembly. While most people today code in a high-level programming language, an understanding of assembly increases a programmer’s ability to better understand what their programs and compilers are doing. A knowledge of assembly is also essential for anyone who designs software for embedded systems and other resource-constrained environments, and for people who work in vulnerability analysis. The chapters contained in the assembly part of this book have spanned 64-bit Intel assembly (x86-64), 32-bit Intel Assembly (IA32), and 64-bit ARM assembly (ARMv8-A).

### Common Features

Regardless of the specific assembly language that one learns, there are some common features to _all_ assembly languages worth highlighting.

The ISA defines the assembly language

The specific assembly language available on a machine is defined by the **instruction set architecture** (ISA) of that machine. To identify the underlying architecture of a particular Linux machine, use the `uname -m` command.

Registers hold data

Every ISA defines a set of basic **registers** that the CPU uses to operate on data. Some registers are _general purpose_ and can hold any kind of data, whereas other registers are _special purpose_ and are typically reserved by the compiler for specific uses (e.g., stack pointer, base pointer). While general purpose registers are readable and writable, some special purposes registers are read-only (e.g., the instruction pointer).

Instructions specify what the CPU can do

The ISA also defines a series of **instructions** that specify operations that the CPU can perform. Each instruction has an **operation code** (opcode) that specifies what the instruction does, and one or more **operands** that specifies the data to be used. The ISA documents specific instructions for data movement, arithmetic operations, conditionals, branches, and accessing memory. These core instructions are often combined to represent more complex data structures like arrays, structs, and matrices.

The program stack holds local variables associated with a particular function

The compiler uses the stack (or stack memory) of a process’s virtual address space to store temporary data. On all modern systems, the program stack grows toward _lower_ memory addresses. The compiler uses the stack pointer and base pointer to specify a **stack frame** that defines the area of the stack that is associated with a particular function or procedure. A new stack frame is added to the stack with every function call and defines the stack region associated with the callee function. The stack frame associated with a particular function is removed from the stack when that function returns. Typically, the stack and base pointers return to their original values when a function ends. While this bit of bookkeeping suggests that local variables are "cleaned" from the stack, old data usually sticks around in the form of junk values, which can sometimes lead to hard-to-debug behaviors. Malicious actors can also use knowledge of an ISA’s stack bookkeeping to create dangerous security exploits, like buffer overflows.

Security

While all systems are vulnerable to security vulnerabilities like buffer overflow, the relatively recent ARMv8-A has had the opportunity to learn from some of the security flaws that affected older Intel architectures. However, the first line of defense is always the programmer. Even with additional protections, no ISA is invulnerable to potential security flaws. When coding in C, programmers should use **length specifiers** whenever possible to reduce the chance of security vulnerabilities resulting from boundary overruns (see [Table 1](https://diveintosystems.org/book/C10-asm_takeaways/index.html#specifiersSummary)).

Table 1. C functions with Length Specifiers

|Instead of:|Use:|
|---|---|
|`gets(buf)`|`fgets(buf, 12, stdin)`|
|`scanf("%s", buf)`|`scanf("%12s", buf)`|
|`strcpy(buf2, buf)`|`strncpy(buf2, buf, 12)`|
|`strcat(buf2, buf)`|`strncat(buf2, buf, 12)`|
|`sprintf(buf, "%d", num)`|`snprintf(buf, 12, "%d", num)`|

### Further Reading

This book offers just a taste of some of the most popular assembly languages in use. For more in-depth understanding of assembly, we encourage you to check out the ISA specifications:

- [Intel 64 and IA32 Manuals](https://software.intel.com/en-us/articles/intel-sdm#architecture)
- [ARM Cortex-A Programmer’s Guide](https://developer.arm.com/docs/den0024/a/preface)

The following free resources may also be useful for those who are interested in learning 32-bit assembly:

- [IA32 Programming Web Aside](http://csapp.cs.cmu.edu/3e/waside/waside-ia32.pdf) (Randal Bryant and David O’Hallaron)
- [32-bit ARM Assembly](https://azeria-labs.com/writing-arm-assembly-part-1/) (Azeria Labs)

The following books also feature in-depth discussions of assembly; these aren’t free, but they are great resources for further reading:

- Intel systems: [_Computer Systems: A Programmer’s Perspective_](http://csapp.cs.cmu.edu/) (Randal Bryant and David O’Hallaron)
- ARMv8: [_Computer Organization and Design_](https://textbooks.elsevier.com/web/product_details.aspx?isbn=9780128017333) (David Patterson and John Hennessy)
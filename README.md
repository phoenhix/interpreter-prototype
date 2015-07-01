This is an early experimental prototype of a WebAssembly interpreter. It's
totally incomplete at this point.

Directory organization:

 - implementation - implementation-specific behavior
 - module - data structures for WebAssembly modules, and code for serializing and deserializing
 - process - data structures for the state of a running process
 - semantics - WebAssembly execution
 - shell - top-level shell program

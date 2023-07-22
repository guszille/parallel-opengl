# ParallelOpenGL

A basic example of how to use multiple threads in an OpenGL application.

## About

OpenGL does not support parallel processing by itself, but we can use multiple threads to execute expensive tasks without blocking, or even penalizing, the rendering loop.

So, following this proposal, this program throws the job of uploading graphics buffers data to a separate and secondary thread, while keeping the draw calls on the main one.

## Comments

1. This example is using multiple contexts, one for each thread, to dispatch OpenGL commands. This also means one command queue by context and the fact they'll need to be synchronized at some moment.
2. It's good to keep in mind that not all OpenGL objects are shared between different contexts.
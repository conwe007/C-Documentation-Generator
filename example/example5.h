#ifndef __EXAMPLE5_H
#define __EXAMPLE5_H

/**
 * This is an example function. It has no parameters and no return.
 */
void example1();

/**
 * This is an example function. It has no parameters and a return.
 * 
 * @return an arbitrary integer
 */
int example2();

/**
 * This is an example function. It has one parameter and no return.
 * 
 * @param parameter1 an integer parameter
 */
void example3(int parameter1);

/**
 * This is an example function. It has two parameters and a return.
 * 
 * @param parameter1 the first parameter
 * @param parameter2 the second parameter
 * @return an arbitrary string
 */
char* example4(char parameter1, char parameter2);

#endif

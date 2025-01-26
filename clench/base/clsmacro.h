#ifndef _CLENCH_BASE_CLSMACRO_H_
#define _CLENCH_BASE_CLSMACRO_H_

#define CLENCH_NO_COPY_CONSTRUCTOR(type) \
	type(const type &) = delete;

#define CLENCH_NO_MOVE_CONSTRUCTOR(type) \
	type(type &&) = delete;

#define CLENCH_NO_COPY_MOVE_CONSTRUCTORS(type) \
CLENCH_NO_COPY_CONSTRUCTOR(type) \
CLENCH_NO_MOVE_CONSTRUCTOR(type)

#define CLENCH_NO_COPY_ASSIGN_OPERATOR(type) \
	type &operator=(const type &) = delete;

#define CLENCH_NO_MOVE_ASSIGN_OPERATOR(type) \
	type &operator=(type &&) = delete;

#define CLENCH_NO_COPY_MOVE_ASSIGN_OPERATORS(type) \
CLENCH_NO_COPY_ASSIGN_OPERATOR(type) \
CLENCH_NO_MOVE_ASSIGN_OPERATOR(type)

#define CLENCH_NO_COPY_MOVE_METHODS(type) \
CLENCH_NO_COPY_MOVE_CONSTRUCTORS(type) \
CLENCH_NO_COPY_MOVE_ASSIGN_OPERATORS(type)

#endif

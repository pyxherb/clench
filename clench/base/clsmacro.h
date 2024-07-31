#ifndef _CLENCH_BASE_CLSMACRO_H_
#define _CLENCH_BASE_CLSMACRO_H_

#define NO_COPY_CONSTRUCTOR(type) \
	type(const type &) = delete;

#define NO_MOVE_CONSTRUCTOR(type) \
	type(type &&) = delete;

#define NO_COPY_MOVE_CONSTRUCTOR(type) \
NO_COPY_CONSTRUCTOR(type) \
NO_MOVE_CONSTRUCTOR(type)

#define NO_COPY_ASSIGN_OPERATOR(type) \
	type &operator=(const type &) = delete;

#define NO_MOVE_ASSIGN_OPERATOR(type) \
	type &operator=(type &&) = delete;

#define NO_COPY_MOVE_ASSIGN_OPERATOR(type) \
NO_COPY_ASSIGN_OPERATOR(type) \
NO_MOVE_ASSIGN_OPERATOR(type)

#define NO_COPY_MOVE_METHODS(type) \
NO_COPY_MOVE_CONSTRUCTOR(type) \
NO_COPY_MOVE_ASSIGN_OPERATOR(type)

#endif
#ifndef __LAB2_H__
#define __LAB2_H__

int intcmp(const void *a, const void *b);
void print_usage_string(void);
void child_process(int level, int num_children);
void parent_process(int level, int num_children);

#endif

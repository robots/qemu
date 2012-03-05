#ifndef GCA_H
#define GCA_H

#ifdef NEED_CPU_H
unsigned int gca_threadlist_create(CPUState *cpu);
unsigned int gca_threadlist_count(CPUState *cpu);
unsigned int gca_threadlist_getnext(CPUState *cpu);

unsigned int gca_thread_getinfo(CPUState *cpu, target_ulong id, char *buf, target_ulong *len);
target_ulong gca_thread_getcurrent(CPUState *cpu);
int gca_thread_isalive(CPUState *cpu, target_ulong id);

/*
target_ulong gca_thread_getregister(CPUState *cpu, unsigned int id, uint8_t *data);
target_ulong gca_thread_setregister(CPUState *cpu, unsigned int id, uint8_t *data);
*/
int gca_thread_regs_read(CPUState *cpu, target_ulong id, uint8_t *regs, target_ulong *len);
int gca_thread_regs_write(CPUState *cpu, target_ulong id, uint8_t *regs, target_ulong len);
int gca_thread_mem_read(CPUState *cpu, target_ulong id, target_ulong addr, uint8_t *mem, target_ulong len);
int gca_thread_mem_write(CPUState *cpu, target_ulong id, target_ulong addr, uint8_t *mem, target_ulong len);

unsigned int gca_symbol_getunknown(char *name, target_ulong *len);
unsigned int gca_symbol_add(const char *name, target_ulong addr);
#endif

int gca_active(void);
int gca_init(const char *file);
void gca_destroy(void);

#endif

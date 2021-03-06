#include <ksyscalls.h>
#include <manager.h>
#include <sysio.h>
#include <sysconsole.h>
#include "video_mode.h"
#include "sound.h"
#include "process.h"
#include "scheduler.h"
#include <ipc.h>
#include <ports.h>
#include <pit.h>

extern unsigned int manage_read(unsigned int fd, char * buffer, unsigned int length);
extern unsigned int manage_write(unsigned int fd, const char * string, unsigned int length);
extern int manage_terminal(int operation, int value);
extern void * manage_alloc(int op, ...);

syscall_st * syscalls_table[_SYSCALLS_SIZE] = {
	[_SYSCALL_READ] = syscall_read,
	[_SYSCALL_WRITE] = syscall_write,
	[_SYSCALL_TERMINAL_SELECT] = syscall_terminal_select,
	[_SYSCALL_TERMINAL_CLEAR] = syscall_terminal_clear,
	[_SYSCALL_TERMINAL_COLOR] = syscall_terminal_color,
	[_SYSCALL_TERMINAL_CURSOR] = syscall_terminal_cursor,
	[_SYSCALL_VIDEO_MODE] = syscall_set_video_mode,
	[_SYSCALL_PAINT_PIXEL] = syscall_paint_pixel,
	[_SYSCALL_SOUND] = syscall_sound,
	[_SYSCALL_NEW_PROCESS] = syscall_new_process,
	[_SYSCALL_PS] = syscall_ps,
	[_SYSCALL_KILL_PROCESS] = syscall_kill_process,
	[_SYSCALL_BLOCK_PROCESS] = syscall_block_process,
	[_SYSCALL_WAITPID] = syscall_waitpid,
	[_SYSCALL_GET_PID] = syscall_get_pid,
	[_SYSCALL_READ_MESSAGE] = syscall_read_message,
	[_SYSCALL_SEND_MESSAGE] = syscall_send_message,
	[_SYSCALL_HAS_MESSAGE] = syscall_has_message,
	[_SYSCALL_PLAY_NOTE] = syscall_play_note,
	[_SYSCALL_SLEEP] = syscall_sleep,
	[_SYSCALL_RAND] = syscall_rand,
	[_SYSCALL_ALLOC] = syscall_alloc,
	// (3) FUTURE SYSCALL HERE !! REMEMBER TO CHANGE _SYSCALLS_LAST !!
};

uint64_t syscall_read(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int fd = (int) rdi;
	char * buffer = (char *) rsi;
	unsigned int count = (unsigned int) rdx;

	return manage_read(fd, buffer, count);
}

uint64_t syscall_write(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int fd = (unsigned int) rdi;
	const char * string = (const char *) rsi;
	unsigned int count = (unsigned int) rdx;

	return manage_write(fd, string, count);
}

uint64_t syscall_terminal_select(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int index = (int) rdi;

	return manage_terminal(_TERMINAL_OPERATION_SELECT, index);
}

uint64_t syscall_terminal_clear(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	return manage_terminal(_TERMINAL_OPERATION_CLEAR, 0);
}

uint64_t syscall_terminal_color(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int operation = (int) rdi;
	int color = (int) rsi;

	return manage_terminal(operation, color);
}

uint64_t syscall_terminal_cursor(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int cursor = (int) rdi;

	return manage_terminal(_TERMINAL_OPERATION_CURSOR, cursor);
}

uint64_t syscall_alloc(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	int op = (int) rdi;

	return (uint64_t) manage_alloc(op, rsi, rdx);
}

uint64_t syscall_set_video_mode(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	return SetVideoMode();
}


uint64_t syscall_paint_pixel(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	
	int x = (int) rdi;
	int y = (int) rsi;
	char blue = (char) rdx;
	char green = (char) r10;
	char red = (char) r8;
	
	paintPixel(x, y, blue, green, red);

	return 1;
}

uint64_t syscall_sound(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	unsigned int frequency = (unsigned int) rdi;
	double time = (double) rsi;
	return sound_beep(frequency, time);
}

uint64_t syscall_new_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	return create_process((char*) rdi, (process_func) rsi, rdx, (char**) r10);
}

uint64_t syscall_ps(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	printProcesses();
	return 1;
}

uint64_t syscall_kill_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return killProcess(rdi);
}

uint64_t syscall_block_process(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return blockProcess((int)rdi);
}

uint64_t syscall_waitpid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return kwaitpid((int)rdi);
}

uint64_t syscall_get_pid(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return (uint64_t) getCurrentPid();
}

uint64_t syscall_read_message(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return (uint64_t) read_next_message(rdi);
}

uint64_t syscall_send_message(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	new_message(rdi, rsi, rdx, (void*) r10);
	return 1;
}

uint64_t syscall_play_note(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	_song_note(rdi, rsi);
	return 1;
}
uint64_t syscall_sleep(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	double milisec = (double)rdi;
	pit_wait(milisec/1000);
	return 1;
}
uint64_t syscall_rand(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return getRand();
}

uint64_t syscall_has_message(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9){
	return has_message(rdi);
}





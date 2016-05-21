/*
 *
 *		interface to simulator UI using shared memory map
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "sharedmem.h" 

struct int_bounds {
  int left;
  int right;
  char dir;
  unsigned int len;
};

struct ghdl_string {
  char *base;
  struct int_bounds *bounds;
};

int fd;
char* FILEPATH = SHAREDMEMORYFILE;	
void *map = NULL;

int init(int dummy)
{

	int result;
	struct SharedMemoryLayout* shm;
	int pid;
    char* const* argv = {NULL};

	/*	create the mmap'ed file we will use to communicate 
		with the gui						*/

	/*	1. see if file exists (simulation already running; exit)
									*/
    if(access(FILEPATH,F_OK) != -1)    
    {
		perror("Shared memory file exists. Another simulation is already running");
		exit(EXIT_FAILURE);
	}

	/*	2. open the file	*/

	fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    	if (fd == -1) {
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
    	}	

	/*	3. grow file	*/
	result = lseek(fd, SHAREDMEMORYSIZE-1, SEEK_SET);
   	if (result == -1) {
		close(fd);
		perror("Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
    }

	/*	4. write through at end of file	*/

	result = write(fd, "", 1);
    if (result != 1) {
		close(fd);
		perror("Error writing last byte of the file");
		exit(EXIT_FAILURE);
    }

	/*	5. mmap		*/

	map = mmap(0, SHAREDMEMORYSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }	

	/*	6. initialise memory	*/
	
	shm = (struct SharedMemoryLayout*)map;

	shm->requestUIExit = 0;
	shm->uiExited = 0;	

	for (int i = 0; i < 10; i++)
	{
		shm->redLEDs[i] = 1;	
		shm->slideSwitches[i] = 0;
	}

	for (int i = 0; i < 10; i++)
	{
		shm->greenLEDs[i] = 1;	
	}

	for (int i = 0; i < 4; i++)
	{
		shm->keys[i] = 0;
		for (int j =0; j < 8; j++)
			shm->sevenSegmentData[i][j] = 1;
	}

	for (int i = 0; i < 640; i++)
		for (int j = 0; j < 480; j++)
			shm->VGAData[j][i] = 0x00000000;

	/*	7. Fork off the gui process	*/

	pid = fork();

	if (pid == 0)
	{
		execvp("/home/jrsharp/code/FPGA/de1_board_simulator/ui/build-de1simulator-Desktop-Debug/de1simulator", argv);
	}

	return 0;
}

int shutdown(int dummy)
{

	struct SharedMemoryLayout* shm;
	
	/*	Kill gui thread by writing kill through to mmap'ed file	*/

	shm = (struct SharedMemoryLayout*)map;

	shm->requestUIExit = 1;

	/*	Wait for gui process to terminate	*/

	printf("Waiting for UI to exit...\n");
	while (!shm->uiExited){};

	/*	munmap memory	*/

    if (munmap(map, SHAREDMEMORYSIZE) == -1) {
		perror("Error un-mmapping the file\n");
    }

	/*	delete the shared memory file	*/
	if (unlink(FILEPATH) == -1) 
	{
		perror("Error deleting the memory mapped file\n");
	}

	return 0;
}

int ui_exited(int dummy)
{
	struct SharedMemoryLayout* shm;
	
	shm = (struct SharedMemoryLayout*)map;
	
	return (shm->uiExited);
}

int set_display_segment(int display, int segment, struct ghdl_string* val)
{
	char string[1024];

	struct SharedMemoryLayout* shm;
	
	shm = (struct SharedMemoryLayout*)map;

	strncpy(string, val->base, val->bounds->len);
	string[val->bounds->len] = '\0';

	if (strcmp(string,"'1'") == 0)
		shm->sevenSegmentData[display][segment] = 1;
	else
		shm->sevenSegmentData[display][segment] = 0;

//	printf("%d %d %s\n", display, segment, string);
	
	return 0;
}

int set_led(int red_green, int led, struct ghdl_string *on_off)
{

	int on_off_int;
	struct SharedMemoryLayout* shm;
	char string[1024];
	
	shm = (struct SharedMemoryLayout*)map;
	
	/*	Parse the std_logic type	*/

	strncpy(string, on_off->base, on_off->bounds->len);
	string[on_off->bounds->len] = '\0';

	if (strcmp(string,"'1'") == 0)
		on_off_int = 1;
	else
		on_off_int = 0;

	if (red_green == 0)
	{
		/*		Red			*/

		if (led < 10 && led >= 0)
			shm->redLEDs[led] = on_off_int;			
	} 
	else
	{
		/*		Green		*/

		if (led < 8 && led >= 0)
			shm->greenLEDs[led] = on_off_int;	
	}

}

int get_slide_switch(int sw)
{
	struct SharedMemoryLayout* shm;
	shm = (struct SharedMemoryLayout*)map;
	
	if (sw >= 0 && sw < 10)
		return shm->slideSwitches[sw];
	else
		return 0;
}

int get_key(int key)
{
	struct SharedMemoryLayout* shm;
	shm = (struct SharedMemoryLayout*)map;

	if (key >= 0 && key < 4)
		return shm->keys[key];
	return 0;	
}

int set_video_ram(int x, int y, int val)
{
	struct SharedMemoryLayout* shm;
	shm = (struct SharedMemoryLayout*)map;

	if (x >= 0 && x < 640 && y >= 0 && y < 480)
		shm->VGAData[y][x] = val;	

	//printf("VGA: %d %d %p\n", x, y, val);
}

int set_vretrace(int val)
{
	struct SharedMemoryLayout* shm;
	shm = (struct SharedMemoryLayout*)map;

	shm->verticalRetrace = val;
}

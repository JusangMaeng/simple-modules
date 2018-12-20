#include <stdio.h>

#include <fcntl.h>	/* open */
#include <unistd.h>	/* close */
#include <sys/ioctl.h>  /* ioctl */

int main()
{
	int fd;

	fd = open("/dev/char-dev-sysfs", O_RDWR | O_NDELAY);
	
	if(fd < 0)
	{
		printf("@@## [%s:%d] open failed!\n", __func__, __LINE__);
		close(fd);
		return 0;
	}
	
	printf("@@## [%s:%d] open succ!\n", __func__, __LINE__);
	
	close(fd);
	
	return 0;
}
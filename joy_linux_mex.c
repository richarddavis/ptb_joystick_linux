#include "mex.h"
/* #include "joy_linux_mex.h" */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

int open_joystick()
{
  int fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK); /* non-blocking */
  /* int fd = open("/dev/input/js0", O_RDONLY); /\* blocking *\/ */
  char number_of_axes;
  /* ioctl(fd, JSIOCGAXES, &number_of_axes); */
  /* printf("Axis: %d\n", number_of_axes); */
  return fd;
}

int read_joystick(struct js_event *jse, int fd)
{
  int bytes;
  bytes = read(fd, jse, sizeof(*jse));
  if (bytes == -1)
    return 0;
  
  if (bytes == sizeof(*jse))
    return 1;

  printf("Unexpected bytes from joystick:%d\n", bytes);

  return -1;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  int fd, rc;
  double* out;
  struct js_event jse;

  const char* me = mexFunctionName();

  /* mexPrintf("%s\n", me); */

  if (strcmp (me, "joy_linux_open") == 0)
    {
      fd = open_joystick();
      /* mexPrintf("%d\n", fd); */
      plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
      *(mxGetPr(plhs[0])) = (double) fd;
      return;
    }
  else if(strcmp (me, "joy_linux_read") == 0)
    {
      fd = (unsigned int) mxGetScalar(prhs[0]);
      rc = read_joystick(&jse, fd);
      /* mexPrintf("Event: time %8u, value %08hd, type %02x, axis/button: %d\n", */
      /* 		jse.time, jse.value, jse.type, jse.number); */
      /* usleep(1000); */
      if (rc == 1)
	{
	  plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	  plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
	  plhs[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
	  plhs[3] = mxCreateDoubleMatrix(1, 1, mxREAL);

	  *(mxGetPr(plhs[0])) = (double) rc;
	  *(mxGetPr(plhs[1])) = (double) jse.value;
	  *(mxGetPr(plhs[2])) = (double) jse.type;
	  *(mxGetPr(plhs[3])) = (double) jse.number;
	}
      else if (rc == 0)
	{
	  plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	  plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
	  plhs[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
	  plhs[3] = mxCreateDoubleMatrix(1, 1, mxREAL);

	  *(mxGetPr(plhs[0])) = (double) rc;
	  *(mxGetPr(plhs[1])) = (double) 0;
	  *(mxGetPr(plhs[2])) = (double) 0;
	  *(mxGetPr(plhs[3])) = (double) 0;
	}
      return;
    }
  return;
}

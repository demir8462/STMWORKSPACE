#include "parcala.h"
char PARTICALBUFFER[5];
int x, y, z,q;
int startindex=1;
void processBuffer(char *BUFFERR,int *x,int *y,int *z,int *q)
{
	startindex=1;
    for (uint8_t BUFFERINDEX = 0; BUFFERINDEX < 4; BUFFERINDEX++)
    {
        for (uint8_t i = startindex; i < 20; i++)
        {
            if (BUFFERR[i] == '#')
            {
                startindex = i+1;
                break;
            }
            else
            {
                PARTICALBUFFER[i - startindex] = BUFFERR[i];

            }
        }
        switch (BUFFERINDEX) {
			case 0:
				*x = atoi(PARTICALBUFFER);
				break;
			case 1:
				*y = atoi(PARTICALBUFFER);
				break;
			case 2:
				*z = atoi(PARTICALBUFFER);
				break;
			case 3:
				*q = atoi(PARTICALBUFFER);
				break;
			default:
				break;
		}
    }
}

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <fcntl.h>

using namespace std;


void loop(int pid, const char* fileName)
{
	int input;
	cout << "loop - Process:" << pid << " got file name:" << fileName << endl;

	// one time open file
	int fd = open(fileName, O_RDONLY);
	if (fd < 0)
	{
		cout << "loop - can not open file:" << fileName << " aborting..." << endl;
		return;
	}
	char* buff = new char [80];

	while (1)
	{
		cout << "enter 1 to finish proccess:" << pid << endl;
		cin >> input;
		if (input == 1)
		{
			break;
		}

		
		
		size_t numBytesToRead = 26;
		ssize_t bytes_read;
		bytes_read = read(fd, buff, numBytesToRead);
		cout << "loop - process" << pid << " read:" << buff << endl;

		unsigned int secToSleep = 3;
       		unsigned int left = sleep(secToSleep);
	
	}

	cout << "loop - terminating loop, returning to main..." << endl;
	delete [] buff;
}



int main(int argc, char** argv)
{
	cout << " ---- main - start  ----" << endl;
	if (argc < 2)
	{
		cout << "main - not enough arguments..." << endl;
		return 2;
	}

	char* devFileName = argv[1];
	cout << "main - got device file name:" << devFileName << endl;

	loop(getpid(), devFileName);


	cout << " ---- main - end  ----" << endl;
	return 0;
}

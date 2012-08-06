#ifndef ROXLU_PIPEH
#define ROXLU_PIPEH

#include <poco/Thread.h>
#include <poco/Runnable.h>
#include <poco/Mutex.h>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

struct PipeData {
	char* data;
	int num_bytes;
};

class Pipe : public Poco::Runnable {
public:
	Pipe() 
		:fp(0)
	{
	}
	~Pipe() {
	}
	
	virtual void run() {
	
		
		while(true) {
			if(!fp || fp < 0) {
				printf("Pipe not opened yet.\n");
				openPipe();
				mutex.lock();
					clear();
				mutex.unlock();
				continue;
			}
			//for(int i = 0; i < pipe_data.size(); ++i) {
			{
				//Poco::Mutex::ScopedLock lock(mutex);
				mutex.lock();
				//printf(">>>>>>>>>>> \n");
			//	printf("Write %zu frames to pipe: %d.\n", pipe_data.size(), fp);
				if(!fp) {
					if(!openPipe()) {
						clear();
					}
				}
				
				for(std::vector<PipeData*>::iterator it = pipe_data.begin(); it != pipe_data.end(); ++it) {
				//for(int i = 0; i < pipe_data.size(); ++i) {
					PipeData& pd = **it;
					//PipeData& pd = *pipe_data[i];
					//printf("Handling: %d\n", i);
			//		printf("num bytes: %d\n", pd.num_bytes);
					int result = ::write(fp, pd.data, pd.num_bytes);
			//		printf(", result = %d\n", result);
					//it = pipe_data.erase(it);
				//	++it;
				}
				clear();
		//		printf("<<<<<<<<<<<<<<<<< \n");
				mutex.unlock();
			}
//			sleep(400);
			
		}
	}
	
	void clear() {
		for(std::vector<PipeData*>::iterator it = pipe_data.begin(); it != pipe_data.end(); ++it) {
			PipeData& pd = **it;
			delete[] pd.data;			
		}
		pipe_data.clear();
	}
	
	bool open(const char* pipe) {
		memcpy(pipe_name, pipe, strlen(pipe));
		return true;
	}
	
	bool openPipe() {
		fp = ::open(pipe_name, O_WRONLY);
		printf("Open: %d\n", fp);
		return fp > 0;
	}
	
	bool write(char* data, int numBytes) {
		//Poco::Mutex::ScopedLock lock(mutex);
		mutex.lock();
		//	printf(">>>>>>>>>>>::\n");
			PipeData* pd = new PipeData();
		//	printf("Allocate: %d bytes.\n", numBytes);
			pd->num_bytes = numBytes;
			pd->data = new char[numBytes];
			memcpy(pd->data, data, numBytes);
			pipe_data.push_back(pd);
		//	printf("pipe_data.num_bytes: %d\n", pd->num_bytes);
		//	printf("<<<<<<<<< ::\n");
		mutex.unlock();
		return true;
	}
	
	char pipe_name[1024];
	int fp;
	Poco::Mutex mutex;
	std::vector<PipeData*> pipe_data;
};

#endif
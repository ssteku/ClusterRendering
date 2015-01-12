#ifndef UTILS_FILEMANAGER_HPP_
#define UTILS_FILEMANAGER_HPP_

#include "WorkerObject.hpp"
#include <string>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include "SceneReader.hpp"
#include "Task.hpp"
#include "SceneReader.hpp"
#include <sstream>
#include <fstream>
#include <queue>
#include <boost/optional.hpp>
#include <mutex>
using namespace std;
class FileManager : public WorkerObject
{
private:
	/**
	 * Function receives data from master, render scene part and send results to master 
	 */
	virtual void manageWork()
	{
		while(1)
		{
			bool end = false;
			cout << "Reading directory: "<< filesPath_ <<endl;
			namespace fs = boost::filesystem;
			fs::path someDir(filesPath_);
			fs::directory_iterator end_iter;


			if ( fs::exists(someDir) && fs::is_directory(someDir))
			{
			  cout << "FileManager: File is directory" << endl;
			  for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
			  {
			    cout << "FileManager: Iteratin over filee: "<< dir_iter->path().string() << endl;
			    cout << "FileManager: File created: " << fs::last_write_time(dir_iter->path()) << endl;
			    if (fs::is_regular_file(dir_iter->status()) && fs::last_write_time(dir_iter->path()) > lastFileWriteTime_)
			    // if (fs::is_regular_file(dir_iter->status()) )
			    {
			      cout <<"FileManager: Adding new task : " << dir_iter->path().string() << endl;
			      lastFileWriteTime_ = fs::last_write_time(dir_iter->path());
			      cout << "FileManager: New last edit time: " << lastFileWriteTime_ << endl;
			      readFile(dir_iter->path().string());	
			      // end = true;		      
			    }
			  }
			}
			if(end)
				break;

			std::chrono::milliseconds dura( 3000 );
			std::this_thread::sleep_for( dura );
		}

	}
public:
	FileManager(std::string filesPath) : 
		filesPath_(filesPath),
		lastFileWriteTime_(std::time(NULL) - (365 * 24 * 3600))
	{
	}

	boost::optional<Task> getTask()
	{
		cout << "FileManager: getTaskr()" << endl;
        std::unique_lock<std::mutex> lock(io_mutex);
        boost::optional<Task> task;
        if(tasks.size()>0)
        {
                task = tasks.front();
                tasks.pop();
			cout << "FileManager: return task" << endl;
		}

        return task;
	} 
	
private:
	void readFile(std::string filePath)
	{
		std::stringstream contextStream;
		contextStream << std::ifstream( filePath.c_str() ).rdbuf();

		SceneReader sr(contextStream);
        Context tmpContext;
        Task task;
        sr.initContext(tmpContext);
        for(int j=0;j<sr.getFrames();j++){
                sr.getFrameContext(tmpContext, j);
                tmpContext.maxRayBounce = 4;
                task.contexts.push_back(tmpContext);
        }        
        task.id = static_cast<unsigned int>(std::time(NULL));                                                
        
                                             
        task.totalFrames = sr.getFrames();

        std::unique_lock<std::mutex> lock(io_mutex);
        cout << "FileManager: New task added to queue" << endl;
        tasks.push(task);
	}


	std::string filesPath_;
	time_t lastFileWriteTime_;
	std::queue<Task> tasks;
    std::mutex io_mutex;
};

#endif  //UTILS_FILEMANAGER_HPP_

#include <mutex>
#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include <functional>

class ThreadPool
{
public:
  static ThreadPool *getInstance()
  {
     if(_instance == nullptr)
     {
       _instance = new ThreadPool();
     }
     return _instance;
  }

  std::vector<std::future<void>> _threadPool;
private:
  static ThreadPool *_instance;

};
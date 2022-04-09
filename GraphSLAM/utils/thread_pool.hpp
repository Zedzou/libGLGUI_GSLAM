#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <atomic>
#include <string>
namespace tools
{
    class TaskThreadPool
    {
        private:
            // 任务元素
            struct task_element_t 
            {
                bool run_with_id;
                const std::function< void() > no_id;
                const std::function< void(std::size_t) > with_id;
                
                explicit task_element_t(const std::function< void() >& f) :
                run_with_id(false), no_id(f), with_id(nullptr) { }
                
                explicit task_element_t(const std::function< void(std::size_t) >& f) :
                run_with_id(true), no_id(nullptr), with_id(f) { }
            };

            std::mutex mutex_; // 互斥锁
            std::queue<task_element_t> tasks_; // 任务队列
            std::vector<std::thread> threads_;
            std::condition_variable condition_;
            std::condition_variable completed_;
            bool running_;
            std::atomic_bool complete_;
            std::size_t available_;
            std::size_t total_;
            bool pause_;

        public:
            explicit TaskThreadPool(std::size_t pool_size):threads_(pool_size),
                                                           running_(true), 
                                                           complete_(true), 
                                                           available_(pool_size), 
                                                           total_(pool_size), 
                                                           pause_(false) 
            {
                for ( std::size_t i = 0; i < pool_size; ++i ) {
                    threads_[i] = std::thread( std::bind(&TaskThreadPool::main_loop, this, i) ); // std::bind把成员函数抠出来当静态函数用
                }
            }
            
            // 设置线程池尺寸
            void setPoolSize(size_t pool_size)
            {
                threads_.clear();
                threads_.resize(pool_size);
                available_ = pool_size;
                total_ = pool_size;
                running_ = true;
                complete_ = true;
                pause_ = false;
                for ( std::size_t i = 0; i < pool_size; ++i )
                {
                    threads_[i] = std::thread(std::bind(&TaskThreadPool::main_loop, this, i));
                }
            }

            size_t getPoolSize(){return threads_.size();}
            bool finished(){return complete_;} 
            
            // 析构函数
            ~TaskThreadPool() 
            {
                // Set running flag to false then notify all threads.
                {
                    std::unique_lock< std::mutex > lock(mutex_);
                    running_ = false;
                    condition_.notify_all();
                }
                
                try 
                {
                    for (auto& t : threads_) 
                    {
                        t.join();
                    }
                }

                // Suppress all exceptions.
                catch (const std::exception&) {}
            }
            
            // 向线程池添加任务
            template <typename Task>
            void runTask(Task task) 
            {
                std::unique_lock<std::mutex> lock(mutex_);
                tasks_.push( task_element_t(static_cast<std::function< void() >>(task)) );
                complete_ = false;
                condition_.notify_one();
            }
        
            // 向线程池添加任务
            template <typename Task>
            void runTaskWithID(Task task) 
            {
                std::unique_lock<std::mutex> lock(mutex_);
                tasks_.push( task_element_t( static_cast<std::function< void(std::size_t) >>(task) ) );
                complete_ = false;
                condition_.notify_one();
            }
            
            // 等待所有任务完成
            void waitWorkComplete() 
            {
                std::unique_lock<std::mutex> lock(mutex_);
                while (!complete_)
                    completed_.wait(lock);
            }
        
            // 等待任务完成
            void waitWorkCompleteandShow(size_t total_num, long long display_time = 1)
            {
                float time_past = display_time;
                static std::string pbstr = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
                static int pbwidh = 60;
                while(checkTaskLeft())
                {
                    std::this_thread::sleep_for(std::chrono::seconds(display_time));
                    int current = checkTaskLeft();
                    int proceed = total_num-current;
                    float sec_left = time_past/proceed*current;
                    time_past += display_time;
                    //                printf("%5.2f%%[%d/%zu]ET: %4.0f(s) \n", float(proceed)/total_num*100, proceed, total_num, sec_left);
                    
                    float percentage = float(proceed)/total_num;
                    int val = (int) (percentage * 100);
                    int lpad = (int) (percentage * pbwidh);
                    int rpad = pbwidh - lpad;
                    printf ("\r[%.*s%*s] %3d%% ET: %4.0f(s)", lpad, pbstr.c_str(), rpad, "", val, sec_left);
                    fflush (stdout);
                }
                printf("\n");
            }
        
            size_t checkTaskLeft(){return tasks_.size();}
            
            // pause函数将等待剩余的作业完成并暂停，直到该函数以false值再次被调用。
            void pause(bool option)
            {
                pause_ = option;
                if(pause_)
                {
                    while(available_ != total_){// wait...;}
                }
            }
            
        private:

            // 线程池入口
            void main_loop(std::size_t index) 
            {
                while (running_) 
                {
                    while (pause_){}
                    
                    // Wait on condition variable while the task is empty and the pool is still running.
                    std::unique_lock<std::mutex> lock(mutex_);
                    while ( tasks_.empty() && running_ ) 
                    {
                        condition_.wait(lock);
                    }
                    
                    // 线程池未运行，跳出循环
                    if (!running_) break;
                
                    {
                        // 复制任务并从队列中清除
                        auto tasks = tasks_.front();
                        tasks_.pop();

                        // 递减计数，指示线程不再可用。
                        --available_;
                        
                        // 运行任务
                        lock.unlock();
                        try 
                        {
                            if (tasks.run_with_id) 
                            {
                                tasks.with_id(index);
                            } 
                            else 
                            {
                                tasks.no_id();
                            }
                        }

                        // 抑制所有异常
                        catch ( const std::exception& ) {}
                        
                        // 更新状态为空，可能需要先恢复锁
                        lock.lock();
                        
                        // Increment count, indicating thread is available.
                        ++available_;
                        
                        if (tasks_.empty() && available_ == total_) 
                        {
                            complete_ = true;
                            completed_.notify_one();
                        }

                    }
                }  // while running_
            }

    };


} // end of namespace tools
#endif


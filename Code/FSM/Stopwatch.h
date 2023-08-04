//#pragma once
//#include <chrono> 
//
//template<typename type = float, typename period = std::milli>
//class Stopwatch
//{
//public:
//    using clock = std::chrono::high_resolution_clock;
//    using duration = std::chrono::duration<type, period>;
//    using time_point = std::chrono::time_point<clock, duration>;
//
//    Stopwatch() : time_(clock::now()) { }
//    Stopwatch(const stopwatch& that) = default;
//    Stopwatch(stopwatch&& temp) = default;
//    ~Stopwatch() = default;
//    Stopwatch& operator=(const stopwatch& that) = default;
//    Stopwatch& operator=(stopwatch&& temp) = default;
//
//    duration tick()
//    {
//        time_point time = clock::now();
//        duration   delta = time - time_;
//        time_ = time;
//        return delta;
//    }
//
//private:
//    time_point time_;
//};
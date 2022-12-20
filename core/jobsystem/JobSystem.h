//
// Created by yjrj on 2022/12/20.
//

#pragma once

#include <core/singleton/Singleton.h>
#include <taskflow/taskflow.hpp>

namespace cc::exp {

class JobSystem : public Singleton<JobSystem> {
public:
    void RunAndWait(tf::Taskflow &flow)
    {
        executor.run(flow).wait();
    }

    auto Run(tf::Taskflow &flow)
    {
        return executor.run(flow);
    }

private:
    friend class Singleton<JobSystem>;
    JobSystem()  = default;
    ~JobSystem() = default;

    tf::Executor executor;
};

}

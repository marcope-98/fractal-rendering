#ifndef FRR_UTILS_BARRIER_HPP_
#define FRR_UTILS_BARRIER_HPP_

#include <condition_variable>
#include <cstdint>
#include <mutex>

namespace frr
{
  class Barrier
  {
  public:
    explicit Barrier(std::size_t count) : threshold{count}, count{count}, generation{0} {}

    void wait()
    {
      std::unique_lock<std::mutex> lock(this->mtx);
      std::size_t                  gen = this->generation;
      if (--this->count == 0)
      {
        this->generation++;
        this->count = this->threshold;
        this->cv.notify_all();
      }
      else
      {
        this->cv.wait(lock, [this, gen]
                      { return gen != this->generation; });
      }
    }

  private:
    std::mutex              mtx;
    std::condition_variable cv;
    std::size_t             threshold{}, count{}, generation{};
  };
} // namespace frr

#endif
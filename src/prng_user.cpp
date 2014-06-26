#include "prng_user.hpp"

PrngUser::PrngUser(std::shared_ptr<std::mt19937> generator)
: generator_{generator}
{
}

std::shared_ptr<std::mt19937> PrngUser::getGenerator() const
{
  return generator_;
}

void PrngUser::setGenerator(std::shared_ptr<std::mt19937> generator)
{
  generator_ = generator;
}

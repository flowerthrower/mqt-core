#pragma once

#include "na/NADefinitions.hpp"
#include "na/operations/NAOperation.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace na {
class NAComputation {
protected:
  std::vector<std::shared_ptr<Point>> initialPositions;
  std::vector<std::unique_ptr<NAOperation>> operations;

public:
  NAComputation() = default;
  NAComputation(NAComputation&& qc) noexcept = default;
  NAComputation& operator=(NAComputation&& qc) noexcept = default;
  NAComputation(const NAComputation& qc)
      : initialPositions(qc.initialPositions) {
    operations.reserve(qc.operations.size());
    std::transform(qc.operations.cbegin(), qc.operations.cend(),
                   std::back_inserter(operations),
                   [](const auto& op) { return op->clone(); });
  }
  NAComputation& operator=(const NAComputation& qc) {
    if (this != &qc) {
      initialPositions = qc.initialPositions;
      operations.clear();
      operations.reserve(qc.operations.size());
      std::transform(qc.operations.cbegin(), qc.operations.cend(),
                     std::back_inserter(operations),
                     [](const auto& op) { return op->clone(); });
    }
    return *this;
  }
  virtual ~NAComputation() = default;
  template <class T> auto emplaceBack(std::unique_ptr<T>&& op) -> void {
    static_assert(std::is_base_of_v<NAOperation, T>,
                  "T must be a subclass of NAOperation.");
    operations.emplace_back(std::move(op));
  }
  template <class T> auto emplaceBack(const std::unique_ptr<T>& op) -> void {
    static_assert(std::is_base_of_v<NAOperation, T>,
                  "T must be a subclass of NAOperation.");
    operations.emplace_back(std::move(op));
  }
  template <class T, class... Args> auto emplaceBack(Args&&... args) -> void {
    static_assert(std::is_base_of_v<NAOperation, T>,
                  "T must be a subclass of NAOperation.");
    operations.emplace_back(std::make_unique<T>(args...));
  }
  auto clear() -> void {
    operations.clear();
    initialPositions.clear();
  }
  [[nodiscard]] auto size() const -> std::size_t { return operations.size(); }
  [[nodiscard]] auto
  getInitialPositions() const -> const std::vector<std::shared_ptr<Point>>& {
    return initialPositions;
  }
  auto emplaceInitialPosition(std::shared_ptr<Point> p) -> void {
    initialPositions.emplace_back(std::move(p));
  }
  [[nodiscard]] auto toString() const -> std::string;
  friend auto operator<<(std::ostream& os,
                         const NAComputation& qc) -> std::ostream& {
    return os << qc.toString();
  }
  // Iterators (pass-through)
  auto begin() noexcept { return operations.begin(); }
  [[nodiscard]] auto begin() const noexcept { return operations.begin(); }
  [[nodiscard]] auto cbegin() const noexcept { return operations.cbegin(); }
  auto end() noexcept { return operations.end(); }
  [[nodiscard]] auto end() const noexcept { return operations.end(); }
  [[nodiscard]] auto cend() const noexcept { return operations.cend(); }
  auto rbegin() noexcept { return operations.rbegin(); }
  [[nodiscard]] auto rbegin() const noexcept { return operations.rbegin(); }
  [[nodiscard]] auto crbegin() const noexcept { return operations.crbegin(); }
  auto rend() noexcept { return operations.rend(); }
  [[nodiscard]] auto rend() const noexcept { return operations.rend(); }
  [[nodiscard]] auto crend() const noexcept { return operations.crend(); }
};
} // namespace na

#pragma once

// Copyright © Conor Williams <conorwilliams@outlook.com>

// SPDX-License-Identifier: MPL-2.0

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <semaphore>

#include "libfork/utility.hpp"

/**
 * @file wait.hpp
 *
 * @brief Implementation of the ``waiter`` mixin class.
 */

namespace lf {

template <bool Waitable>
class waiter;

/**
 * @brief A promise mixin that provides the wait method for root tasks.
 */
template <>
class waiter<true> {
 public:
  /**
   * @brief Check if the task has completed, non-blocking.
   */
  [[nodiscard]] auto is_ready() noexcept -> bool { return m_sem.try_acquire(); }

  /**
   * @brief Block until the task has completed.
   */
  auto wait() noexcept -> void { return m_sem.acquire(); }

 protected:
  /**
   * @brief Signal that the task has completed.
   */
  auto make_ready() noexcept -> void {
    DEBUG_TRACKER("root task signals completion");
    m_sem.release();
  }

 private:
  std::binary_semaphore m_sem{0};
};

/**
 * @brief An empty class for non-waitable/non-root tasks.
 */
template <>
class waiter<false> {};

}  // namespace lf
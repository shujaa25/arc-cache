# Adaptive Replacement Cache (ARC) Implementation in C++

This repository contains a C++ implementation of the **Adaptive Replacement Cache (ARC)** algorithm, based on the research paper from **FAST 2003**. ARC dynamically balances **recency** and **frequency** in cache management, making it highly effective under mixed workloads.

---

## Features

- Implements ARC with **T1, T2, B1, B2** lists.
- Tracks cache hits and misses.
- Automatically adjusts the target size `p` for balancing recency and frequency.
- Demonstrates adaptive behavior under various access patterns.
- Simple `access(key)` interface for integration with different workloads.

---

## How It Works

ARC maintains four lists:

1. **T1**: Recently accessed entries (recency).
2. **T2**: Frequently accessed entries (frequency).
3. **B1**: Ghost list for recently evicted T1 entries.
4. **B2**: Ghost list for recently evicted T2 entries.

The algorithm uses a dynamic parameter `p` to determine how much cache space is allocated to **recency (T1)** versus **frequency (T2)**. When a key is accessed:

- If it is in **T1**, it is promoted to **T2** (frequency list).
- If it is in **T2**, it is moved to the front of **T2** (maintain recency among frequent entries).
- If it is in **B1** or **B2** (ghost lists), `p` is adjusted and the key is brought back into cache.
- If it is a **cache miss**, the least recently/frequently used item is evicted according to ARC’s replacement rules.

---

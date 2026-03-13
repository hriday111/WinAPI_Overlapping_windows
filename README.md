# Lab 2: WinAPI Elastic Window Chain (Study Guide)

## 1. Project Overview
This application creates a "Chain" of windows. One window is the **Leader**, and the others are **Followers**. When you move the Leader, the Followers glide toward it with a smooth, elastic effect. Each follower is progressively smaller than the one before it.

---

## 2. The Application Flow (Lifecycle)
If the teacher asks "How does the app start and run?", here is the step-by-step:

1.  **Entry Point (`main.cpp`)**: The program starts in `wWinMain`. It creates an `App` object and calls `Initialize()`.
2.  **Initialization (`App.cpp`)**:
    *   **Register Class**: It tells Windows about a new type of window called "ChainWnd" and links it to our `WndProc`.
    *   **Create Windows**: It loops to create the Leader and Followers. 
    *   **Ownership (Z-Order)**: It sets each window's "Owner". In WinAPI, an **Owned** window stays above its **Owner**. We link them so the Leader is at the bottom.
3.  **Message Loop (`App.cpp`)**: The `Run()` method starts a loop that waits for Windows messages (like clicks, moves, or timers) and sends them to the correct window.
4.  **Movement Logic**:
    *   **Step A**: You move the Leader. Windows sends a `WM_MOVE` message.
    *   **Step B**: The Leader notifies the `App` class, which updates the "Target Position" for all Followers.
    *   **Step C**: A Timer (`WM_TIMER`) triggers 60 times a second. Each Follower calculates the distance to its target and moves a small percentage (Alpha) toward it. This is called **Linear Interpolation (Lerp)**.

---

## 3. Key Files & Roles
*   **`Constants.h`**: The "Control Room." All numbers that define behavior are here.
*   **`App.h / .cpp`**: The "Manager." It coordinates between windows and handles the high-level Windows setup.
*   **`Window.h / .cpp`**: The "Individual." Each window object stores its own position, size, and handle (`HWND`).
*   **`main.cpp`**: The "Ignition." Just starts the engine.

---

## 4. The "Lab Cheat Sheet" (Variables to Change)

If your teacher asks you to change the behavior on the fly, look here:

### In `Constants.h`:
*   `CHAIN_LEN`: Change this to add or remove windows (e.g., set to `10` for a long snake).
*   `TICK_MS`: Change the smoothness. `16` is roughly 60 FPS. Lower is faster/smoother.
*   `ALPHA_BASE`: The "Elasticity." Higher = stiffer/faster following. Lower = lazier/more lag.
*   `OFFSET_Y`: The vertical gap between windows in the chain.

### In `App.cpp` (Inside `Initialize` and `UpdateFollowers`):
*   `shrinkAmount`: Change this (currently `30`) to make the followers get smaller faster or slower. Set to `0` to make them all the same size.

---

## 5. Core WinAPI Concepts (For Theory Questions)

### Q: What is `WndProc`?
**A:** It is the "Brain" of the window. Every time something happens (mouse move, paint, close), Windows calls this function. We use `SetWindowLongPtr` to attach our C++ `Window` object to the WinAPI `HWND` so the `WndProc` knows which object to talk to.

### Q: How does the "Glide" work?
**A:** We use **Lerp (Linear Interpolation)**. 
Formula: `CurrentPosition += (TargetPosition - CurrentPosition) * Alpha`.
It moves a percentage of the remaining distance every frame, which naturally slows down as it gets closer, creating that smooth "elastic" feel.

### Q: Why is the Leader at the bottom?
**A:** Because we set the **Ownership chain**. In WinAPI, an "Owned" window is always drawn on top of its "Owner."
*   Follower 1 is the Owner of the Leader.
*   Follower 2 is the Owner of Follower 1.
This forces the Leader to the absolute bottom of the stack.

### Q: What is `WM_NCCREATE`?
**A:** It's one of the very first messages a window gets. We use it to "hand over" the `this` pointer from our C++ class to the WinAPI window handle so they are linked from the very start.

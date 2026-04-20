Delivery Route Planner using Dijkstra’s Algorithm

A complete DSAA project that implements Dijkstra’s Algorithm in C++ and visualizes the shortest path using D3.js.

📌 Overview

This project solves the shortest path problem in a weighted graph, simulating a real-world delivery routing system.

It combines:

⚙️ Backend (C++ algorithm)
📊 Data (JSON output)
🌐 Frontend (D3.js visualization)
🎯 Problem Statement

In delivery systems (Amazon, Swiggy, etc.), finding the shortest route between locations is essential to minimize cost and time.

This project uses Dijkstra’s Algorithm to efficiently compute the shortest path and visualize the process.

🧠 Algorithm Used

Dijkstra’s Algorithm (Greedy Approach)

Key Idea:

At each step, pick the node with the minimum distance and update its neighbors.

⚙️ Tech Stack
Backend
C++
STL (vector, priority_queue, chrono, fstream)
Frontend
HTML, CSS, JavaScript
D3.js (for visualization)
Data Format
JSON (output.json)
🔄 Project Flow
User Input (C++)
        ↓
Dijkstra Algorithm
        ↓
JSON Output (output.json)
        ↓
Frontend (D3.js)
        ↓
Visualization
📥 Input Format
N E
u v w
u v w
...
src dest
Example:
7 9
0 1 4
0 2 2
1 3 5
2 3 8
2 4 3
3 6 6
4 5 7
5 6 1
1 4 2
0 6
📤 Output
Console Output
Shortest Path
Total Cost
Execution Time
JSON Output (output.json)
{
  "nodes": [...],
  "edges": [...],
  "path": [...],
  "dist": [...],
  "steps": [...],
  "src": 0,
  "dest": 6,
  "cost": 13,
  "timeMs": 0.003
}
✨ Features
✅ Shortest path calculation
✅ Step-by-step traversal tracking
✅ JSON-based output
✅ Interactive visualization
✅ Execution time measurement
✅ Error handling (invalid input / no path)
🚀 How to Run
1️⃣ Compile (C++)
g++ -std=c++11 route.cpp -o route
2️⃣ Run (Windows)
route.exe
3️⃣ Start Server
python -m http.server
4️⃣ Open in Browser
http://localhost:8000
5️⃣ Use UI
Click Load Graph
Click Start
📊 Complexity
Time Complexity: O((V + E) log V)
Space Complexity: O(V + E)
⚠️ Limitations
❌ No support for negative weights
❌ Manual input (no GUI input yet)
❌ Random graph layout in visualization
🔥 Future Improvements
GUI-based graph input
Drag-and-drop graph editor
Compare with Bellman-Ford
Real map integration (Google Maps)
Live updates in visualization
🟣 Conclusion

This project demonstrates a real-world application of Dijkstra’s Algorithm, combining:

Efficient computation (C++)
Structured data (JSON)
Interactive visualization (D3.js)
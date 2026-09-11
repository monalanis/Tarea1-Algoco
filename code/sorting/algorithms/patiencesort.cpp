/*
 * Referencia: Esta implementación en especifico tiene optimización en O(n log n),
 * Este utiliza búsqueda binaria para las pilas y colas de prioridad, para que así
 * la únion de los resultados sea más eficiente.
 * URL: https://www.geeksforgeeks.org/dsa/patience-sorting/
 */

#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
 
using namespace std;
 
vector<int> patienceSort(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n <= 1) return arr;

   vector<vector<int>> piles;
    vector<int> tops;
 
    for (int i = 0; i < n; ++i) {
        int value = arr[i];

        auto it = lower_bound(tops.begin(), tops.end(), value);
 
        if (it == tops.end()) {
            piles.push_back({value});
            tops.push_back(value);
        } else {
           
            size_t idx = static_cast<size_t>(it - tops.begin());
            piles[idx].push_back(value);
            tops[idx] = value; 
        }
    }

    using HeapItem = pair<int, pair<int, int>>;
    auto cmp = [](const HeapItem& a, const HeapItem& b) {
        return a.first > b.first; // min-heap
    };
    priority_queue<HeapItem, vector<HeapItem>, decltype(cmp)> heap(cmp);
 
    int k = static_cast<int>(piles.size());
    for (int p = 0; p < k; ++p) {
        int topIdx = static_cast<int>(piles[p].size()) - 1;
        heap.push({piles[p][topIdx], {p, topIdx}});
    }
 
    int outIdx = 0;
    while (!heap.empty()) {
        auto [value, loc] = heap.top();
        heap.pop();
        int p = loc.first;
        int j = loc.second; 
        arr[outIdx++] = value;
 
        if (j - 1 >= 0) {
            heap.push({piles[p][j - 1], {p, j - 1}});
        }
    }
 
    return arr;
}
 

#ifdef PATIENCESORT_STANDALONE_MAIN
#include <iostream>
int main() {
    vector<int> arr = {6, 3, 8, 1, 9, 2, 7, 4, 5, 0};
 
    vector<int> sorted = patienceSort(arr);
 
    for (int v : sorted) cout << v << " ";
    cout << endl;
 
    return 0;
}
#endif
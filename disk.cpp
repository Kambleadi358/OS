#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

// Print sequence
void printSequence(int seq[], int n) {
    for(int i=0;i<n;i++)
        cout << seq[i] << " ";
}

// ---------------- FCFS ----------------
void fcfs(int req[], int n, int head) {
    int seek = 0;
    int curr = head;

    cout << "\n--- FCFS ---\nOrder: " << curr << " ";

    for(int i=0;i<n;i++) {
        seek += abs(req[i] - curr);
        curr = req[i];
        cout << curr << " ";
    }

    cout << "\nTotal Seek Movement = " << seek << endl;
}

// ---------------- SSTF ----------------
void sstf(int req[], int n, int head) {
    int visited[50] = {0};
    int seek = 0, curr = head;

    cout << "\n--- SSTF ---\nOrder: " << curr << " ";

    for(int i=0;i<n;i++) {
        int minDist = 1e9, idx = -1;

        for(int j=0;j<n;j++) {
            if(!visited[j]) {
                int dist = abs(req[j] - curr);
                if(dist < minDist) {
                    minDist = dist;
                    idx = j;
                }
            }
        }

        visited[idx] = 1;
        seek += abs(req[idx] - curr);
        curr = req[idx];

        cout << curr << " ";
    }

    cout << "\nTotal Seek Movement = " << seek << endl;
}

// ---------------- SCAN ----------------
void scan(int req[], int n, int head, int diskSize) {
    int seek = 0;
    int curr = head;

    sort(req, req+n);

    cout << "\n--- SCAN ---\nOrder: " << curr << " ";

    int i;
    for(i=0;i<n;i++)
        if(req[i] > head) break;

    // right side
    for(int j=i;j<n;j++) {
        seek += abs(req[j] - curr);
        curr = req[j];
        cout << curr << " ";
    }

    // go to end
    seek += abs((diskSize-1) - curr);
    curr = diskSize-1;
    cout << curr << " ";

    // left side
    for(int j=i-1;j>=0;j--) {
        seek += abs(req[j] - curr);
        curr = req[j];
        cout << curr << " ";
    }

    cout << "\nTotal Seek Movement = " << seek << endl;
}

// ---------------- C-SCAN ----------------
void cscan(int req[], int n, int head, int diskSize) {
    int seek = 0;
    int curr = head;

    sort(req, req+n);

    cout << "\n--- C-SCAN ---\nOrder: " << curr << " ";

    int i;
    for(i=0;i<n;i++)
        if(req[i] > head) break;

    // right
    for(int j=i;j<n;j++) {
        seek += abs(req[j] - curr);
        curr = req[j];
        cout << curr << " ";
    }

    // go to end
    seek += abs((diskSize-1) - curr);
    curr = diskSize-1;
    cout << curr << " ";

    // jump to 0
    seek += (diskSize-1);
    curr = 0;
    cout << curr << " ";

    // remaining
    for(int j=0;j<i;j++) {
        seek += abs(req[j] - curr);
        curr = req[j];
        cout << curr << " ";
    }

    cout << "\nTotal Seek Movement = " << seek << endl;
}

// ---------------- C-LOOK ----------------
void clook(int req[], int n, int head) {
    int seek = 0;
    int curr = head;

    sort(req, req+n);

    cout << "\n--- C-LOOK ---\nOrder: " << curr << " ";

    int i;
    for(i=0;i<n;i++)
        if(req[i] > head) break;

    // right
    for(int j=i;j<n;j++) {
        seek += abs(req[j] - curr);
        curr = req[j];
        cout << curr << " ";
    }

    // jump to first request
    if(i > 0) {
        seek += abs(curr - req[0]);
        curr = req[0];
        cout << curr << " ";
    }

    // remaining
    for(int j=1;j<i;j++) {
        seek += abs(req[j] - curr);
        curr = req[j];
        cout << curr << " ";
    }

    cout << "\nTotal Seek Movement = " << seek << endl;
}

// ---------------- MAIN ----------------
int main() {
    int n, head, diskSize;

    cout << "Enter number of requests: ";
    cin >> n;

    int req[50];
    cout << "Enter request sequence:\n";
    for(int i=0;i<n;i++)
        cin >> req[i];

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> diskSize;

    int choice;

    do {
        cout << "\n===== DISK SCHEDULING MENU =====\n";
        cout << "1. FCFS\n2. SSTF\n3. SCAN\n4. C-SCAN\n5. C-LOOK\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: fcfs(req, n, head); break;
            case 2: sstf(req, n, head); break;
            case 3: scan(req, n, head, diskSize); break;
            case 4: cscan(req, n, head, diskSize); break;
            case 5: clook(req, n, head); break;
        }

    } while(choice != 6);

    return 0;
}
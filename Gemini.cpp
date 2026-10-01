 #include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct Email {
    std::string category;
    std::string subject;
    std::string date; // MM-DD-YYYY

    // Helper to get priority level based on category (higher number = higher priority)
    int getCategoryPriority() const {
        if (category == "Boss") return 5;
        if (category == "Subordinate") return 4;
        if (category == "Peer") return 3;
        if (category == "ImportantPerson") return 2;
        if (category == "OtherPerson") return 1;
        return 0;
    }

    // Converts MM-DD-YYYY to YYYYMMDD integer for fast chronological comparison
    int getDateKey() const {
        if (date.length() < 10) return 0;
        int month = std::stoi(date.substr(0, 2));
        int day   = std::stoi(date.substr(3, 2));
        int year  = std::stoi(date.substr(6, 4));
        return year * 10000 + month * 100 + day;
    }

    // Overload strict total ordering: greater operator for MaxHeap
    bool operator>(const Email& other) const {
        int prio1 = getCategoryPriority();
        int prio2 = other.getCategoryPriority();

        if (prio1 != prio2) {
            return prio1 > prio2;
        }

        // Newer emails come first if categories are equal
        return getDateKey() > other.getDateKey();
    }
};

// List-based MaxHeap implementation built from scratch
class MaxHeap {
private:
    std::vector<Email> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = static_cast<int>(heap.size());
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap[left] > heap[largest]) {
                largest = left;
            }
            if (right < size && heap[right] > heap[largest]) {
                largest = right;
            }

            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    bool empty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }

    void push(const Email& email) {
        heap.push_back(email);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    const Email& peekMax() const {
        return heap[0];
    }

    void popMax() {
        if (empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!empty()) {
            heapifyDown(0);
        }
    }
};

int main() {
    MaxHeap priorityQueue;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        if (line.rfind("EMAIL ", 0) == 0) {
            std::string payload = line.substr(6);
            std::stringstream ss(payload);

            Email email;
            std::getline(ss, email.category, ',');
            std::getline(ss, email.subject, ',');
            std::getline(ss, email.date);

            // Trim leading/trailing spaces if any exist in parsing
            priorityQueue.push(email);

        } else if (line == "COUNT") {
            std::cout << "There are " << priorityQueue.size() << " emails to read.\n";

        } else if (line == "NEXT") {
            if (!priorityQueue.empty()) {
                const Email& nextEmail = priorityQueue.peekMax();
                std::cout << "Next email:\n";
                std::cout << "Sender: " << nextEmail.category << "\n";
                std::cout << "Subject: " << nextEmail.subject << "\n";
                std::cout << "Date: " << nextEmail.date << "\n";
            }
            
        } else if (line == "READ") {
            if (!priorityQueue.empty()) {
                priorityQueue.popMax();
            }
        }
    }

    return 0;
}
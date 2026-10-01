/*  EECS 348 Assignment 2
 *  Idk yet
 * 
 *  Collaborators: Luke Tidball, ChatGPT
 */ 

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// ------------------------------------------------------------
// Email class
// ------------------------------------------------------------
class Email {
private:
    string senderCategory;
    string subject;
    string date;
    int dateKey;

    int priority() const {
        if (senderCategory == "Boss")
            return 5;
        else if (senderCategory == "Subordinate")
            return 4;
        else if (senderCategory == "Peer")
            return 3;
        else if (senderCategory == "ImportantPerson")
            return 2;
        else
            return 1; // OtherPerson or unrecognized category
    }


    // Converts MM-DD-YYYY into a number that can be compared.
    int convertDateToKey(const string& date) const {
        int month, day, year;
        char dash1, dash2;

        stringstream ss(date);

        if (ss >> month >> dash1 >> day >> dash2 >> year) {
            return year * 10000 + month * 100 + day;
        }

        return 0;
    }


public:
    Email()
        : senderCategory(""), subject(""), date(""), dateKey(0) {}

    Email(string senderCategory, string subject, string date)
        : senderCategory(senderCategory),
        subject(subject),
        date(date),
        dateKey(convertDateToKey(date)) {}

    string getSender() const {
        return senderCategory;
    }

    string getSubject() const {
        return subject;
    }

    string getDate() const {
        return date;
    }

    // Returns true if this email has higher priority than other.
    bool higherPriorityThan(const Email& other) const {
        if (priority() != other.priority()) {
            return priority() > other.priority();
        }

        // Same sender category: newest email gets priority.
        return dateKey > other.dateKey;
    }

};

// ------------------------------------------------------------
// List-based MaxHeap
// ------------------------------------------------------------
class MaxHeap {
private:
    vector<Email> heap;

    int parent(int index) {
        return (index - 1) / 2;
    }

    int leftChild(int index) {
        return 2 * index + 1;
    }

    int rightChild(int index) {
        return 2 * index + 2;
    }

    void swapEmails(int first, int second) {
        Email temp = heap[first];
        heap[first] = heap[second];
        heap[second] = temp;
    }

    // Moves an element upward until the MaxHeap property is restored.
    void heapifyUp(int index) {
        while (index > 0) {
            int p = parent(index);

            if (heap[index].higherPriorityThan(heap[p])) {
                swapEmails(index, p);
                index = p;
            }
            else {
                break;
            }
        }
    }

    // Moves an element downward until the MaxHeap property is restored.
    void heapifyDown(int index) {
        while (true) {
            int left = leftChild(index);
            int right = rightChild(index);
            int largest = index;

            if (left < heap.size() &&
                heap[left].higherPriorityThan(heap[largest])) {
                largest = left;
            }

            if (right < heap.size() &&
                heap[right].higherPriorityThan(heap[largest])) {
                largest = right;
            }

            if (largest != index) {
                swapEmails(index, largest);
                index = largest;
            }
            else {
                break;
            }
        }
    }

public:
    // Add an email to the MaxHeap.
    void insert(const Email& email) {
        heap.push_back(email);
        heapifyUp(heap.size() - 1);
    }

    const Email& getMax() const {
        return heap[0];
    }

    // Remove the highest-priority email.
    void removeMax() {
        if (heap.empty()) {
            return;
        }

        if (heap.size() == 1) {
            heap.pop_back();
            return;
        }

        heap[0] = heap.back();
        heap.pop_back();

        heapifyDown(0);
    }

    bool isEmpty() const {
        return heap.empty();
    }

    int size() const {
        return heap.size();
    }
};

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------
int main() {
    MaxHeap emailQueue;

    string line;

    while (getline(cin, line)) {
        if (line.empty()) {
            continue;
        }
        // --------------------------------------------------------
        // EMAIL command
        // --------------------------------------------------------
        if (line.substr(0, 5) == "EMAIL") {
            string data = line.substr(6); // Skip "EMAIL "

            // Find the commas separating the three fields.
            size_t comma1 = data.find(',');
            size_t comma2 = data.find(',', comma1 + 1);

            string senderCategory = data.substr(0, comma1);
            string subject = data.substr(
                comma1 + 1,
                comma2 - comma1 - 1
            );
            string date = data.substr(comma2 + 1);

            Email email(senderCategory, subject, date);
            emailQueue.insert(email);
        }

        // --------------------------------------------------------
        // NEXT command
        // --------------------------------------------------------
        else if (line == "NEXT") {

            if (emailQueue.isEmpty()) {
                // There is no email to display.
                continue;
            }

            // Get the highest-priority email without removing it.
            const Email& currentEmail = emailQueue.getMax();

            cout << "\nNext email:\n";
            cout << "\tSender: " << currentEmail.getSender() << "\n";
            cout << "\tSubject: " << currentEmail.getSubject() << "\n";
            cout << "\tDate: " << currentEmail.getDate() << "\n";
        }

        // --------------------------------------------------------
        // READ command
        // --------------------------------------------------------
        else if (line == "READ") {

            if (!emailQueue.isEmpty()) {
                // READ removes the highest-priority email,
                // whether or not NEXT was called first.
                emailQueue.removeMax();
            }
        }

        // --------------------------------------------------------
        // COUNT command
        // --------------------------------------------------------
        else if (line == "COUNT") {
            cout << "There are "
                << emailQueue.size()
                << " emails to read."
                << endl;
        }
    }

    return 0;
}
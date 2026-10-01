 #include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// ------------------------------------------------------------
// Email class
// ------------------------------------------------------------
class Email {
private:
    string sender;
    string subject;
    string date;

    int priority() const {
        if (sender == "Boss")
            return 5;
        else if (sender == "Subordinate")
            return 4;
        else if (sender == "Peer")
            return 3;
        else if (sender == "ImportantPerson")
            return 2;
        else
            return 1; // OtherPerson
    }

    // Converts MM-DD-YYYY into a number that can be compared.
    int dateValue() const {
        int month, day, year;
        char dash1, dash2;

        stringstream ss(date);
        ss >> month >> dash1 >> day >> dash2 >> year;

        return year * 10000 + month * 100 + day;
    }

public:
    Email() {}

    Email(string sender, string subject, string date) {
        this->sender = sender;
        this->subject = subject;
        this->date = date;
    }

    string getSender() const {
        return sender;
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
        return dateValue() > other.dateValue();
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

    // Return the highest-priority email without removing it.
    Email getMax() const {
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

    // Used to remember the email displayed by the most recent NEXT.
    // This is important because two NEXT commands in a row should
    // display the same email.
    bool hasCurrentEmail = false;
    Email currentEmail;

    while (getline(cin, line)) {

        // --------------------------------------------------------
        // EMAIL command
        // --------------------------------------------------------
        if (line.substr(0, 5) == "EMAIL") {
            string data = line.substr(6); // Skip "EMAIL "

            // Find the commas separating the three fields.
            size_t comma1 = data.find(',');
            size_t comma2 = data.find(',', comma1 + 1);

            string sender = data.substr(0, comma1);
            string subject = data.substr(
                comma1 + 1,
                comma2 - comma1 - 1
            );
            string date = data.substr(comma2 + 1);

            Email email(sender, subject, date);
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

            // Only get the highest-priority email.
            // Do NOT remove it.
            currentEmail = emailQueue.getMax();
            hasCurrentEmail = true;

            cout << "Next email:" << endl;
            cout << "Sender: " << currentEmail.getSender() << endl;
            cout << "Subject: " << currentEmail.getSubject() << endl;
            cout << "Date: " << currentEmail.getDate() << endl;
            cout << endl;
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

            hasCurrentEmail = false;
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
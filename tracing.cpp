#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <limits>
#include <stack> // Added for potential future use, though not strictly needed for this refactor
#include <thread> // For idle screen
#include <chrono> // For idle screen
#include <cstdlib> // For system("clear")
#include <termios.h> // For kbhit() on Linux
#include <unistd.h> // For kbhit() on Linux
#include <fcntl.h> // For kbhit() on Linux

using namespace std;

// ----- Helper Functions for UI (from the student management system) -----

// Linux-compatible color codes using ANSI escape sequences
void setColor(int color) {
    // ANSI color codes mapping
    int ansiColors[] = {0, 4, 2, 6, 1, 5, 3, 7, 4, 2, 6, 1, 5, 3, 7};
    if (color >= 0 && color < 15) {
        cout << "\033[1;3" << ansiColors[color] << "m";
    }
}

// Reset color to default
void resetColor() {
    cout << "\033[0m";
}

// Check if a key was pressed (non-blocking)
bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

// Function to display idle screen
void idleScreen() {
    system("clear"); // Clear screen
    
    setColor(0); // Black text on black background (simulates black screen)
    cout << "\n\n\n\n\n\n\n\n";
    cout << "               SYSTEM IDLE MODE ACTIVATED\n";
    cout << "               Press any key to return to menu\n\n";
    
    auto start = chrono::steady_clock::now();
    auto end = start + chrono::minutes(5); // 5 minutes duration
    
    while (chrono::steady_clock::now() < end) {
        if (kbhit()) { // If any key is pressed
            getchar(); // Read the pressed key
            break;
        }
        
        // Calculate remaining time
        auto remaining = chrono::duration_cast<chrono::seconds>(end - chrono::steady_clock::now());
        int minutes = remaining.count() / 60;
        int seconds = remaining.count() % 60;
        
        // Display countdown in the same line
        cout << "\r               Time remaining: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << "   ";
        cout.flush();
        
        this_thread::sleep_for(chrono::milliseconds(100)); // Wait 100ms
    }
    
    resetColor();
    system("clear");
    setColor(10); // Light green
    cout << "\nIdle mode deactivated. Returning to main menu.\n";
    resetColor();
}

// Function to wait for Enter key and clear screen
void waitForEnterAndClear() {
    setColor(14); // Yellow for prompt
    cout << "\nللمواصلة اضغط Enter...";
    resetColor();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("clear");
}

// ----- Data Structures (Unchanged) -----

struct StatusNode {
    string status;
    StatusNode *next;
    StatusNode(const string &s) : status(s), next(nullptr) {}
};

class StatusLog {
private:
    StatusNode *head;
    StatusNode *tail;
public:
    StatusLog() : head(nullptr), tail(nullptr) {}
    void addStatus(const string &status) {
        StatusNode *newNode = new StatusNode(status);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void displayLog() const {
        StatusNode *current = head;
        while (current) {
            cout << "  - " << current->status << endl;
            current = current->next;
        }
    }
    ~StatusLog() {
        StatusNode *current = head;
        while (current) {
            StatusNode *next = current->next;
            delete current;
            current = next;
        }
    }
};

struct Package {
    string trackingNumber;
    string sender;
    string recipient;
    string province;
    string currentStatus;
    StatusLog log;
    Package(string tn, string s, string r, string p)
        : trackingNumber(tn), sender(s), recipient(r), province(p), currentStatus("Arrived at Office") {
        log.addStatus(currentStatus);
    }
};

// ----- Main System Class (Modified for new UI) -----

class TrackingSystem {
private:
    vector<Package> allPackages;
    unordered_map<string, Package *> trackingMap;
    queue<string> deliveryQueue;
public:
    void addPackage() {
        system("clear"); // Clear screen at the beginning
        string tn, s, r, p;
        setColor(13); // Purple for title
        cout << "--- Add New Package ---\n";
        resetColor();
        setColor(14); // Yellow for prompts
        cout << "Tracking Number: ";
        resetColor();
        cin >> tn;
        if (trackingMap.find(tn) != trackingMap.end()) {
            setColor(12); // Red for error
            cout << "Error: This tracking number already exists!\n";
            resetColor();
            waitForEnterAndClear();
            return;
        }
        setColor(14);
        cout << "Sender Name: ";
        resetColor();
        cin.ignore();
        getline(cin, s);
        setColor(14);
        cout << "Recipient Name: ";
        resetColor();
        getline(cin, r);
        setColor(14);
        cout << "Province: ";
        resetColor();
        getline(cin, p);

        allPackages.emplace_back(tn, s, r, p);
        trackingMap[tn] = &allPackages.back();

        setColor(10); // Green for success
        cout << "\nPackage added successfully!\n";
        resetColor();
        waitForEnterAndClear();
    }

    void updateStatus() {
        system("clear"); // Clear screen at the beginning
        string tn;
        setColor(13);
        cout << "--- Update Package Status ---\n";
        resetColor();
        setColor(14);
        cout << "Enter Tracking Number: ";
        resetColor();
        cin >> tn;

        auto it = trackingMap.find(tn);
        if (it == trackingMap.end()) {
            setColor(12);
            cout << "Package not found.\n";
            resetColor();
            waitForEnterAndClear();
            return;
        }

        Package *pkg = it->second;
        setColor(11); // Cyan for info
        cout << "Current Status: " << pkg->currentStatus << "\n";
        resetColor();
        setColor(14);
        cout << "Choose New Status:\n";
        cout << "1. Out for Delivery\n";
        cout << "2. Delivered\n";
        resetColor();
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            pkg->currentStatus = "Out for Delivery";
            pkg->log.addStatus(pkg->currentStatus);
            deliveryQueue.push(tn);
            setColor(10);
            cout << "Status updated and package added to delivery queue.\n";
            resetColor();
        } else if (choice == 2) {
            pkg->currentStatus = "Delivered";
            pkg->log.addStatus(pkg->currentStatus);
            setColor(10);
            cout << "Status updated to 'Delivered'.\n";
            resetColor();
        } else {
            setColor(12);
            cout << "Invalid choice.\n";
            resetColor();
        }
        waitForEnterAndClear();
    }

    void searchByTrackingNumber() {
        system("clear"); // Clear screen at the beginning
        string tn;
        setColor(13);
        cout << "--- Search for a Package ---\n";
        resetColor();
        setColor(14);
        cout << "Enter Tracking Number: ";
        resetColor();
        cin >> tn;

        auto it = trackingMap.find(tn);
        if (it != trackingMap.end()) {
            Package *pkg = it->second;
            setColor(13);
            cout << "\n--- Package Details ---\n";
            resetColor();
            setColor(11);
            cout << "Tracking Number: " << pkg->trackingNumber << "\n";
            cout << "Sender: " << pkg->sender << "\n";
            cout << "Recipient: " << pkg->recipient << "\n";
            cout << "Province: " << pkg->province << "\n";
            cout << "Current Status: " << pkg->currentStatus << "\n";
            cout << "Status Log:\n";
            resetColor();
            pkg->log.displayLog();
        } else {
            setColor(12);
            cout << "No package found with this number.\n";
            resetColor();
        }
        waitForEnterAndClear();
    }

    void displayAllPackages() {
        system("clear"); // Clear screen at the beginning
        setColor(13);
        cout << "--- List of All Packages (" << allPackages.size() << " packages) ---\n";
        resetColor();
        if (allPackages.empty()) {
            setColor(11);
            cout << "There are no packages currently.\n";
            resetColor();
        } else {
            for (const auto &pkg : allPackages) {
                setColor(11);
                cout << "--------------------------------\n";
                cout << "Tracking Number: " << pkg.trackingNumber << "\n";
                cout << "Recipient: " << pkg.recipient << "\n";
                cout << "Province: " << pkg.province << "\n";
                cout << "Status: " << pkg.currentStatus << "\n";
                resetColor();
            }
        }
        waitForEnterAndClear();
    }

    void displayPackagesByProvince() {
        system("clear"); // Clear screen at the beginning
        string p;
        setColor(13);
        cout << "--- Display Packages for a Specific Province ---\n";
        resetColor();
        setColor(14);
        cout << "Enter Province Name: ";
        resetColor();
        cin.ignore();
        getline(cin, p);

        setColor(13);
        cout << "\n--- Packages in " << p << " Province ---\n";
        resetColor();
        bool found = false;
        for (const auto &pkg : allPackages) {
            if (pkg.province == p) {
                setColor(11);
                cout << "--------------------------------\n";
                cout << "Tracking Number: " << pkg.trackingNumber << "\n";
                cout << "Recipient: " << pkg.recipient << "\n";
                cout << "Status: " << pkg.currentStatus << "\n";
                resetColor();
                found = true;
            }
        }
        if (!found) {
            setColor(11);
            cout << "No packages found for this province.\n";
            resetColor();
        }
        waitForEnterAndClear();
    }

    void displayUndeliveredPackages() {
        system("clear"); // Clear screen at the beginning
        setColor(13);
        cout << "--- Undelivered Packages ---\n";
        resetColor();
        bool found = false;
        for (const auto &pkg : allPackages) {
            if (pkg.currentStatus != "Delivered") {
                setColor(11);
                cout << "--------------------------------\n";
                cout << "Tracking Number: " << pkg.trackingNumber << "\n";
                cout << "Recipient: " << pkg.recipient << "\n";
                cout << "Province: " << pkg.province << "\n";
                cout << "Status: " << pkg.currentStatus << "\n";
                resetColor();
                found = true;
            }
        }
        if (!found) {
            setColor(11);
            cout << "All packages have been delivered.\n";
            resetColor();
        }
        waitForEnterAndClear();
    }

    void deleteDeliveredPackage() {
        system("clear"); // Clear screen at the beginning
        string tn;
        setColor(13);
        cout << "--- Delete a Delivered Package ---\n";
        resetColor();
        setColor(14);
        cout << "Enter Tracking Number of the package to delete: ";
        resetColor();
        cin >> tn;

        auto it = trackingMap.find(tn);
        if (it == trackingMap.end()) {
            setColor(12);
            cout << "Package not found.\n";
            resetColor();
            waitForEnterAndClear();
            return;
        }

        Package *pkg = it->second;
        if (pkg->currentStatus != "Delivered") {
            setColor(12);
            cout << "Cannot delete this package because it has not been delivered yet.\n";
            resetColor();
            waitForEnterAndClear();
            return;
        }

        allPackages.erase(remove_if(allPackages.begin(), allPackages.end(),
                                    [&](const Package &p) { return p.trackingNumber == tn; }),
                          allPackages.end());
        trackingMap.erase(it);

        setColor(10);
        cout << "Package deleted successfully.\n";
        resetColor();
        waitForEnterAndClear();
    }

    void displayStatusCounts() {
        system("clear"); // Clear screen at the beginning
        setColor(13);
        cout << "--- Status Statistics ---\n";
        resetColor();
        unordered_map<string, int> counts;
        counts["Arrived at Office"] = 0;
        counts["Out for Delivery"] = 0;
        counts["Delivered"] = 0;

        for (const auto &pkg : allPackages) {
            counts[pkg.currentStatus]++;
        }

        setColor(11);
        cout << "Packages at Office: " << counts["Arrived at Office"] << "\n";
        cout << "Packages Out for Delivery: " << counts["Out for Delivery"] << "\n";
        cout << "Packages Delivered: " << counts["Delivered"] << "\n";
        resetColor();
        waitForEnterAndClear();
    }
};

// ----- Main Menu and Main Function (Modified) -----

void showMenu() {
    setColor(11); // Cyan color for title
    cout << "\n========== Postal Package Tracking System ==========\n";
    resetColor();
    setColor(10); // Light green for options
    cout << "1. Add New Package\n";
    cout << "2. Update Package Status\n";
    cout << "3. Search for a Package (by Tracking Number)\n";
    cout << "4. Display All Packages\n";
    cout << "5. Display Packages by Province\n";
    cout << "6. Display Undelivered Packages\n";
    cout << "7. Delete a Delivered Package\n";
    cout << "8. Display Status Counts\n";
    cout << "9. Activate Idle Screen (5 minutes)\n";
    cout << "0. Exit\n";
    resetColor();
    setColor(14); // Yellow for prompt
    cout << "Enter your choice: ";
    resetColor();
}

int main() {
    TrackingSystem system;
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                system.addPackage();
                break;
            case 2:
                system.updateStatus();
                break;
            case 3:
                system.searchByTrackingNumber();
                break;
            case 4:
                system.displayAllPackages();
                break;
            case 5:
                system.displayPackagesByProvince();
                break;
            case 6:
                system.displayUndeliveredPackages();
                break;
            case 7:
                system.deleteDeliveredPackage();
                break;
            case 8:
                system.displayStatusCounts();
                break;
            case 9:
                idleScreen();
                break;
            case 0:
                setColor(10); // Light green for exit message
                cout << "Thank you for using the system. Goodbye!\n";
                resetColor();
                break;
            default:
                setColor(12); // Red for error
                cout << "Invalid choice! Please try again.\n";
                resetColor();
                waitForEnterAndClear();
                break;
        }

    } while (choice != 0);

    return 0;
}
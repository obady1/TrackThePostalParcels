# Postal Package Tracking System

A console-based postal package tracking system written in C++ that allows users to manage package records, update delivery statuses, search packages, generate statistics, and monitor delivery progress through a terminal interface.

> **Note:** The project name is inferred from the source code and application menu.

---

## Features

| Feature | Description |
|----------|-------------|
| Add Package | Create a new package with tracking number, sender, recipient, and province |
| Tracking Lookup | Search packages using tracking numbers |
| Status Management | Update package delivery status |
| Status History | Keep a complete log of status changes |
| Package Listing | Display all registered packages |
| Province Filtering | View packages belonging to a specific province |
| Undelivered Packages | Display all packages not yet delivered |
| Package Deletion | Delete packages after successful delivery |
| Statistics | Show package counts by status |
| Delivery Queue | Track packages marked as out for delivery |
| Idle Mode | Activate a timed idle screen |
| Colored Terminal UI | ANSI color-based user interface |

---

## Technologies Used

### Language

- C++

### Standard Library Components

- vector
- unordered_map
- queue
- string
- algorithm
- limits
- thread
- chrono

### Platform-Specific Libraries

- termios.h
- unistd.h
- fcntl.h

> These libraries indicate that the application is intended for Linux/Unix-like environments.

---

## Project Structure

```text
tracing.cpp
│
├── UI Helper Functions
│   ├── setColor()
│   ├── resetColor()
│   ├── kbhit()
│   ├── idleScreen()
│   └── waitForEnterAndClear()
│
├── Data Structures
│   ├── StatusNode
│   ├── StatusLog
│   └── Package
│
├── TrackingSystem
│   ├── addPackage()
│   ├── updateStatus()
│   ├── searchByTrackingNumber()
│   ├── displayAllPackages()
│   ├── displayPackagesByProvince()
│   ├── displayUndeliveredPackages()
│   ├── deleteDeliveredPackage()
│   └── displayStatusCounts()
│
├── Menu System
│   └── showMenu()
│
└── main()
```

---

## Data Model

### Package

Represents a postal package.

| Field | Type |
|---------|---------|
| trackingNumber | string |
| sender | string |
| recipient | string |
| province | string |
| currentStatus | string |
| log | StatusLog |

Default status:

```text
Arrived at Office
```

---

### StatusLog

A linked-list-based structure used to store package status history.

Supported operations:

- Add status entry
- Display status history
- Automatic memory cleanup

---

## Package Status Workflow

```text
Arrived at Office
        ↓
Out for Delivery
        ↓
Delivered
```

Each status transition is recorded in the package history log.

---

## Core Functionalities

### Add New Package

Creates a package and stores it in:

- Package collection
- Tracking lookup map

Validation:

- Prevents duplicate tracking numbers

---

### Update Package Status

Available status updates:

```text
1. Out for Delivery
2. Delivered
```

When a package is marked as:

```text
Out for Delivery
```

its tracking number is added to the delivery queue.

---

### Search Package

Search by:

```text
Tracking Number
```

Displays:

- Tracking Number
- Sender
- Recipient
- Province
- Current Status
- Status History

---

### Display All Packages

Displays a summary of all registered packages.

---

### Display Packages by Province

Filters packages according to the specified province.

---

### Display Undelivered Packages

Displays all packages whose status is not:

```text
Delivered
```

---

### Delete Delivered Package

Conditions:

- Package must exist
- Package status must be `Delivered`

Otherwise deletion is rejected.

---

### Display Status Statistics

Shows counts for:

- Arrived at Office
- Out for Delivery
- Delivered

---

### Idle Screen

Features:

- 5-minute timer
- Countdown display
- Returns immediately when any key is pressed

---

## Internal Data Structures

### Package Storage

```cpp
vector<Package> allPackages;
```

Stores all package records.

---

### Tracking Index

```cpp
unordered_map<string, Package*> trackingMap;
```

Provides fast lookup by tracking number.

---

### Delivery Queue

```cpp
queue<string> deliveryQueue;
```

Stores tracking numbers of packages currently out for delivery.

---

## Requirements

### Operating System

- Linux
- Unix-like systems

### Compiler

A C++ compiler supporting:

```text
C++11 or newer
```

Required because the project uses:

- std::thread
- std::chrono
- range-based loops
- modern STL containers

---

## Build

### GCC

```bash
g++ tracing.cpp -o tracking_system -std=c++11
```

### GCC with pthread support

```bash
g++ tracing.cpp -o tracking_system -std=c++11 -pthread
```

---

## Run

```bash
./tracking_system
```

---

## Main Menu

```text
1. Add New Package
2. Update Package Status
3. Search for a Package (by Tracking Number)
4. Display All Packages
5. Display Packages by Province
6. Display Undelivered Packages
7. Delete a Delivered Package
8. Display Status Counts
9. Activate Idle Screen (5 minutes)
0. Exit
```

---

## Example Usage

### Add Package

```text
Tracking Number: PKG001
Sender Name: Ahmed
Recipient Name: Ali
Province: Aden
```

Initial status:

```text
Arrived at Office
```

---

### Update Status

```text
Enter Tracking Number: PKG001

1. Out for Delivery
2. Delivered
```

---

### Search Package

```text
Tracking Number: PKG001

Sender: Ahmed
Recipient: Ali
Province: Aden
Current Status: Delivered

Status Log:
- Arrived at Office
- Out for Delivery
- Delivered
```

---

## Limitations

- Data is stored only in memory
- No database integration
- No file persistence
- Data is lost when the application exits
- Delivery queue is maintained internally but has no dedicated display screen
- Screen clearing depends on:

```cpp
system("clear");
```

which is platform-dependent.

---

## License

No license information was found in the provided source code.
/*
 * Course: COEN 2220 - Programming 2
 * Name: Logan N. Ramirez Rodriguez
 * Lab: Lab 12 - Quicksort and Searching
 * Description: Preserve parcel arrival records and search a ready-only view
 * Due date: 11/10/2026
 */

#include <iostream>
#include <string>
using namespace std;

struct Parcel
{
    int pickupCode; // Unique lookup key supplied by the recipient.
    string locker; // Physical location, not a sorting key.
    bool ready;    // Only ready parcels belong in the lookup view.
};

void displayParcels(const Parcel parcels[], int count);
void displayReady(Parcel *view[], int count);
void swapParcelPointers(Parcel *&left, Parcel *&right);
int partitionReady(Parcel *view[], int first, int last);
void quickSortReady(Parcel *view[], int first, int last);
int binaryFindReady(Parcel *view[], int count, int target);

int main()
{
    const int CAPACITY = 8;
    Parcel parcels[CAPACITY] = {
        {640, "L-4", true},  {215, "L-8", false},
        {580, "L-2", true},  {325, "L-6", true},
        {710, "L-1", true},  {460, "L-5", false},
        {190, "L-3", true},  {505, "L-7", true}
    };
    Parcel *readyView[CAPACITY]; // Borrow addresses; do not delete these objects.
    int readyCount = 0;

    cout << boolalpha << "Original arrival records:" << endl;
    displayParcels(parcels, CAPACITY);
    for (int index = 0; index < CAPACITY; index++)
    {
        if (parcels[index].ready)
        {
            readyView[readyCount] = &parcels[index];
            readyCount++; // Count initialized view positions only.
        }
    }

    quickSortReady(readyView, 0, readyCount - 1);
    cout << "Ready parcels by pickup code:" << endl;
    displayReady(readyView, readyCount);
    cout << "Original arrival records after sorting:" << endl;
    displayParcels(parcels, CAPACITY);

    // --- STEP D1: Activate after completing the search function ---
    int target;
    cout << "Enter a pickup code: ";
    if (!(cin >> target))
    {
        cout << "Invalid code input." << endl;
    }
    else
    {
        int position = binaryFindReady(readyView, readyCount, target);
        if (position == -1)
        {
            cout << "No ready parcel with that code." << endl;
        }
        else
        {
            cout << "Pickup code " << readyView[position]->pickupCode
                 << " | Locker: " << readyView[position]->locker << endl;
        }
    }
    return 0;
}

void swapParcelPointers(Parcel *&left, Parcel *&right)
{
    Parcel *saved = left;
    left = right;
    right = saved;
}

int partitionReady(Parcel *view[], int first, int last)
{
    int middle = first + (last - first) / 2;
    swapParcelPointers(view[first], view[middle]);
    int pivotCode = view[first]->pickupCode;
    int boundary = first;

    for (int scan = first + 1; scan <= last; scan++)
    {
        if (view[scan]->pickupCode < pivotCode)
        {
            boundary++;
            swapParcelPointers(view[boundary], view[scan]);
        }
    }
    swapParcelPointers(view[first], view[boundary]);

    return boundary;
}

void quickSortReady(Parcel *view[], int first, int last)
{
    if (first < last)
    {
        int pivotIndex = partitionReady(view, first, last);
        quickSortReady(view, first, pivotIndex - 1);
        quickSortReady(view, pivotIndex + 1, last);
    }
}

int binaryFindReady(Parcel *view[], int count, int target)
{
    int first = 0;
    int last = count - 1;
    int result = -1;

    while (first <= last && result == -1)
    {
        int middle = first + (last - first) / 2;
        if (view[middle]->pickupCode == target)
        {
            result = middle;
        }
        else if (view[middle]->pickupCode < target)
        {
            first = middle + 1;
        }
        else
        {
            last = middle - 1;
        }
    }

    return result;
}

void displayParcels(const Parcel parcels[], int count)
{
    for (int index = 0; index < count; index++)
    {
        cout << "Code: " << parcels[index].pickupCode
             << " | Locker: " << parcels[index].locker
             << " | Ready: " << parcels[index].ready << endl;
    }
}

void displayReady(Parcel *view[], int count)
{
    for (int index = 0; index < count; index++)
    {
        cout << "View index: " << index
             << " | Code: " << view[index]->pickupCode
             << " | Locker: " << view[index]->locker << endl;
    }
}

/*
 * Analysis Answers
 *
 * C3 - Partition decisions
 * Trace the first partition of the six-pointer ready view. Record first, last,
 * middle, pivotCode, boundary, scan, and codes before and after each comparison.
 * Include the loop's final false test and the final pivot exchange.
 * Answer:
 * The ready view starts as 640 580 325 710 190 505, so first is 0 and last is 5.
 * middle is 0 + (5 - 0) / 2 = 2, which is code 325. After the first swap the view
 * is 325 580 640 710 190 505, pivotCode is 325 and boundary is 0.
 * scan 1: 580 < 325 is false, boundary stays 0.
 * scan 2: 640 < 325 is false, boundary stays 0.
 * scan 3: 710 < 325 is false, boundary stays 0.
 * scan 4: 190 < 325 is true, boundary becomes 1 and positions 1 and 4 swap. The view
 * is 325 190 640 710 580 505.
 * scan 5: 505 < 325 is false, boundary stays 1.
 * scan 6: 6 <= 5 is false, so the loop ends and nothing is read at position 6.
 * Last exchange: positions 0 and 1 swap, so the view is 190 325 640 710 580 505 and
 * the function returns 1. The pivot 325 is in its final place. The next ranges are
 * 0..0 and 2..5.
 *
 * C4 - Data and memory
 * Explain what changes when swapping two Parcel pointers by reference.
 * Identify who owns the objects and why the view must not delete them.
 * Answer:
 * Swapping two Parcel pointers by reference only changes which address each position
 * of readyView has. The Parcel objects do not change, their fields stay the same and
 * they stay in the same place in parcels. The parcels array in main owns the objects.
 * The view only borrows their addresses and it never uses new, so it must not call
 * delete. A delete would be wrong because those objects were not created with new,
 * and it would also break records that the original array still needs.
 *
 * D3 - Search positions and absence
 * Trace code 505 and the absent ready-view code 460. Give the checked ranges
 * and explain why a view index cannot be used as an index into parcels.
 * Answer:
 * The sorted view is 190 325 505 580 640 710 with view indexes 0 to 5.
 * Code 505: first 0, last 5, middle 2. The code at 2 is 505, so it is found right
 * away and the function returns 2. The locker is L-7.
 * Code 460: range 0..5, middle 2 has 505 which is bigger, so the next range is 0..1.
 * Middle 0 has 190 which is smaller, so the range is 1..1. Middle 1 has 325 which is
 * smaller, so the range is 2..1. Now first is bigger than last, the loop ends and
 * the function returns -1. Then main prints that there is no ready parcel with that
 * code.
 * The 2 returned for 505 is a position in readyView, not in parcels. In parcels the
 * 505 parcel is at index 7 and parcels[2] is the 580 parcel, so using the view index
 * in the original array would give the wrong parcel.
 *
 * D4 - Time and auxiliary space
 * For n ready parcels and q queries, compare sorting once then using binary
 * search with scanning the unsorted ready view for every query. Give worst-case
 * bounds for this quicksort, balanced-partition bounds, and iterative-search
 * auxiliary space. Distinguish the view storage from recursive-call storage.
 * Answer:
 * n is the number of ready parcels and q is the number of queries.
 * Sorting once with this quicksort takes O(n log n) time when the partitions are
 * balanced and O(n^2) in the worst case, because choosing the middle does not
 * guarantee balanced partitions. After that each query with binary search takes
 * O(log n), so q queries take O(q log n). The balanced total is O(n log n + q log n).
 * Scanning the unsorted view for every query takes O(n) per query, so O(q * n) in
 * total. Sorting once is better when there are many queries. For only a few queries
 * the scan can be cheaper than sorting.
 * The iterative binary search uses O(1) auxiliary space, just a few int variables.
 * Quicksort uses O(log n) auxiliary space for the recursive calls when it is
 * balanced and O(n) in the worst case. The view itself is O(n) pointers, but that
 * storage is needed anyway for the queries and it is not recursive-call storage.
 *
 * Extra tests (observed results)
 * Empty range 0..-1: quickSortReady returns without reading any position.
 * Single pointer 0..0: the view stays the same (300, L-1).
 * Three pointers 300 100 200 sorted: 100 200 300. Sorted a second time: 100 200 300.
 * Search with count 0 for 100: returns -1 without reading view[0].
 * Search in a view with one pointer (300): code 300 returns 0 and code 301 returns -1.
 * After all the tests the original parcels kept the same codes, lockers and positions.
 */

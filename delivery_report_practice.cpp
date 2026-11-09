/*
 * Course: COEN 2220 - Programming 2
 * Name: Logan N. Ramirez Rodriguez
 * Lab: Lab 12 - Quicksort and Searching
 * Description: Practice - sort a temporary delivery report and search it by order ID
 * Due date: 11/10/2026
 */

#include <iostream>
#include <string>
using namespace std;

struct OrderPreview
{
    int orderId;
    string orderType;
    string status;
    string building;
    int priority;
};

void displayOrders(const OrderPreview orders[], int count);
void displayView(OrderPreview *view[], int count);
bool comesBeforeInReport(const OrderPreview *left, const OrderPreview *right);
void swapOrderPointers(OrderPreview *&left, OrderPreview *&right);
int partitionOrders(OrderPreview *view[], int first, int last);
void quickSortOrders(OrderPreview *view[], int first, int last);
int findOrderInView(OrderPreview *view[], int count, int target);

int main()
{
    const int ORDER_COUNT = 6;
    OrderPreview orders[ORDER_COUNT] =
    {
        {420, "DELIVERY", "PENDING", "Biblioteca", 1},
        {180, "DELIVERY", "IN_PROGRESS", "Ingenieria", 3},
        {650, "DINE_IN", "PENDING", "Cafeteria-Lunch", 1},
        {310, "DELIVERY", "PENDING", "Biblioteca", 3},
        {205, "DELIVERY", "PENDING", "Biblioteca", 3},
        {900, "DELIVERY", "COMPLETED", "Ingenieria", 2}
    };
    OrderPreview *view[ORDER_COUNT];
    int viewCount = 0;
    int target;

    cout << "Orders before building the report:" << endl;
    displayOrders(orders, ORDER_COUNT);

    for (int h = 0; h < ORDER_COUNT; h++)
    {
        bool isActive = orders[h].status == "PENDING" || orders[h].status == "IN_PROGRESS";

        if (orders[h].orderType == "DELIVERY" && isActive)
        {
            view[viewCount] = &orders[h];
            viewCount++;
        }
    }

    quickSortOrders(view, 0, viewCount - 1);
    cout << "Delivery report:" << endl;
    displayView(view, viewCount);

    cout << "Enter an order ID: ";
    if (cin >> target)
    {
        int position = findOrderInView(view, viewCount, target);

        if (position == -1)
        {
            cout << "No active delivery order with that ID in the report." << endl;
        }
        else
        {
            cout << "Order " << view[position]->orderId
                 << " | Building: " << view[position]->building
                 << " | Status: " << view[position]->status
                 << " | Priority: " << view[position]->priority << endl;
        }
    }
    else
    {
        cout << "Invalid ID input." << endl;
    }

    cout << "Orders after building the report:" << endl;
    displayOrders(orders, ORDER_COUNT);

    return 0;
}

void displayOrders(const OrderPreview orders[], int count)
{
    for (int h = 0; h < count; h++)
    {
        cout << "Order: " << orders[h].orderId
             << " | Type: " << orders[h].orderType
             << " | Status: " << orders[h].status
             << " | Building: " << orders[h].building
             << " | Priority: " << orders[h].priority << endl;
    }
}

void displayView(OrderPreview *view[], int count)
{
    for (int h = 0; h < count; h++)
    {
        cout << "Report position: " << h
             << " | Order: " << view[h]->orderId
             << " | Building: " << view[h]->building
             << " | Priority: " << view[h]->priority << endl;
    }
}

bool comesBeforeInReport(const OrderPreview *left, const OrderPreview *right)
{
    bool result;

    if (left->building != right->building)
    {
        result = left->building < right->building;
    }
    else if (left->priority != right->priority)
    {
        result = left->priority > right->priority;
    }
    else
    {
        result = left->orderId < right->orderId;
    }

    return result;
}

void swapOrderPointers(OrderPreview *&left, OrderPreview *&right)
{
    OrderPreview *saved = left;
    left = right;
    right = saved;
}

int partitionOrders(OrderPreview *view[], int first, int last)
{
    int middle = first + (last - first) / 2;
    swapOrderPointers(view[first], view[middle]);
    OrderPreview *pivot = view[first];
    int boundary = first;

    for (int scan = first + 1; scan <= last; scan++)
    {
        if (comesBeforeInReport(view[scan], pivot))
        {
            boundary++;
            swapOrderPointers(view[boundary], view[scan]);
        }
    }
    swapOrderPointers(view[first], view[boundary]);

    return boundary;
}

void quickSortOrders(OrderPreview *view[], int first, int last)
{
    if (first < last)
    {
        int pivotIndex = partitionOrders(view, first, last);
        quickSortOrders(view, first, pivotIndex - 1);
        quickSortOrders(view, pivotIndex + 1, last);
    }
}

int findOrderInView(OrderPreview *view[], int count, int target)
{
    int result = -1;

    for (int h = 0; h < count && result == -1; h++)
    {
        if (view[h]->orderId == target)
        {
            result = h;
        }
    }

    return result;
}

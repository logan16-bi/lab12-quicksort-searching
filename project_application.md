# Lab 12 - Project Application

## E1 - Filter and ownership
The selected IDs are 420, 180, 310 and 205. They are DELIVERY orders and their status
is PENDING or IN_PROGRESS. The excluded IDs are 650, because it is DINE_IN, and 900,
because it is COMPLETED. The array orders owns all six records. The view only has
pointers to the four selected ones, so no copies are made and there is no new or delete.

## E2 - Report order and algorithm
The report I got was 205, 310, 420, 180. The building decides first, from A to Z, so
Biblioteca goes before Ingenieria and that is why 180 is last. Inside Biblioteca the
higher priority goes first, so 205 and 310 (priority 3) go before 420 (priority 1). 205
and 310 have the same building and the same priority, so the lower ID goes first.
I used quicksort on the pointers of the view, with the middle record as the pivot. It
takes O(n log n) time when the partitions are balanced and O(n^2) in the worst case.
The auxiliary space is O(log n) for the recursive calls when it is balanced and O(n)
in the worst case. The view itself is O(n) pointers.

## E3 - Lookup choice and tests
The report is ordered by building and priority, so the IDs in the view are not
ascending: 205, 310, 420, 180. Binary search by orderId would not work. For example,
searching 180 compares with 310 first, which is bigger, then with 205, which is also
bigger, and it would say that 180 is not there when it is in the last position. That is
why I used a linear search on the view, which takes O(n) time and O(1) extra space.
Tests:
- ID 310 (found): Biblioteca, PENDING, priority 3.
- ID 900 (excluded): "No active delivery order with that ID in the report." because it
  is COMPLETED and it is not in the view.
- ID 777 (nonexistent): the same message.
An extra view sorted by orderId would need another array of n pointers and an O(n log n)
sort. After that it would allow binary search in O(log n) for each ID, but it only pays
off if there are many queries.

## E4 - Evidence and project connection
Source: delivery_report_practice.cpp
Output: delivery_report_output.txt
To check that the original records did not change, the program prints all the fields of
orders before and after building the report and doing the search. Both prints are the
same, with the same IDs, types, statuses, buildings and priorities in the same positions.
In the project this keeps the operational FIFO sequence safe. Only the pointers in the
temporary view are moved, so the order in which the orders are prepared does not change.

In this assignment, we will create a simplified TV guide system. We will build up a database of
channels, and shows that are offered by each channel. And we will be able to find shows based on their
channel, name, day of the week, and time. (We will not, however, be able to delete a show or channel
once it’s in the system.)
As with earlier programs, we will provide the user interface code. Your job is to implement and
maintain the collection of channels and shows, and to search for shows using various criteria. Because
the number of channels and shows are not known ahead of time, and because the collection is built up
incrementally (one channel/show at a time), you will need to use the linked list data structure.
You will also be implementing two abstract data types, in which the details of the data representation of
the type is not visible to the user of the type. The user can only a pre-defined set of functions (known as
the “interface”) to access or change the data type values. (We will discuss this at length in class.)
To implement this program, you will need to know how to do the following:
• Add items to a linked list.
• Search for items in a linked list.